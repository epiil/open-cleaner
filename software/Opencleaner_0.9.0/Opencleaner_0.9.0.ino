/* 
Opencleaner Arduino Sketch
Version 0.9.0 (beta)
Revised 23 July 2015
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Adafruit_RGBLCDShield.h>

//---------------------------------- Motor shield instantiation -------------------------------------------

// Creates motor shield object with alternate 0x61 I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0X61); 

// Selects ports for spindle and pellon motors
Adafruit_DCMotor *spindlesupplyMotor = AFMS.getMotor(1);
Adafruit_DCMotor *spindletakeupMotor = AFMS.getMotor(2);
Adafruit_DCMotor *pellonsupplyMotor = AFMS.getMotor(3);
Adafruit_DCMotor *pellontakeupMotor = AFMS.getMotor(4);

//---------------------------------- LCD shield instantiation -------------------------------------------

// Creates LCD shield object
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//---------------------------------- Pin definitions -------------------------------------------

#define LED 13

//------------------------------------- Variables -------------------------------------------

// Character buffer for serial input/serial monitor
boolean LED_STATE = false;
char COMMAND[30];               

// Motor variables
int spindlesupplyMotorSpeed = 0;
int spindletakeupMotorSpeed = 0;
int pellonsupplyMotorSpeed = 0;
int pellontakeupMotorSpeed = 0;

//--------------------------------- General functions -------------------------------------------

// Toggles arduino on-board LED for easy debugging
void toggle_LED(void)  

{
  LED_STATE = !LED_STATE;
  digitalWrite(LED, LED_STATE);
}
//-------------------------------- Serial Port Support ---------------------------------------

void CLEAR_SERIAL_BUFFER(void)
{
 int junk = 0;
  while (Serial.available())
  {
  junk = Serial.read();
  }
}

void SHOW_MOTOR_COMMAND_CHOICES(void)
{
  Serial.println("1 Run spindlesupplyMotor forward at current speed");
  Serial.println("2 Run spindlesupplyMotor reverse at current speed");
  Serial.println("3 Brake spindlesupplyMotor");
  Serial.println("4 Stop spindlesupplyMotor");
  Serial.println("5 SetspindlesupplyMotor Speed");
  
  Serial.println("6 Run spindletakeupMotor forward at current speed");
  Serial.println("7 Run spindletakeupMotor reverse at current speed");
  Serial.println("8 Brake spindletakeupMotor");
  Serial.println("9 Stop spindletakeupMotor");
  Serial.println("A Set spindletakeupMotor Speed");

  Serial.println("B Run pellonsupplyMotor forward at current speed");
  Serial.println("C Run pellonsupplyMotor reverse at current speed");
  Serial.println("D Brake pellonsupplyMotor");
  Serial.println("E Stop pellonsupplyMotor");
  Serial.println("F Set pellonsupplyMotor Speed");

  Serial.println("G Run pellontakeupMotor forward at current speed");
  Serial.println("H Run pellontakeupMotor reverse at current speed");
  Serial.println("I Brake pellontakeupMotor");
  Serial.println("J Stop pellontakeupMotor");
  Serial.println("K Set pellontakeupMotor Speed");
}

//-------------------------------- Motor functions -------------------------------------------
// Only two motor-specific functions to call:

// motorname->run(COMMAND)
// where COMMAND is
// FORWARD            Run Forward
// BACKWARD           Run Reverse
// BRAKE              Short the windings together.  Stops the motor as fast as possible.
// RELEASE            Both windings in High impedance state. Free wheeling motor like it is disconnected.

// motorname->setSpeed(SPEED)
// where SPEED is 0 to 255 with 0 as the slowest

void setup() 
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("OPENCLEANER V0.9.0");
  
//---------------------------------- Motor Setup --------------------------------------
  
  // Sets default frequency at 1.6KHz
  AFMS.begin();  
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Default Speeds (OFF)
  spindlesupplyMotor->setSpeed(150);
  spindletakeupMotor->setSpeed(150);
  pellonsupplyMotor->setSpeed(150);
  pellontakeupMotor->setSpeed(150); 
  // Turn all motor off.  Free wheeling
  spindlesupplyMotor->run(RELEASE);
  spindletakeupMotor->run(RELEASE);
  pellonsupplyMotor->run(RELEASE);
  pellontakeupMotor->run(RELEASE);
  SHOW_MOTOR_COMMAND_CHOICES();
  
//---------------------------------- LCD Setup --------------------------------------
  
// Set up LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  lcd.print("OPENCLEANER V0.9.0");
}

void loop() 
{
if (Serial.available() > 0) 
  {
  Serial.readBytesUntil('\n',COMMAND,20);// 
  char val = COMMAND[0];
  CLEAR_SERIAL_BUFFER();
  
// ------------------------------------ spindlesupplyMotor  ----------------------------------
  
  if (val == '1')// spindlesupplyMotor
    {
    spindlesupplyMotor->run(FORWARD);
     val = 'x';
    }
  else if (val == '2')// spindlesupplyMotor
    {
    spindlesupplyMotor->run(BACKWARD);
     val = 'x';
    }
  else if (val == '3')// spindlesupplyMotor
    {
    spindlesupplyMotor->run(BRAKE);
     val = 'x';
    }
  else if (val == '4')// spindlesupplyMotor
    {
    spindlesupplyMotor->run(RELEASE);
     val = 'x';
    }
  else if (val == '5')// spindlesupplyMotor
    {
    Serial.println("Enter speed (0-255)");
    while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
      delay(10);
      spindlesupplyMotorSpeed  = Serial.parseInt();
      spindlesupplyMotor->setSpeed(spindlesupplyMotorSpeed);
      CLEAR_SERIAL_BUFFER();
      SHOW_MOTOR_COMMAND_CHOICES();
      val = 'x';
    }
	
// ------------------------------------ spindletakeupMotor  ----------------------------------
	
 if (val == '6')// spindletakeupMotor
    {
    spindletakeupMotor->run(FORWARD);
     val = 'x';
    }
  else if (val == '7')// spindletakeupMotor
    {
    spindletakeupMotor->run(BACKWARD);
     val = 'x';
    }
  else if (val == '8')// spindletakeupMotor
    {
    spindletakeupMotor->run(BRAKE);
     val = 'x';
    }
  else if (val == '9')// spindletakeupMotor
    {
    spindletakeupMotor->run(RELEASE);
     val = 'x';
    }
  else if (val == 'A')// spindletakeupMotor
    {
    Serial.println("Enter speed (0-255)");
    while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
      delay(10);
      spindletakeupMotorSpeed  = Serial.parseInt();
      spindletakeupMotor->setSpeed(spindletakeupMotorSpeed);
      CLEAR_SERIAL_BUFFER();
      SHOW_MOTOR_COMMAND_CHOICES();
       val = 'x';
    }
	
 // ------------------------------------ pellonsupplyMotor  ----------------------------------
	
 if (val == 'B')// pellonsupplyMotor
    {
    pellonsupplyMotor->run(FORWARD);
    }
  else if (val == 'C')// pellonsupplyMotor
    {
    pellonsupplyMotor->run(BACKWARD);
     val = 'x';
    }
  else if (val == 'D')// pellonsupplyMotor
    {
    pellonsupplyMotor->run(BRAKE);
     val = 'x';
    }
  else if (val == 'E')// pellonsupplyMotor
    {
    pellonsupplyMotor->run(RELEASE);
    val = 'x';
    }
  else if (val == 'F')// pellonsupplyMotor
    {
    Serial.println("Enter speed (0-255)");
    while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
      delay(10);
      pellonsupplyMotorSpeed  = Serial.parseInt();
      pellonsupplyMotor->setSpeed(pellonsupplyMotorSpeed);
      CLEAR_SERIAL_BUFFER();
      SHOW_MOTOR_COMMAND_CHOICES();
       val = 'x';
    }
	
// ------------------------------------ pellontakeupMotor  ----------------------------------
	
if (val == 'G')// pellontakeupMotor
    {
    pellontakeupMotor->run(FORWARD);
     val = 'x';
    }
  else if (val == 'H')// pellontakeupMotor
    {
    pellontakeupMotor->run(BACKWARD);
     val = 'x';
    }
  else if (val == 'I')// pellontakeupMotor
    {
    pellontakeupMotor->run(BRAKE);
     val = 'x';
    }
  else if (val == 'J')// pellontakeupMotor
    {
    pellontakeupMotor->run(RELEASE);
     val = 'x';
    }
  else if (val == 'K')// pellontakeupMotor
    {
    Serial.println("Enter speed (0-255)");
    while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
      delay(10);
      pellontakeupMotorSpeed  = Serial.parseInt();
      pellontakeupMotor->setSpeed(pellontakeupMotorSpeed);
      CLEAR_SERIAL_BUFFER();
      SHOW_MOTOR_COMMAND_CHOICES();
       val = 'x';
    }
    
  } //end if (Serial.available() > 0) 
}// end loop
