/* 
CLOSED LOOP TEST PROGRAM
12/29/15
T. WOOLAVER
STARTED WITH Opencleaner Arduino Sketch Version 1.0.0.0 to write this
which was Revised 20 December 2015.
Lots of stuff commented out
No LCD Buttons

Other changes
Changed the baud rate of the serial port from 9600 to 115200.
Added a 100 msec timer to the main loop.
Added a timer unterrupt service routine that fires every 1 msec used to sample feedeback from servo loops.
https://github.com/epiil/open-cleaner
*/

// Libraries for the Adafruit LCD Keypad / Motor Shield

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <TimerOne.h>


// Motor shield instantiation 

// Creates motor shield object with alternate 0x61 I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0X61); 

// Selects ports for spindle and pellon motors
Adafruit_DCMotor *spindlesupplyMotor = AFMS.getMotor(2);
Adafruit_DCMotor *spindletakeupMotor = AFMS.getMotor(1);
Adafruit_DCMotor *pellonMotor1 = AFMS.getMotor(3);
Adafruit_DCMotor *pellonMotor2 = AFMS.getMotor(4);

// LCD shield instantiation

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

#define BUTTON_SHIFT BUTTON_SELECT

// Pin definitions

#define LED 13
#define TAPE_SPEED_OPTO 4
// Photointerrupter pins
#define photointerrupt1 = 3;
#define photointerrupt2 = 4;


// --------------------------------------- Variables -------------------------------------
//---- LCD module button variables -----
unsigned long lastInput = 0; // last button press
 
const int logInterval = 10000; // log every 10 seconds
long lastLogTime = 0;
//---- Motor Variables  -----
uint8_t DIR_RELEASE_BRAKE = BRAKE;  // Global Parameter used with the run command
                                    // These are the defines from the libeary
                                    // #define FORWARD 1
                                    // #define BACKWARD 2
                                    // #define BRAKE 3
                                    // #define RELEASE 4 


//Tape Speed Optical sensor

boolean TAPE_SPEED_OPTO_STATE = false;
boolean LAST_TAPE_SPEED_OPTO_STATE = false;         // Used for debouncing


// Tape speed control servo variables and constants

int TAPE_SPEED_COUNTER = 0;                           // msec.
int TAPE_SPEED_OPTO_DEBOUNCE_COUNTER = 0;             // msec.
const int TAPE_SPEED_OPTO_DEBOUNCE_COUNT = 1;
;         // msec.
int TAPE_SPEED_OPTO_DEBOUNCE_DELAY_COUNTER = 0;       // msec.
const int TAPE_SPEED_OPTO_DEBOUNCE_DELAY_COUNT = 0;   // msec.
boolean TAPE_SPEED_OPTO_DEBOUNCE_COMPLETE_FLAG_LOW = false;
int TAPE_SPEED_PERIOD = 0;
int SERIAL_STATUS_OUTPUT_UPDATE_COUNTER = 0;         // msec.
const int SERIAL_STATUS_OUTPUT_UPDATE_COUNT = 50;    // 1 sec..
float INPUT_PERIOD_FLOAT = 0.0;                      // Period of the desired speed input = 1/SPEED
int INPUT_PERIOD = 0;
int PERIOD_ERROR = 0;                                //       
float PERIOD_ERROR_FLOAT = 0.0;
int spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND = 30;   // Speed input tpo the closed loop speed
// -------------- These are the parameters to change for the proportional loop ----------------------
float SPEED_SERVO_GAIN_CONSTANT = 0.03;
int spindletakeupMotorSpeed_LOWER_LIMIT = 40;
int spindletakeupMotorSpeed_UPPER_LIMIT = 80;
//---------------------------------------------------------------------------------------------------
int SPEED_SERVO_PWM_COMMAND = 0;
boolean spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG = false;

//  Main Loop Timer variables and constants

unsigned long MAIN_LOOP_TIMER = 0;
const unsigned long   MAIN_LOOP_TIMER_INCREMENT = 50; //50 msec
unsigned long  CURRENT_TIME = 0;
 

// Character buffer for serial input/serial monitor

char COMMAND[30]; 
              
boolean LED_STATE = false;

