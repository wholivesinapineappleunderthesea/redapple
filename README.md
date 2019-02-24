# redapple
Garbage CS:GO cheat for MacOS

Look in CONFIG.h for configuration

# Building

You can do this in the terminal or Xcode.

Run xcodebuild in the terminal (in the redapple directory). Then the executable will be in build/Release/.

Or in Xcode run it, then drag the redapple executable into your root shell.

# Why root

Apple, in all their secure glory disabled task_for_pid() if you are not root or do not have a developer license... So we need to be root. 

You might run into issues with task_for_pid(), you may need to disable SIP (rootless) via csrutil in recovery or mess around with Clover.

# Credits
Major credit to: https://github.com/iseekwonderful/csgoGlow/blob/master/Wall/memory/process.cpp

Which is written under: https://www.gnu.org/licenses/agpl-3.0.html

Also thanks to: https://github.com/aKalisch/gosx-public-external
