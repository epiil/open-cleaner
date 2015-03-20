###open-cleaner: an (in-progress) open-source tape cleaning solution for analog video / magnetic media

###Introduction

This repo contains instructions for assembling, testing, and operating an open-reel magnetic video tape cleaner. When used in conjunction with other conservation treatments like incubation, tape cleaning assists in minimizing the effects of tape degradation, such as binder hydrolisis or "Sticky Shed Syndrome" during the playback/reformatting process.  

The fundamental mechanics of the tape cleaner consist of a dual-motor spindle drivetrain that moves the tape through two Pellon cleaning stations by way of a supply and takeup reel, controlled by an Arduino Uno microcontroller and integrated motor shield. The tape's speed and tension are monitored by a series of IR sensors, photoresistors and potentiometers that provide feedback to the microcontroller boards and motors. Each Pellon cleaning uses individual drivetrains that incrementally advance fabric past several contact points. 

While the primary motor spindles of the tape cleaner may be used for open-reel 1/2" video and 1/4" audio tape, additional spindles may be employed for cassette-less cleaning of 3/4" U-Matic and VHS tape. 


###Repository Contents:

- /hardware - contains .scad, .dxf and .stl files for enclosure and spindle fabrication. All 2D/3D modeling is done with OpenSCAD (http://www.openscad.org/). 

- /software - contains .ino Arduino "sketches" and associated libraries (http://www.arduino.cc/)

- /schematics - contains electronic schematics done with Fritzing (http://fritzing.org/home/)

- /partslist - contains inventory and pricelist of associated parts