// Motor variables
int spindlesupplyMotorSpeed = 0;
float spindlesupplyMotorSpeed_FLOAT = 0.0;
int spindletakeupMotorSpeed = 0;
float spindletakeupMotorSpeed_FLOAT = 0.0;
int pellonMotor1Speed = 0;
float pellonMotor1Speed_FLOAT = 0.0;
int pellonMotor2Speed = 0;
float pellonMotor2Speed_FLOAT = 0.0;

// General functions

// Toggles arduino on-board LED for easy debugging
void toggle_LED(void)  
{
  LED_STATE = !LED_STATE;
  digitalWrite(LED, LED_STATE);
}

void SHOW_MOTOR_COMMAND_CHOICES(void)
{
  Serial.println("1 Run spindlesupplyMotor FORWARD");
  Serial.println("2 Run spindlesupplyMotor BACKWARD");
  Serial.println("3 Brake spindlesupplyMotor");
  Serial.println("4 STOP spindlesupplyMotor");
  Serial.println("5 Set spindlesupplyMotor Speed");
  
  Serial.println("6 Run spindletakeupMotor forward at current speed");
  Serial.println("7 Run spindletakeupMotor reverse at current speed");
  Serial.println("8 Brake spindletakeupMotor");
  Serial.println("9 Stop spindletakeupMotor");
  Serial.println("A Set spindletakeupMotor Speed");

  Serial.println("B Run pellonMotor1 forward at current speed");
  Serial.println("C Stop pellonMotor1");
  Serial.println("D Set pellonMotor1 Speed");

  Serial.println("E Run pellonMotor2 forward at current speed");
  Serial.println("F Stop pellonMotor2");
  Serial.println("G Set pellonMotor2 Speed");
  Serial.println("H Read tensionRoller_POT_VOLTAGE");

  Serial.println("I Change Tape duration");
  Serial.println("J Change Direction");
  Serial.println("K Run");
  Serial.println("L Off");

  Serial.println("M Ramp spindlesupplyMotor 0-255");
  Serial.println("N Read Tape Speed Opto");
  Serial.println("O Run the spindletakeupMotor as speed");
}

// Serial Port Support

void CLEAR_SERIAL_BUFFER(void)
{
 int junk = 0;
  while (Serial.available())
  {
  junk = Serial.read();
  }
}

void TIMER_ISR(void)
{
// Timer interrupt service routine.
// Fires once every 1 msec. all the time.
// NOTE:  I tried updating the motor speed in side theis ISR.  It dies not work.  No motor shield functions
// can be called from inside any ISR since they must use interrupts for the I2C interface and interrupts are
// disabled when ISRs fire.  Consequently, the speed update is called in the timer loop inside the main loop
//-------------------------- Tape Speed Monitor ---------------------------
    toggle_LED();
    if(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG)
    {
      TAPE_SPEED_COUNTER++;
      TAPE_SPEED_OPTO_STATE  = digitalRead(TAPE_SPEED_OPTO);
      if (!TAPE_SPEED_OPTO_STATE && !LAST_TAPE_SPEED_OPTO_STATE  && !TAPE_SPEED_OPTO_DEBOUNCE_COMPLETE_FLAG_LOW)    //We need it to be tripped (low) 5 times tp make sure
      {                                                                                                              // it is not sounce.
        TAPE_SPEED_OPTO_DEBOUNCE_COUNTER++;
      }                                                           
      LAST_TAPE_SPEED_OPTO_STATE = TAPE_SPEED_OPTO_STATE; 
      if(TAPE_SPEED_OPTO_DEBOUNCE_COUNTER == TAPE_SPEED_OPTO_DEBOUNCE_COUNT)    //If we got 5 lows,  It's real.
      {
        TAPE_SPEED_OPTO_DEBOUNCE_COMPLETE_FLAG_LOW = true;
        if(TAPE_SPEED_OPTO_STATE)                                               // so we wait for it to go hich again
        {
          TAPE_SPEED_PERIOD = TAPE_SPEED_COUNTER;                               // Read the time from the last pulse on the OPTO.
          CALCULATE_spindletakupMotor_CLOSED_LOOP_SPEED_ERROR_AND_CORRECT();
          TAPE_SPEED_COUNTER = 0;                                               //   and set the counter to 0 to start counting until
          TAPE_SPEED_OPTO_DEBOUNCE_COMPLETE_FLAG_LOW = false;                   //   and initialize varaibles for the next rotation
          TAPE_SPEED_OPTO_DEBOUNCE_COUNTER = 0;
        }// End if(TAPE_SPEED_OPTO_STATE) 
      }// End if(TAPE_SPEED_OPTO_DEBOUNCE_COUNTER == TAPE_SPEED_OPTO_DEBOUNCE_COUNT)                                                                      //   the next pulse.
    }// End f(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG)
//--------------------- End  Tape Speed Monitor ---------------------------
} // End void TIMER_ISR(void)
 


