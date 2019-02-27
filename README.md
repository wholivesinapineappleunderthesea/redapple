# redapple
Garbage CS:GO cheat for MacOS

Look in CONFIG.h for configuration

# Building

Open up your favourite terminal, cd to the downloaded project. Then type:

>make --always-make

You may need binutils from brew.

Then in the folder, there will be an executable with a random name (numbers), run it as root or with sudo.

# Why root

Apple, in all their secure glory disabled task_for_pid() if you are not root or do not have a developer license... So we need to be root. 

You might run into issues with task_for_pid(), you may need to disable SIP (rootless) via csrutil in recovery or mess around with Clover.

# Credits
Major credit to: https://github.com/iseekwonderful/csgoGlow/blob/master/Wall/memory/process.cpp which is written under: https://www.gnu.org/licenses/agpl-3.0.html

Also thanks to: https://github.com/aKalisch/gosx-public-external
