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
#include <sstream>
#include "helper.h"
#include "CONFIG.h"

player ourPlayer;
int ourTeam, ourHealth;
bool ourIsLegit;
int main(int argc, char * argv[]) {
    srand(static_cast<unsigned int>(time(NULL)));
    if (getuid() != 0) errorExit(255, " Yikes, Gotta run as root; We can't use task_for_pid() without being root.");
    memHelper->setup();
    printf("0x%jx \n", clientModule);
    ourPlayer.initializeLocal();
#ifdef espEnabled
    std::thread espThread(esp);
    espThread.detach();
#endif
#ifdef triggerEnable
    std::thread triggerThread(trigger);
    triggerThread.detach();
#endif
#ifdef bhopEnable
    std::thread bhopThread(bhop);
    bhopThread.detach();
#endif
    while (true) {
        ourTeam = Memory->read<int>(ourPlayer.playerAddress + m_iTeamNum);
        if (ourTeam == 2 || ourTeam == 3) ourIsLegit = true; else ourIsLegit = false;
        std::cout << "\033]0;" << randomString() << "\007"; // Change console title (Hide from VAC)
        if (isPressed(FORCEQUIT_KEY)) exit(128);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
void esp() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    player espPlayer;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(14));
        if (ourIsLegit) {
            for (int i = 1; i <= MAX_PLAYER_LIST; i++) {
                espPlayer.initialize(i);
                espPlayer.glowOutline(ourTeam);
            }
        }
    }
}
void trigger() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
        if (isPressed(TRIGGERBOT_KEY) && ourIsLegit) {
            int xhair = Memory->read<int>(ourPlayer.playerAddress + crosshairid);
            player xhairPlayer;
            if (xhair <= MAX_PLAYER_LIST && xhair > 0) {
                xhairPlayer.initializeXhair(xhair - 1); // Some reason 0x10 didn't work. 0x20 did
                if (Memory->read<int>(xhairPlayer.playerAddress + m_iTeamNum) != ourTeam) click();
            }
        }
    }
}
void bhop() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(12)); // Turn this down if you want more consistant hops
        if (ourIsLegit && isPressed(BHOP_KEY)) if (Memory->read<int>(ourPlayer.playerAddress + m_fFlags) & PL_INAIR) pressVKey(kVK_Space);
    }
}
std::string randomString() {
    int alphaLength = sizeof(alphanum) - 1;
    std::string a;
    for (int z = 0; z < rand(); z++) a += alphanum[rand() % alphaLength];
    return a;
}
void errorExit(int code, std::string exitData) {
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
