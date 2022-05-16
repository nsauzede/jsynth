# jsynth

This project is a kind of TB303 synthetiser "emulator" written from scratch,
based on Jack and SDL.
It can convert RBS files to internal x0x file format for playing the 303
part of Rebirth338 songs.

## Simple "j" Midi Jack client
In order to use real USB/MIDI devices appearing only as "Alsa Midi",
one must install:
sudo apt install a2jmidid
And run:
a2jmidid -e &
To connect the Alsa midi output as Jack midi input

## WORLDE Panda MINI
Pad1-8: chan9 Freq: 48 45 49 51 36 38 42 46 (02-7f)
  97 bytes=3 : NOTE ON : chan=9 freq=48 velo=127 99 30 7f
  99 bytes=3 : NOTE ON : chan=9 freq=45 velo=127 99 2d 7f
 101 bytes=3 : NOTE ON : chan=9 freq=49 velo=127 99 31 7f
 103 bytes=3 : NOTE ON : chan=9 freq=51 velo=127 99 33 7f
 105 bytes=3 : NOTE ON : chan=9 freq=36 velo=127 99 24 7f
 107 bytes=3 : NOTE ON : chan=9 freq=38 velo=127 99 26 7f
 109 bytes=3 : NOTE ON : chan=9 freq=42 velo=127 99 2a 7f
 111 bytes=3 : NOTE ON : chan=9 freq=46 velo=127 99 2e 7f

pot1-4: chan0 2nd: 0e 0f 10 11 (00-7f)
  83 bytes=3 : Control Change: chan=0 b0 0e 1b
  84 bytes=3 : Control Change: chan=0 b0 0f 22
  91 bytes=3 : Control Change: chan=0 b0 10 77
  92 bytes=3 : Control Change: chan=0 b0 11 02

slider1-4: chan0 2nd: 03 04 05 06 (00-7f)
 119 bytes=3 : Control Change: chan=0 b0 03 53
 124 bytes=3 : Control Change: chan=0 b0 04 34
 125 bytes=3 : Control Change: chan=0 b0 05 7d
 126 bytes=3 : Control Change: chan=0 b0 06 3d


## jack-keyboard
Notes:
   0 bytes=3 : NOTE ON : chan=0 freq=61 velo=64 90 3d 40
   1 bytes=3 : NOTE OFF: chan=0 freq=61 velo=64 80 3d 40
   2 bytes=3 : NOTE ON : chan=0 freq=62 velo=64 90 3e 40
   3 bytes=3 : NOTE OFF: chan=0 freq=62 velo=64 80 3e 40

Mod:
  91 bytes=3 : Control Change: chan=0 Modulation Wheel or Lever value=11 b0 01 0b
  92 bytes=3 : Control Change: chan=0 Modulation Wheel or Lever value=14 b0 01 0e
  93 bytes=3 : Control Change: chan=0 Modulation Wheel or Lever value=16 b0 01 10

Pitch:
  19 bytes=3 : Pitch Bend Change: chan=0 value=-2 e0 0a 3e
  20 bytes=3 : Pitch Bend Change: chan=0 value=-4 e0 66 3c
  21 bytes=3 : Pitch Bend Change: chan=0 value=-7 e0 42 3b
