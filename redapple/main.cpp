#include <mach/mach_traps.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach.h>
#include <mach-o/dyld_images.h>
#include <mach-o/loader.h>
#include <libproc.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <Carbon/Carbon.h>
#include "helper.h"
#include "CONFIG.h"
player ourPlayer;
int main(int argc, const char * argv[]) {
    srand(time(NULL));
    if (getuid() != 0) {
        printf("Yikes, Gotta run as root; We can't use task_for_pid() without being root. Try: \n\n\e[0;34msudo %s\e[0m\n\n", argv[0]);
        exit(255);
    }
    memHelper->initCSGOPid();
    memHelper->getCSGOTask();
    memHelper->getClient();
    memHelper->getEngine();
    memHelper->setupGlowManager();
    ourPlayer.initializeLocal();
    if (espEnabled) {
        std::thread espThread(esp);
        espThread.detach();
    }
    if (triggerEnable) {
        std::thread triggerThread(trigger);
        triggerThread.detach();
    }
    if (bhopEnable) {
        std::thread bhopThread(bhop);
        bhopThread.detach();
    }
    while (true) {
        std::cout << "\033]0;" << randomString() << "\007";
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        if (isPressed(FOVSWITCH_KEY)) {
            fov = !fov;
            if (fov) Memory->write(ourPlayer.playerAddress + iDefaultFOV, CUSTOMFOV); // CUSTOM FOV
            else Memory->write(ourPlayer.playerAddress + iDefaultFOV, DEFAULTFOV); // DEFAULT FOV
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        if (isPressed(FORCEQUIT_KEY_1) && isPressed(FORCEQUIT_KEY_2)) exit(128);
        ourPlayer.initializeLocal();
    }
    return 0;
}
void esp() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
        if (ourPlayer.isLivePlayer()) {
            for (int i = 1; i <= 64; i++) {
                player espPlayer;
                espPlayer.initialize(i);
                espPlayer.glowOutline(ourPlayer.team());
            }
        }
    }
}
void trigger() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
        if (isPressed(TRIGGERBOT_KEY) && ourPlayer.isLivePlayer()) {
            int xhair = Memory->read<int>(ourPlayer.playerAddress + crosshairid);
            player xhairPlayer;
            if (xhair <= 64 && xhair > 0) {
                xhairPlayer.initializeXhair(xhair - 1); // Some reason 0x10 didn't work. 0x20 did
                if (xhairPlayer.team() != ourPlayer.team() && xhairPlayer.isLivePlayer()) click();
            }
        }
    }
}
void bhop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
        if (ourPlayer.isLivePlayer() && isPressed(BHOP_KEY) && ourPlayer.flags() & PL_INAIR) pressVKey(kVK_Space);
    }
}
std::string randomString() {
    int alphaLength = sizeof(alphanum) - 1;
    std::string a;
    for (int z = 0; z < rand(); z++) a += alphanum[rand() % alphaLength];
    return a;
}
void errorExit(int code, const char * exitData) {
    std::cout << "Error: " << code << exitData << std::endl;
    sleep(5);
    exit(code);
}
bool isPressed(unsigned short inKeyCode) {
    unsigned char keyMap[16];
    GetKeys((BigEndianUInt32*)&keyMap);
    return (0 != ((keyMap[ inKeyCode >> 3] >> (inKeyCode & 7)) & 1));
}
void pressVKey(unsigned short key) {
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventRef keyUp;
    CGEventRef keyDown;
    keyUp = CGEventCreateKeyboardEvent(src, key, false);
    keyDown = CGEventCreateKeyboardEvent(src, key, true);
    CGEventPost(kCGHIDEventTap, keyDown);
    usleep(9);
    CGEventPost(kCGHIDEventTap, keyUp);
    CFRelease(keyUp);
    CFRelease(keyDown);
    CFRelease(src);
}
void click() {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%(MAX_TRIGGERBOT_SPEED - MIN_TRIGGERBOT_SPEED + 1) + MIN_TRIGGERBOT_SPEED));
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    CGEventRef click_down = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, cursor, kCGMouseButtonLeft);
    CGEventRef click_up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, cursor, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, click_down);
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%(MAX_TRIGGERBOT_HOLD - MIN_TRIGGERBOT_HOLD + 1) + MIN_TRIGGERBOT_HOLD));
    CGEventPost(kCGHIDEventTap, click_up);
}