// Motor functions 
// Only two motor-specific functions to call:
// motorname->run(COMMAND)
// where COMMAND is
// FORWARD            Run Forward
// BACKWARD           Run Reverse
// BRAKE              Short the windings together.  Stops the motor as fast as possible.
// RELEASE            Both windings in High impedance state. Free wheeling motor like it is disconnected.
// These are the defines from the libeary
//#define FORWARD 1
//#define BACKWARD 2
//#define BRAKE 3
//#define RELEASE 4
// all of them are tpype uint8_t
// motorname->setSpeed(SPEED)
// where SPEED is 0 to 255 with 0 as the slowest

//--------------------------------- CLOSED LOOP TEST FUNCTIONS -----------------------------------
// Tension potentiometer variables
int tensionRoller = 3;
int tensionRoller_POT_VOLTAGE_READING = 0;  //NEW FOR PID TEST

void RUN_spindletakupMotor_AT_CLOSED_LOOP_SPEED(float CL_SPEED)
// The unit of the variable CL_SPEED is in RPS (Revolutions per second or Hz).
{
   INPUT_PERIOD = (int)(((1.0/CL_SPEED)*1000.0));  //msec
   TAPE_SPEED_OPTO_STATE = false;
   LAST_TAPE_SPEED_OPTO_STATE = false; 
   TAPE_SPEED_OPTO_DEBOUNCE_COUNTER = 0;  
   TAPE_SPEED_COUNTER = 0;  
   TAPE_SPEED_OPTO_DEBOUNCE_DELAY_COUNTER = 0;  
   spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG = true;
   Serial.print("DESURED PERIOD = ");
   Serial.println(INPUT_PERIOD); 
}

void CALCULATE_spindletakupMotor_CLOSED_LOOP_SPEED_ERROR_AND_CORRECT(void)
{
// The interrupt service routine TIMER_ISR measures the period of the idler pulley and puts it in
// the variable TAPE_SPEED_PERIOD.  The unit of TAPE_SPEED_PERIOD is msec.
// This routine variaes the PWM command to thespindletakeupMotor to servo the motor to match 
// TAPE_SPEED_PERIOD to 1/(CL_SPEED/60) or 1/rev/sec. or 1/f.

  PERIOD_ERROR = INPUT_PERIOD - TAPE_SPEED_PERIOD;
  spindletakeupMotorSpeed_FLOAT = (float)spindletakeupMotorSpeed;
  PERIOD_ERROR_FLOAT = (float)PERIOD_ERROR;
  spindletakeupMotorSpeed_FLOAT = spindletakeupMotorSpeed_FLOAT - (PERIOD_ERROR_FLOAT * SPEED_SERVO_GAIN_CONSTANT);
  spindletakeupMotorSpeed = (int)spindletakeupMotorSpeed_FLOAT;
  if(spindletakeupMotorSpeed <= spindletakeupMotorSpeed_LOWER_LIMIT)
  {
    spindletakeupMotorSpeed = spindletakeupMotorSpeed_LOWER_LIMIT;
  }
  if(spindletakeupMotorSpeed >= spindletakeupMotorSpeed_UPPER_LIMIT)
  {
    spindletakeupMotorSpeed = spindletakeupMotorSpeed_UPPER_LIMIT;
  }
}

