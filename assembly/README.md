# Assembly Instructions - Motor Shield and LCD Keypad

## Motor Shield

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_001.JPG " ")

Instructions for assembling the Adafruit Motor Shield (v2) can be found [here](https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/install-headers). 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_002.JPG " ")

The shield comes with a set of standard headers that need to be soldered. This can be done by putting the headers in the Arduino and placing the motor shield on top of the header-ed board. 

## LCD Keypad

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_003.JPG " ")

Instructions for assembling the Adafruit LCD Keypad can be found [here](https://learn.adafruit.com/adafruit-16x2-character-lcd-plus-keypad-for-raspberry-pi/assembly).

## Motor shield modification - LCD Keypad connection

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_004.JPG " ")

First, solder the bridge to create the 0x61 / binary 0001 address.

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_005.JPG " ")

Connect the 5V pin to the area on the perma-proto section similar to the position above. Leave around an inch of hook-up wire to connect to the bottom of the board.

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_006.JPG " ")

Connect the Ground ("GND") pin to the area on the perma-proto section similar to the position above. Leave around an inch of hook-up wire to connect to the bottom of the board. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_007.JPG " ")

Solder a 2.54mm 4-pin terminal block to the perma-proto section similar to the position above.

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_009.JPG " ")

Using the ends of the 5V and Ground wires, connect the ends to Pins 1 and 4 of the 4-pin terminal block. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_008.JPG " ") 

Connect the Serial Data Line (SDA) pin to the area on the perma-proto section similar to the position above (either Pin 2 or Pin 3 of the 4-pin terminal block). Leave around a half-inch of hook-up wire to connect to the bottom of the board.

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_010.JPG " ")

Connect the Serial Clock Line (SCL) pin to the area on the perma-proto section similar to the position above (either Pin 2 or Pin 3 of the 4-pin terminal block). Leave around a half-inch of hook-up wire to connect to the bottom of the board.

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_011.JPG " ")

Connect the ends of SDA and SCL wires to Pins 2 and 3 of the 4-pin terminal block. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_012.JPG " ")

Cut four equal lengths of hook-up wire, RED for VIN, Black for GROUND, White for SDA, Blue for SCL. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_013.JPG " ")

Twist the lengths together to form one bundle of wire. Strip about 1/4" worth of insulation off of each wire's end. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_014.JPG " ")

On one side of the bundled cable, connect each RED, BLACK, WHITE and BLUE wire to each of the associated terminals on the 4-pin terminal block. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_015.JPG " ")

On the other side of the bundled cable, connect each RED, BLACK, WHITE and BLUE wire connect the other end to the associated header slots of the LCD Keypad. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/MotorShieldLCDAssembly/MotorShieldLCDAssembly_016.JPG " ")

The motor shield and LCD/keypad connected can now be tested using the latest Opencleaner sketch. 

## Motor shield modification - Photointerrupters and pot connections

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_001.JPG " ")

Solder three (3) 2.54mm 3-pin terminal blocks to the perma-proto section similar to the positions above.

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_002.JPG " ")

Using the previously soldered 5V and Ground connections, solder 5V and Ground to Pins 1 and 3 of each 3-pin terminal block. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_004.JPG " ")

Cut two lengths of white hook-up wire and one length of red hook-up wire as shown. Strip about 1/2" worth of insulation off of each wire's end.  

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_003.JPG " ")

Curve each wire as shown. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_006.JPG " ")

Using this wire, make the following connections:

- Connect DIGITAL I/O 3 to Pin 2 of the first 3-pin terminal block. 
- Connect DIGITAL I/O 2 to Pin 2 of the second 3-pin terminal block. 
- Connect ANALOG I/O 3 to Pin 2 of the third 3-pin terminal block. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_005.JPG " ")

On the bottom of the board, solder the connections accordingly. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_007.JPG " ")

Top-view of DIGITAL I/O 2 and DIGITAL I/O 2 connections. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_008.JPG " ")

Side-view of DIGITAL I/O 2 and DIGITAL I/O 2 connections. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_009.JPG " ")

Side-view of ANALOG I/O 3 connection. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_010.JPG " ")

Top-view of DIGITAL I/O 2 and DIGITAL I/O 2 terminal block connections. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_011.JPG " ")

Top view of motor shield with completed connections. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_012.JPG " ")

Cut three equal lengths of hook-up wire (ca. 2 feet long), RED for PWR, Black for GROUND, and White for SIG. Twist the lengths together to form one bundle of wire. Cut in half to make two separate bundles. Strip about 1/4" worth of insulation off of each wire's end. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_013.JPG " ")

Solder one end of each bundle to the photointerrupter as shown. 

![alt text](https://github.com/epiil/open-cleaner/blob/master/photos/photos/PhotointerrupterPotAssembly/PhotointerrupterPotAssembly_014.JPG " ")

Connect the other end of each bundle to the first and second 3-pin terminal blocks as shown.  


