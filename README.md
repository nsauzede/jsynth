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
