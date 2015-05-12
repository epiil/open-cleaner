#open-cleaner: an (in-progress) open-source tape cleaning solution for analog video / magnetic media

##Introduction

This repo contains instructions for assembling, testing, and operating an open-reel magnetic video tape cleaner. When used in conjunction with other conservation treatments like incubation (baking), tape cleaning assists in minimizing the effects of tape degradation, such as binder hydrolysis or "sticky shed syndrome" during the playback/reformatting process.  

The tape cleaner consists of a dual-motor spindle drivetrain that moves the tape through two Pellon cleaning stations by way of a supply and takeup reel, controlled by an Arduino Uno microcontroller and integrated motor shield. The tape's speed and tension are monitored by a series of IR sensors, photoresistors and potentiometers that provide feedback to the microcontroller boards and motors. Each Pellon cleaning station uses individual drivetrains that incrementally advance fabric past several contact points. 

##Application

While the primary motor spindles of the tape cleaner may be used for open-reel 1/2" video and 1/4" audio tape, additional spindles may be employed for cassette-less cleaning of 3/4" U-Matic and VHS tape. 


##Repository Contents:

- /hardware - contains .scad, .dxf and .stl files for enclosure and spindle fabrication. All 2D/3D modeling is done with OpenSCAD (http://www.openscad.org/). Note that .stl 3D files are viewable on Github. 

- /software - contains .ino Arduino "sketches" and associated libraries (http://www.arduino.cc/).

- /schematics - contains .sch schematic files done in Eagle (http://www.cadsoftusa.com/).

- /partslist - contains inventory and pricelist of associated parts. 


##License

This repo is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details. You should have received a copy of the GNU General Public License along with this program. If not, see
<http://www.gnu.org/licenses/>.
