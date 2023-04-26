# synth3dmouse

Maps buttons of the "SpacePilot" 3D mouse to normal keyboard keys under linux/wayland.

Dependencies: 

sudo apt install libspnav-dev libevdev-dev #or something like it

Compile:

gcc synth3dmouse.c -lspnav -levdev  -I/usr/include/libevdev-1.0/ -L/usr/local/lib -o synth3dmouse.bin

Run:

./synth3dmouse.bin
