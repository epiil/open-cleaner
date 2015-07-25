#Design Narrative

##Summary

Opencleaner consists of a dual-motor spindle drivetrain that moves videotape through two Pellon cleaning stations by way of a supply and takeup reel, controlled by an Arduino Uno microcontroller and integrated motor shield. The tape's speed and tension are monitored by an array of IR sensors, photointerrupters and potentiometers that provide feedback to the microcontroller for motor speed control. Each Pellon cleaning station consists of individual drivetrains that incrementally advance fabric past several contact points adjacent to the tape path. 

##Spindles

The cleaner consists of supply and takeup spindles designed to accomodate 1/2" and 1/4" open reel video and audio tape. 

##IR tape sensors

IR tape sensors are used to determine the precise amount of tape left on each hub, sending input information to the Arduino, which would in turn adjust the DC takeup and supply motor speeds accordingly. These IR sensors in particular measure distance, and are pointed at specific locations on each takeup and supply hub. 

##Photointerrupters

Photointerrupters are placed on both the supply and takeup sides of the tape cleaner. Their purpose is to detect the presence of tape during operation. If tape is not detected, the photointerrupters will send input information to the Arduino and seize all motor operations. 

##Multi-format spindles

The main spindle on the tape cleaner accomodates 1/2" open reel video and 1/4" open reel audio formats. Custom spindles can be created to sit on top of the main cleaner spindles to accomodate other video formats like 3/4" UMatic (see above). Since 3/4" UMatic in particular is a cassette-driven format. 

