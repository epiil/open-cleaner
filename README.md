#opencleaner: an (IN PROGRESS!!) open-source tape cleaning solution for magnetic media

##Introduction

This repo contains instructions for assembling, testing, and operating an open-reel magnetic video tape cleaner. When used in conjunction with other conservation treatments like incubation (baking), tape cleaning assists in minimizing the effects of tape degradation, such as binder hydrolysis or "sticky shed syndrome" during the playback/reformatting process.  

Opencleaner consists of a dual-motor spindle drivetrain that moves the tape through two Pellon cleaning stations by way of a supply and takeup reel, controlled by an Arduino Uno microcontroller and integrated Adafruit motor shield. The tape's speed and tension are monitored by a series of IR sensors, photoresistors and potentiometers that provide feedback to the microcontroller boards and motors. Each Pellon cleaning station uses individual drivetrains that incrementally advance fabric past several contact points. While the primary motor spindles of the tape cleaner may be used for open-reel 1/2" video and 1/4" audio tape, additional spindles are planned for cassette-less cleaning of 3/4" U-Matic and VHS tape. 


##Repository Contents:

- /hardware - contains .scad, .dxf and .stl files for enclosure and spindle fabrication. All 2D/3D modeling is done with OpenSCAD (http://www.openscad.org/). Note that associated .stl 3D files are viewable on Github. 

- /software - contains .ino Arduino "sketches" and associated libraries (http://www.arduino.cc/).

- /schematics - contains .sch schematic files done in Eagle (http://www.cadsoftusa.com/).

- /partslist - contains inventory and pricelist of associated parts, updared regularly. 

- /photos - in-progress photos

