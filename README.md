#opencleaner: an open-source tape cleaning solution for magnetic media

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/DSC06149.jpg " ")

##Introduction

This repo contains instructions for assembling, testing, and operating an open-reel 1/2" magnetic videotape cleaner. When used in conjunction with other remediation treatments like incubation, tape cleaning assists in minimizing the effects of tape degradation such as binder hydrolysis ("sticky shed syndrome") during the playback/reformatting process.  

Controlled by an Arduino Uno microcontroller and integrated motor shield, the machine consists of a dual-motor spindle system that moves tape through two Pellon cleaning stations. Tape speed and tension are monitored through feedback from optical encoders attached to each motor's shaft. Each Pellon cleaning station incrementally advances fabric past several contact points.  

In-progress videos:

[02/15/2017 - 24VDC Motor Test](https://vimeo.com/204296759)
[02/23/2017 - PID Control Test](https://vimeo.com/205490774)

##Repository Contents:

- /hardware - contains .dwg, .pdf, .stp and .stl files for enclosure and spindle fabrication.

- /software - contains .ino Arduino sketches (http://www.arduino.cc/).

- /schematics - contains .sch and .pdf schematic files for individual components.

- /photos - photo gallery

##Acknowledgements

This project could not have been possible without the generous contributions from the following people and organizations:

Tim Woolaver

Mike Maxwell

Maurice Schechter

Video Data Bank - Tom Colley, Kristin McDonough

Smithsonian NMAAHC - Walter Forsberg

Signal Culture - Deborah Bernagozzi, Jason Bernagozzi, Hank Rudolph






