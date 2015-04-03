/******************************************************************************
 * Open-cleaner
 * 
 * 
 * 
 * 
 ******************************************************************************/

#include <SoftwareSerial.h>

/******************************************************************************
 * PhotoInterrupter Definitions
 ******************************************************************************/

int supplyIntPin = 2; // select analog 2 input pin for supply-side photointerrupter
int supplyIntVal; // variable to store the value coming from supply-side photointerrupter
//int takeupIntPin = 4; // select analog 4 input pin for take-up side photointerrupter
//int takeupIntVal; // variable to store the value coming from take-up side photointerrupter

/******************************************************************************
 * IR Sensor Definitions
 ******************************************************************************/

//#define supplyirSensorPin 5       // supply-side IR sensor on this pin
//#define takeupirSensorPin 6       // take-up IR sensor on this pin
//int irRead(int readPin, int triggerPin); //function prototype

/******************************************************************************
 * LCD Definitions
 ******************************************************************************/

#define txPin 2 //Rx pin on SerLCD is connected to Digital pin 2

SoftwareSerial LCD = SoftwareSerial(0, txPin);
// since the LCD does not send data back to the Arduino, we should only define the txPin
const int LCDdelay=10;  // conservative, 2 actually works

// wbp: goto with row & column
void lcdPosition(int row, int col) {
  LCD.write(0xFE);   //command flag
  LCD.write((col + row*64 + 128));    //position 
  delay(LCDdelay);
}
void clearLCD(){
  LCD.write(0xFE);   //command flag
  LCD.write(0x01);   //clear command.
  delay(LCDdelay);
}
void backlightOn() {  //turns on the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(157);    //light level.
  delay(LCDdelay);
}
void backlightOff(){  //turns off the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(128);     //light level for off.
   delay(LCDdelay);
}
void serCommand(){   //a general function to call the command flag for issuing all other commands   
  LCD.write(0xFE);
}

void setup() 
{
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  clearLCD();
  lcdPosition(0,0);
  LCD.print("   OPENCLEANER");
  
/******************************************************************************
 * DC Motor Definitions
 ******************************************************************************/
  
  pinMode(12, OUTPUT); //Initiates Motor Channel A (supply-side motor) pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A (supply-side motor) pin
  pinMode(13, OUTPUT); //Initiates Motor Channel B (take-up side motor) pin
  pinMode(8, OUTPUT); //Initiates Brake Channel B (take-up side motor) pin
}

void loop() 
{
  supplyIntVal = analogRead(supplyIntPin); // read the value from photointerrupter
  Serial.println(supplyIntVal); // print the sensor value to the serial monitor
  delay(50);
  if (supplyIntVal > 10)
{
	
  digitalWrite(13, LOW); //Stops Channel A (supply-side motor)
  digitalWrite(8, HIGH);   //Engages the Brake for Channel A (supply-side motor)
  digitalWrite(12, LOW); //Stops Channel B (take-up side motor)
  digitalWrite(9, HIGH);   //Disengages the Brake for Channel B (take-up side motor)
}
  else
{
  digitalWrite(13, HIGH); //Establishes forward direction of Channel A (supply-side motor)
  digitalWrite(8, LOW);   //Disengages the Brake for Channel A (supply-side motor)
  analogWrite(3, 80);   //Spins the motor on Channel A (supply-side motor) at full speed
  digitalWrite(12, HIGH); //Establishes backward direction of Channel B (take-up side motor)
  digitalWrite(9, LOW);   //Disengage the Brake for Channel B (take-up side motor)
  analogWrite(11, 200);   //Spins the motor on Channel B (take-up side motor) at half speed
}
}
 
