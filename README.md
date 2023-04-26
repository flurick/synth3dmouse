# synth3dmouse

Maps buttons of the "SpacePilot" 3D mouse to normal keyboard keys under linux/wayland.
(Some keys are missing here due to lack of hardware but it should be easy to use xev and the linux keycode list linked in .c file and make changes for other models and personal preference.)

Dependencies: 

sudo apt install libspnav-dev libevdev-dev #or something like it

Compile:

gcc synth3dmouse.c -lspnav -levdev  -I/usr/include/libevdev-1.0/ -L/usr/local/lib -o synth3dmouse.bin

Run:

./synth3dmouse.bin