//------------------------------END CLOSED LOOP TEST FUNCTIONS -----------------------------------
void setup() 
{
// set up Serial library at 115200 bps
//  Serial.begin(115200);           
Serial.begin(9600); 
// Arduino I/O pins I/O setup

pinMode(LED,OUTPUT);
pinMode(TAPE_SPEED_OPTO,INPUT);

// Timer initialization 

  Timer1.initialize(1000);              // Set the timer resolution to 1 msec.  (1000 usec.) 
  Timer1.attachInterrupt(TIMER_ISR);

// Motor Setup
  
  AFMS.begin();  // Sets default frequency at 1.6KHz
  
// Default Speeds (60 of 255 counts.  Abount nominal speed.
// NOTEL  We always use the speed variables i.e spindlesupplyMotorSpeed so we can query the speed at any time  
 
  spindlesupplyMotorSpeed = 60;
  spindlesupplyMotor->setSpeed(spindlesupplyMotorSpeed);

  spindletakeupMotorSpeed = 60;
  spindletakeupMotor->setSpeed(spindletakeupMotorSpeed);

  pellonMotor1Speed = 60;
  pellonMotor1->setSpeed(pellonMotor1Speed);

  pellonMotor2Speed = 60;
  pellonMotor2->setSpeed(pellonMotor2Speed); 
  
  // Turn all motor off.  Free wheeling
  spindlesupplyMotor->run(RELEASE);
  spindletakeupMotor->run(RELEASE);
  pellonMotor1->run(RELEASE);
  pellonMotor2->run(RELEASE);
  SHOW_MOTOR_COMMAND_CHOICES();
  
// Initialize LCD Display 
/*
  lcd.begin(16, 2); // Set up LCD's number of columns and rows 
  lcd.setCursor(0,0);
  lcd.setBacklight(WHITE); 
  lcd.print("Opencleaner"); //Opening screen
  lcd.setCursor(0, 1);  // (note: line 1 is the second row, since counting begins with 0):
  lcd.print("Version 1.0.0.0"); 
  delay(5000);
  lcd.clear();
  lcd.print("Select Function"); 
  */
//  MAIN_LOOP_TIMER SETUP

CURRENT_TIME = millis();
MAIN_LOOP_TIMER = CURRENT_TIME + MAIN_LOOP_TIMER_INCREMENT;   // MAIN_LOOP_TIMER_INCREMENT = 100 msec.
}

uint8_t i=0;


