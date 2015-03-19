#open-cleaner: An (in-progress) open-source tape cleaning solution for analog video / magnetic media

###Introduction

This repo contains instructions for building, testing, and operating a device designed for cleaning open-reel magnetic video and audio tape. When used in conjunction with audiovisual reformatting, the device will assist in the mitigation of binder hydrolsis (aka "Sticky Shed Syndrome"). 

The fundamental mechanics of the device consist of a dual-motor spindle drivetrain that moves tape through two Pellon cleaning stations by way of a supply and takeup reel, controlled by an Arduino Uno microcontroller and accompanying motor shield. Tape speed and tension are monitored by a series of IR sensors, photoresistors and potentiometers, which provide feedback to the Arduino and motor shield. Each Pellon cleaning station has an individual drivetrain that advances cleaning fabric. 

While the primary motor spindles of the tape cleaner may be used for 1/2" and 1/4" open-reel video and audio tape, additional spindles may be used for the cassette-less cleaning of 3/4" U-Matic and VHS formats.


###Repository Contents:

- /hardware - contains .scad, .dxf and .stl files for enclosure and spindle fabrication. All 2D/3D modeling is done with OpenSCAD (http://www.openscad.org/). All circuit schematics are done with Fritzing (http://fritzing.org/home/)

- /software - contains .ino Arduino "sketches" and associated libraries (http://www.arduino.cc/)





