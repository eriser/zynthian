#/bin/bash

#Start Jack Audio RT System
#jackd -dalsa -dhw:0 -r48000 -p512 -n4 -P &

#Start Synths
#timidity -a -iA -Os -B1,8
#fluidsynth /usr/share/sounds/sf2/FluidR3_GM.sf2
#fluidsynth -a alsa -g 1 /usr/share/sounds/sf2/FluidR3_GM.sf2 ./sf2/KeppysSteinwayPiano2.8.sf2
#zynaddsubfx --auto-connect --no-gui
#/home/pi/Downloads/zynaddsubfx-2.5.0/build/src/zynaddsubfx -O alsa -I alsa -U
./software/zynaddsubfx/build/src/zynaddsubfx -O alsa -I alsa -U
