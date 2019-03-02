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
    mach_vm_address_t getModule(const char * modName) { // Sorry about code wall future me
        struct task_dyld_info dyldinfo;
        mach_vm_address_t address = NULL;
        mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
        if (task_info(csgoTask, TASK_DYLD_INFO, (task_info_t)&dyldinfo, &count) == KERN_SUCCESS) address = dyldinfo.all_image_info_addr;
        struct dyld_all_image_infos *dyldaii;
        mach_msg_type_number_t size = sizeof(dyld_all_image_infos);
        vm_offset_t readOut;
        kern_return_t kr = vm_read(csgoTask, address, size, &readOut, &size);
        if (kr == KERN_FAILURE) errorExit(3, " Shizzle, KERN_FAILURE...");
        dyldaii = (dyld_all_image_infos *)readOut;
        int imgCount = dyldaii->infoArrayCount;
        mach_msg_type_number_t dataCount = imgCount * 0x24;
        struct dyld_image_info *g_dii = NULL;
        g_dii = (struct dyld_image_info *)malloc(dataCount);
        kr = vm_read(csgoTask, (mach_vm_address_t)dyldaii->infoArray, dataCount, &readOut, &dataCount);
        if (kr == KERN_FAILURE) errorExit(4, " Shizzle v2, KERN_FAILURE...");
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
        if (address != NULL) {
            kern_return_t kr = vm_write(csgoTask, address, (vm_offset_t)&data, sizeof(data));
            if (kr == KERN_SUCCESS) return true;
        }
        return false;
    }
    uint8_t * readTo(vm_address_t modStart, size_t modSize) {
        uint8_t * buf = nullptr;
        vm_offset_t memoryFromRead;
        mach_msg_type_number_t maxSize = (mach_msg_type_number_t)modSize;
        kern_return_t kr = vm_read(csgoTask, modStart, maxSize, &memoryFromRead, &maxSize);
        if (kr == KERN_FAILURE) errorExit(69, " Reading data from game into buffer failed, pattern scanner failure");
        uint64_t address = (uint64_t)memoryFromRead;
        buf = (uint8_t *)address;
        return buf;
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
        glowObjectManager = Memory->read<uint64_t>(clientModule + offsets::dwGlowObjectManager);
    }
    void setup() {
        initCSGOPid();
        getCSGOTask();
        getClient();
        getEngine();
        setupGlowManager();
    }
};
memstuffs * memHelper = new memstuffs(); // Game helper
float r = 200.0f, g = 0.0f, b = 200.0f;
bool decreasered, decreasegreen, decreaseblue;
class player {
public:
    uint64_t playerAddress;
    bool isDoingObjective() {
        if (Memory->read<bool>(this->playerAddress + offsets::m_bIsGrabbingHostage) || Memory->read<bool>(playerAddress + offsets::m_bIsDefusing))
            return true;
        return false;
    }
    Vec3 playerPosition() {
        return Memory->read<Vec3>(this->playerAddress + 0x170);
    }
    void initialize(int index) {
        this->playerAddress = Memory->read<uint64_t>(clientModule + offsets::dwEntityList + index * 0x10);
    }
    void initializeXhair(int index) {
        this->playerAddress = Memory->read<uint64_t>(clientModule + offsets::dwEntityList + index * 0x20);
    }
    void initializeLocal() {
        this->playerAddress = Memory->read<uint64_t>(clientModule + offsets::dwLocalPlayer);
    }
    void glowOutline(int ourTeam) {
        uint64_t glowBase = glowObjectManager + (0x40 * Memory->read<int>(this->playerAddress + offsets::m_iGlowIndex));
        clr playerGlowColor;
        if (this->isDoingObjective()) { // Defusing or grabbing, different colour (DEF GREEN)
            playerGlowColor.r = 0.0f;
            playerGlowColor.g = 125.0f;
            playerGlowColor.b = 0.0f;
            playerGlowColor.a = 10.0f;
        } else if (Memory->read<int>(this->playerAddress + offsets::m_iTeamNum) == ourTeam) { // Teammate (DEF BLUE)
            playerGlowColor.r = 0.0f;
            playerGlowColor.g = 1.0f;
            playerGlowColor.b = 255.0f;
            playerGlowColor.a = 0.25f;
        } else { // Enemy (DEF PURPLE/PINK)
            playerGlowColor.r = 255.0f;
            playerGlowColor.g = 0.0f;
            playerGlowColor.b = 255.0f;
            playerGlowColor.a = 0.3f;
        }
        Memory->write<clr>(glowBase + 0x8, playerGlowColor);
        Memory->write<bool>(glowBase + 0x28, true); // Render when occluded
        Memory->write<bool>(glowBase + 0x29, false); // Render when unoccluded
    }
};
#endif /* helper_h */