//===================================  MAIN LOOP =============================
void loop() 
{
// --------------------------------- Timer Loop ------------------------------

CURRENT_TIME = millis();
if (CURRENT_TIME >= MAIN_LOOP_TIMER)
{
   SERIAL_STATUS_OUTPUT_UPDATE_COUNTER++;
   MAIN_LOOP_TIMER = CURRENT_TIME + MAIN_LOOP_TIMER_INCREMENT; 
    if(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG)     
    {
       spindletakeupMotor->setSpeed(spindletakeupMotorSpeed);  
       if(SERIAL_STATUS_OUTPUT_UPDATE_COUNTER >= SERIAL_STATUS_OUTPUT_UPDATE_COUNT)  // We are just going to send a serial print of 
       {                                                                            //    the rotation period every 500 msec.
          Serial.println("-----------------------");
          Serial.print("TARGET = "); 
          Serial.println(INPUT_PERIOD);         
          Serial.print("TAPE_SPEED_PERIOD = ");                                       //    The serial port is not fast enough to do it 
          Serial.println(TAPE_SPEED_PERIOD);                                          //    every time this ISR fires.
          Serial.print("PERIOD ERROR = ");
          Serial.println(PERIOD_ERROR); 
          Serial.print("spindletakeupMotorSpeed = ");    
          Serial.println(spindletakeupMotorSpeed);
           SERIAL_STATUS_OUTPUT_UPDATE_COUNTER = 0;
       }// if(SERIAL_STATUS_OUTPUT_UPDATE_COUNTER >= SERIAL_STATUS_OUTPUT_UPDATE_COUNT)
    }// End if(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG) 
}// End if (CURRENT_TIME >= MAIN_LOOP_TIMER)
// ------------------------------- End Timer Loop ------------------------------
 
/*   lcd.setBacklight(WHITE);
   uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) {
      lcd.print("Brake Motors ");
      spindlesupplyMotor->run(RELEASE);
      spindletakeupMotor->run(RELEASE);
	  pellonMotor1->run(RELEASE);
	  pellonMotor2->run(RELEASE);
    }
    if (buttons & BUTTON_DOWN) {
      lcd.print("Stop Tape ");
      spindlesupplyMotor->run(FORWARD);
      spindlesupplyMotor->setSpeed(230);
      spindletakeupMotor->run(FORWARD);
      spindletakeupMotor->setSpeed(230);
	  pellonMotor1->run(RELEASE);
	  pellonMotor2->run(RELEASE);
    }
    if (buttons & BUTTON_LEFT) {
      lcd.print("REVERSE TAPE ");
      spindlesupplyMotor->run(FORWARD);
      spindlesupplyMotor->setSpeed(210);
      spindletakeupMotor->run(FORWARD);
      spindletakeupMotor->setSpeed(30);
	  pellonMotor1->run(FORWARD);
	  pellonMotor2->run(FORWARD);
	  delay(20000);
	  spindletakeupMotor->setSpeed(25);
	  spindlesupplyMotor->setSpeed(215);
	  delay(20000);
	  spindletakeupMotor->setSpeed(20);
	  spindlesupplyMotor->setSpeed(220);
	  delay(20000);
	  spindletakeupMotor->setSpeed(15);
	  spindlesupplyMotor->setSpeed(225);
	  delay(20000);
	  spindletakeupMotor->setSpeed(10);
	  spindlesupplyMotor->setSpeed(230);
	  delay(20000);
	  spindletakeupMotor->setSpeed(5);
	  spindlesupplyMotor->setSpeed(235);
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.print("FORWARD TAPE ");
      spindlesupplyMotor->run(FORWARD);
	  spindlesupplyMotor->setSpeed(30);
      spindletakeupMotor->run(FORWARD);
      spindletakeupMotor->setSpeed(210);
	  pellonMotor1->run(FORWARD);
	  pellonMotor2->run(FORWARD);
	  delay(20000);
      spindlesupplyMotor->setSpeed(25);
	  spindletakeupMotor->setSpeed(215);
	  delay(20000);
	  spindlesupplyMotor->setSpeed(20);
	  spindletakeupMotor->setSpeed(220);
	  delay(20000);
	  spindlesupplyMotor->setSpeed(15);
	  spindletakeupMotor->setSpeed(225);
	  delay(20000);
	  spindlesupplyMotor->setSpeed(10);
	  spindletakeupMotor->setSpeed(230);
	  delay(20000);
	  spindlesupplyMotor->setSpeed(5);
	  spindletakeupMotor->setSpeed(235);
    }
    if (buttons & BUTTON_SELECT) {
      lcd.print("IDLE ");
      spindlesupplyMotor->run(FORWARD);
      spindlesupplyMotor->setSpeed(20);
      spindletakeupMotor->run(FORWARD);
      spindletakeupMotor->setSpeed(20);
	  pellonMotor1->run(RELEASE);
	  pellonMotor2->run(RELEASE);
    }
  }
  
  // read photointerrupter state
  int photointState1 = digitalRead(photointerrupt1);
  int photointState2 = digitalRead(photointerrupt2);
  
  //read tension potentiometer state
  val = analogRead(tensionRoller);
  
  //print photointerrupter and potentiometer readings   
  lcd.setCursor(0,1);
  lcd.print("SP  TP  POT      ");
  lcd.setCursor(3,1);
  lcd.print(photointState1);
  lcd.setCursor(7,1);
  lcd.print(photointState2);
  lcd.setCursor(12,1);
  lcd.print(val);
  delay(500);        // delay in between reads for stability
*/

// ----------------------------- Serial Port Interface ------------------------------
// Nothing in here runs unless ther is data in the serial FIFO
// Just ignore all of this code if you are using the LCD as the interface
if (Serial.available() > 0) 
  {
  Serial.readBytesUntil('\n',COMMAND,20);// 
  char val = COMMAND[0];
  CLEAR_SERIAL_BUFFER();
 
// spindlesupplyMotor
  
  if (val == '1')// spindlesupplyMotor
    {
	    lcd.clear();
	    lcd.setCursor(0,0);
	    lcd.print("FORWARD ");
      spindlesupplyMotor->run(FORWARD);
      val = 'x';
    }
  else if (val == '2')// spindlesupplyMotor
    {
	    lcd.clear();
	    lcd.setCursor(0,0);
	    lcd.print("BACKWARD ");
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
      Serial.print("Current Speed = ");
      Serial.println(spindlesupplyMotorSpeed);
      Serial.println("Enter spindlesupplyMotorSpeed(0-255)");
      
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
	
// spindletakeupMotor
	
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
      spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG = false;
      val = 'x';
    }
  else if (val == 'A')// spindletakeupMotor
    {
      Serial.print("Current Speed = ");
      Serial.println(spindletakeupMotorSpeed);
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
	
 // pellonMotor1
	
 if (val == 'B')// pellonMotor1
    {
      pellonMotor1->run(FORWARD);
    }
  else if (val == 'C')// pellonMotor1
    {
      pellonMotor1->run(RELEASE);
      val = 'x';
    }
  else if (val == 'D')// pellonMotor1
    {
      Serial.println("Enter speed (0-255)");
      while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
        delay(10);
        pellonMotor1Speed  = Serial.parseInt();
        pellonMotor1->setSpeed(pellonMotor1Speed);
        CLEAR_SERIAL_BUFFER();
        SHOW_MOTOR_COMMAND_CHOICES();
       val = 'x';
    }
	
// pellonMotor2 
	
if (val == 'E')// pellonMotor2
    {
       pellonMotor2->run(FORWARD);
       val = 'x';
    }
  else if (val == 'F')// pellonMotor2
    {
      pellonMotor2->run(RELEASE);
      val = 'x';
    }
  else if (val == 'G')// pellonMotor2
    {
      Serial.println("Enter speed (0-255)");
      while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
      delay(10);
      pellonMotor2Speed  = Serial.parseInt();
      pellonMotor2->setSpeed(pellonMotor2Speed);
      CLEAR_SERIAL_BUFFER();
      SHOW_MOTOR_COMMAND_CHOICES();
      val = 'x';
    }
   else if (val == 'H')// READ THE tensionRoller POT
    {
      tensionRoller_POT_VOLTAGE_READING = analogRead(tensionRoller);
      Serial.print("tensionRoller_POT_VOLTAGE_READING = ");
      Serial.println(tensionRoller_POT_VOLTAGE_READING);
    }
   else if (val == 'I')// Change Tape duration
    {
     
    }
   else if (val == 'J')// Change Direction
    {
      Serial.println("Enter Direction");
      Serial.println("1 = FORWARD");
      Serial.println("2 = REVERSE");
      Serial.println("3 = BRAKE");
      Serial.println("4 = RELEASE");
      while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
      delay(10);
      DIR_RELEASE_BRAKE  = Serial.parseInt();
      CLEAR_SERIAL_BUFFER();
      SHOW_MOTOR_COMMAND_CHOICES();
      val = 'x';
    }
   else if (val == 'K')// Run
    {
       
    }
   else if (val == 'L')// Off
    {
     spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG = false;
     spindletakeupMotor->run(RELEASE);
    }
   else if (val == 'M')// Ramp spindlesupplyMotor 0-255
    {
      Serial.println("RAMP");
      spindlesupplyMotor->setSpeed(0);
      spindlesupplyMotor->run(FORWARD);
     
    
      for (int i=0; i <= 255; i++)
      {
       spindlesupplyMotor->setSpeed(i);
       delay(200);
      } 
       val = 'x';
    }  
     else if (val == 'N')// READ THE TAPE_SPEED_OPTO_STATE
     {
       TAPE_SPEED_OPTO_STATE  = digitalRead(TAPE_SPEED_OPTO);
       Serial.print("TAPE_SPEED_OPTO_STATE = ");
       Serial.println(TAPE_SPEED_OPTO_STATE);
    }
     else if (val == 'O')// Run the spindletakeupMotor as speed
     {
      Serial.println("Enter speed (RPM)");
      while (!Serial.available())
      { 
        //wait for speed for entry serial port
      }
      delay(10);
      spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND = Serial.parseInt();
      spindletakeupMotor->setSpeed(35);
      spindletakeupMotor->run(FORWARD);
      delay(1000);
      
      
      RUN_spindletakupMotor_AT_CLOSED_LOOP_SPEED(spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND);  // 5 Revs per sec. NOTE: The speed is a float
     }
  } //end if (Serial.available() > 0) 
// ------------------------ End Serial Port Interface ------------------------------
} // end loop
