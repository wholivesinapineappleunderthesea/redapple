#include "defs.h"
#ifndef helper_h
#define helper_h
class MemoryManager {
public:
    pid_t getProcessId(const char * procName) {
        pid_t pids[1024];
        proc_listpids(PROC_ALL_PIDS, 0, pids, 1024);
        for (int i = 1; i < sizeof(pids); i++) {
            char curpidName[1024];
            proc_name(pids[i], curpidName, sizeof(curpidName));
            if (!strncmp(curpidName, procName, sizeof(procName))) return pids[i];
        }
        return 0;
    }
    int getTask(pid_t pid) {
        task_t task;
        if (task_for_pid(current_task(), pid, &task) == KERN_FAILURE) errorExit(2, " Yoinks! task_for_pid() failure... You might need to disable SIP (rootless)...");
        return task;
    }
    mach_vm_address_t getModule(const char * modName) {
        struct task_dyld_info dyldinfo;
        mach_vm_address_t address = NULL;
        mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
        if (task_info(csgoTask, TASK_DYLD_INFO, (task_info_t)&dyldinfo, &count) == KERN_SUCCESS)
            address = dyldinfo.all_image_info_addr;
        struct dyld_all_image_infos *dyldaii;
        mach_msg_type_number_t size = sizeof(dyld_all_image_infos);
        vm_offset_t readOut;
        kern_return_t kr = vm_read(csgoTask, address, size, &readOut, &size);
        if (kr == KERN_FAILURE)
            errorExit(3, " Shizzle, KERN_FAILURE...");
        dyldaii = (dyld_all_image_infos *)readOut;
        int imgCount = dyldaii->infoArrayCount;
        mach_msg_type_number_t dataCount = imgCount * 0x24;
        struct dyld_image_info *g_dii = NULL;
        g_dii = (struct dyld_image_info *)malloc(dataCount);
        kr = vm_read(csgoTask, (mach_vm_address_t)dyldaii->infoArray, dataCount, &readOut, &dataCount);
        if (kr == KERN_FAILURE)
            errorExit(4, " Shizzle v2, KERN_FAILURE...");
        struct dyld_image_info *dii = (struct dyld_image_info *)readOut;
        for (int i = 0; i < imgCount; i++) {
            dataCount = 1024;
            vm_read(csgoTask, (mach_vm_address_t)dii[i].imageFilePath, dataCount, &readOut, &dataCount);
            char *imgName = (char *)readOut;
            if (imgName) g_dii[i].imageFilePath = strdup(imgName); else g_dii[i].imageFilePath = NULL;
            g_dii[i].imageLoadAddress = dii[i].imageLoadAddress;
            if (strstr(imgName, modName) != NULL) return (mach_vm_address_t)dii[i].imageLoadAddress;
        }
        return NULL;
    }
    template <typename type>
    type read(mach_vm_address_t address, size_t extraSize = sizeof(type)) {
        type content;
        vm_offset_t data;
        uint32_t sz;
        kern_return_t kr = vm_read(csgoTask, (vm_address_t)address, extraSize, &data, &sz);
        if(kr == KERN_SUCCESS) {
            content = (type) *(type *)(data);
            vm_deallocate(current_task(), data, sz);
        }
        
        return content;
    }
    template <typename type>
    bool write(mach_vm_address_t address, type data) {
        kern_return_t kr = vm_write(csgoTask, address, (vm_offset_t)&data, sizeof(data));
        if (kr == KERN_SUCCESS) {
            return true;
        }
        
        return false;
    }
};
MemoryManager * Memory = new MemoryManager();

class memstuffs {
public:
    void initCSGOPid() {
        csgoPid = Memory->getProcessId("csgo_osx64");
        if (csgoPid == 0) errorExit(1, " Zoinks! Couldn't find CS:GO! Are you sure CS:GO is running?");
    }
    void getCSGOTask() {
        csgoTask = Memory->getTask(csgoPid);
    }
    void getClient() {
        clientModule = Memory->getModule("/client_panorama.dylib");
        if (clientModule == NULL) errorExit(5, " Hocking Heck! Couldn't get client_panorama.dylib!");
    }
    void getEngine() {
        engineModule = Memory->getModule("/engine.dylib");
        if (engineModule == NULL) errorExit(6, " Dang! Couldn't get engine.dylib!");
    }
    void setupGlowManager() {
        glowObjectManager = Memory->read<uint64_t>(clientModule + dwGlowObjectManager);
    }
};
memstuffs * memHelper = new memstuffs(); // Game helper

class player {
public:
    uint64_t playerAddress;
    bool isDoingObjective() {
        if (Memory->read<bool>(this->playerAddress + isTakingHostage) || Memory->read<bool>(playerAddress + isDefusing))
            return true;
        return false;
    }
    int health() {
        return Memory->read<int>(this->playerAddress + healthOffset);
    }
    int team() {
        return Memory->read<int>(this->playerAddress + teamOffset);
    }
    int glowIndex() {
        return Memory->read<int>(this->playerAddress + m_iGlowIndex);
    }
    int flags() {
        return Memory->read<int>(this->playerAddress + m_fFlags);
    }
    bool isScoped() {
        return Memory->read<bool>(this->playerAddress + b_isScoped);
    }
    bool isLivePlayer() {
        if (this->health() > 0)
            return true; // Sometimes I would get a player that ain't real
        return false; // so now I check if they have a real team
    }
    void initialize(int index) {
        this->playerAddress = Memory->read<uint64_t>(clientModule + dwEntityList + index * 0x10);
    }
    void initializeXhair(int index) {
        this->playerAddress = Memory->read<uint64_t>(clientModule + dwEntityList + index * 0x20);
    }
    void initializeLocal() {
        this->playerAddress = Memory->read<uint64_t>(clientModule + dwLocalPlayer);
    }
    void glowOutline(int ourTeam) {
        uint64_t glowBase = glowObjectManager + (0x40 * this->glowIndex());
        clr playerGlowColor;
        if (this->isLivePlayer()) {
            if (this->isDoingObjective()) { // Defusing or grabbing, different colour
                playerGlowColor.r = 0.0f;
                playerGlowColor.g = 255.0f;
                playerGlowColor.b = 255.0f;
                playerGlowColor.a = 10.0f;
            } else if (this->team() == ourTeam) { // Teammate
                playerGlowColor.r = 0.0f;
                playerGlowColor.g = 1.0f;
                playerGlowColor.b = 255.0f;
                playerGlowColor.a = 0.25f;
            } else if (this->isScoped()) { // Scoped In
                playerGlowColor.r = 0.0f;
                playerGlowColor.g = 125.0f;
                playerGlowColor.b = 0.0f;
                playerGlowColor.a = 0.75f;
            } else { // Enemy
                playerGlowColor.r = 255.0f;
                playerGlowColor.g = 0.0f;
                playerGlowColor.b = 0.0f;
                playerGlowColor.a = 0.5f;
            }
            Memory->write<clr>(glowBase + 0x8, playerGlowColor);
            Memory->write<bool>(glowBase + 0x28, true); // Render when occluded
        }
    }
};
#endif /* helper_h */
