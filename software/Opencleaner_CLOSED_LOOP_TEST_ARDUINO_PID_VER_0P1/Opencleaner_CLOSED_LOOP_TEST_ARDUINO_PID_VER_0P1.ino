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
#include <PID_v1.h>
#include <EEPROMex.h>
#include <EEPROMVar.h>

// Table for querries from USB port
#define Run_spindlesupplyMotor_Forward_at_current_Speed '1'
#define Run_spindlesupplyMotor_Backward_at_current_Speed '2'
#define SET_spindlesupplyMotor_closed_loop_DIRECTION '3'
#define STOP_spindlesupplyMotor '4'
#define Set_spindlesupplyMotor_Speed '5'
#define Run_spindletakeupMotor_Forward_at_current_Speed '6' 
#define Run_spindletakeupMotor_Backward_at_current_Speed '7'
#define SET_spindletakeupMotor_closed_loop_DIRECTION '8'
#define STOP_spindletakeupMotor '9'
#define Set_spindletakeupMotor_Speed 'A'
#define Run_pellonMotor1_forward_at_current_Speed 'B'
#define Stop_pellonMotor1 'C' 
#define Set_pellonMotor1_Speed 'D'
#define Run_pellonMotor2_forward_at_current_Speed 'E'
#define Stop_pellonMotor2 'F'
#define Set_pellonMotor2_Speed 'G'
#define Read_tensionRoller_POT_VOLTAGE 'H'
#define Change_Tape_duration 'I'
#define Change_Direction 'J'
#define Run 'K'
#define Off 'L'
#define Ramp_spindlesupplyMotor_0_255 'M'
#define Read_Tape_Speed_Opto 'N' 
#define Run_the_spindletakeupMotor_at_closed_loop_Speed 'O'
#define Run_spindlesupplyMotor_at_closed_loop_tension 'P'
#define SET_spindletakeupMotor_PID_Kp 'Q'
#define SET_spindletakeupMotor_PID_Ki 'R'
#define SET_spindletakeupMotor_PID_Kd 'S'
#define SET_spindlesupplyMotor_PID_Kp 'T'
#define SET_spindlesupplyMotor_PID_Ki 'U'
#define SET_spindlesupplyMotor_PID_Kd 'V'
#define SET_spindletakeupMotor_PID_Speed_Upper_limit 'W'
#define SET_spindletakeupMotor_PID_Speed_Lower_limit 'X'
#define SET_spindlesupplyMotor_PID_Speed_Upper_limit 'Y'
#define SET_spindlesupplyMotor_PID_Speed_Lower_limit 'Z'
#define spindletakeupMotor_PID_EEPROM_SAVE '%'
#define spindlesupplyMotor_PID_EEPROM_SAVE '$'
  
#define GET_spindlesupplyMotor_SPEED 'a'
#define GET_spindletakeupMotor_SPEED 'b'
#define GET_pellonMotor1_SPEED 'c'
#define GET_pellonMotor2_SPEED 'd'
#define GET_system_STATES 'e'
#define GET_spindletakeupMotor_PID_Kp 'g'
#define GET_spindletakeupMotor_PID_Ki 'h'
#define GET_spindletakeupMotor_PID_Kd 'i'
#define GET_spindlesupplyMotor_PID_Kp 'j'
#define GET_spindlesupplyMotor_PID_Ki 'k'
#define GET_spindlesupplyMotor_PID_Kd 'l'
#define GET_spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND 'm'
#define GET_tensionRoller_POT_VOLTAGE_SETPOINT 'n'
#define GET_spindletakeupMotor_TAPE_SPEED_PERIOD 'o'
#define GET_spindletakeupMotor_PID_Speed_Upper_limit 'p'
#define GET_spindletakeupMotor_PID_Speed_Lower_limit 'q'
#define GET_spindlesupplyMotor_PID_Speed_Upper_limit 'r'
#define GET_spindlesupplyMotor_PID_Speed_Lower_limit 's'
#define GET_spindletakeupMotorSpeed_CLOSED_LOOP 't'
#define GET_spindlesupplyMotor_closed_loop_DIRECTION 'u'
#define GET_spindletakeupMotor_closed_loop_DIRECTION 'v'
#define GET_spindletakeupMotorSpeed_OPEN_LOOP_RPM 'w'
#define GET_photointState1 'x'
#define GET_photointState2 'y'

// Motor shield instantiation 

// Creates motor shield object with alternate 0x61 I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0X61); 
// Selects ports for spindle and pellon motors
Adafruit_DCMotor *spindlesupplyMotor = AFMS.getMotor(1);
Adafruit_DCMotor *spindletakeupMotor = AFMS.getMotor(2);
Adafruit_DCMotor *pellonMotor1 = AFMS.getMotor(3);
Adafruit_DCMotor *pellonMotor2 = AFMS.getMotor(4);

// LCD shield instantiation

// Creates LCD shield object
//Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
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
#define photointerrupt1 3
#define photointerrupt2 5
#define tensionRoller A3

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

int TAPE_SPEED_COUNTER = 0;                                 // msec.
int TAPE_SPEED_OPTO_DEBOUNCE_COUNTER = 0;                   // msec.
const int TAPE_SPEED_OPTO_DEBOUNCE_COUNT = 1;               // msec.
int TAPE_SPEED_OPTO_DEBOUNCE_DELAY_COUNTER = 0;             // msec.
const int TAPE_SPEED_OPTO_DEBOUNCE_DELAY_COUNT = 0;         // msec.
boolean TAPE_SPEED_OPTO_DEBOUNCE_COMPLETE_FLAG_LOW = false;
int TAPE_SPEED_PERIOD = 0;
double TAPE_SPEED_PERIOD_DOUBLE = 0.0;
int SERIAL_STATUS_OUTPUT_UPDATE_COUNTER = 0;                // msec.
const int SERIAL_STATUS_OUTPUT_UPDATE_COUNT = 10;           // 1 sec..
double INPUT_PERIOD_DOUBLE = 0.0;                           // Period of the desired speed input = 1/SPEED
double spindletakeupMotorSpeed_CLOSED_LOOP = 0.0;
double spindletakeupMotor_PID_Kp = 0.005;
double spindletakeupMotor_PID_Ki = 0.01;
double spindletakeupMotor_PID_Kd = 0.01;
int INPUT_PERIOD = 0;
int PERIOD_ERROR = 0;                                //       
int spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND = 30;   // Speed input to the closed loop speed
double spindletakeupMotorSpeed_LOWER_LIMIT = 12.0;
int spindletakeupMotorSpeed_LOWER_LIMIT_INT = (int)(spindletakeupMotorSpeed_LOWER_LIMIT);
double spindletakeupMotorSpeed_UPPER_LIMIT = 40.0;
int spindletakeupMotorSpeed_UPPER_LIMIT_INT = (int)(spindletakeupMotorSpeed_UPPER_LIMIT);
int spindletakeupMotor_closed_loop_DIRECTION = FORWARD;
//---------------------------------------------------------------------------------------------------

// Tape tension control servo variables and constants

boolean spindlesupplyMotor_CLOSED_LOOP_TENSION_RUN_FLAG = false;
boolean spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG = false;
int tensionRoller_POT_VOLTAGE_READING = 0;  //NEW FOR PID TEST
double tensionRoller_POT_VOLTAGE_READING_DOUBLE = 0.0;
double spindlesupplyMotorSpeed_CLOSED_LOOP = 0.0;
double tensionRoller_POT_VOLTAGE_SETPOINT = 700.0;
double spindlesupplyMotor_PID_Kp = 0.05;
double spindlesupplyMotor_PID_Ki = 0.1;
double spindlesupplyMotor_PID_Kd = 0.1;
double spindlesupplyMotorSpeed_LOWER_LIMIT = 40.0;
int spindlesupplyMotorSpeed_LOWER_LIMIT_INT = (int)(spindlesupplyMotorSpeed_LOWER_LIMIT);
double spindlesupplyMotorSpeed_UPPER_LIMIT = 100.0;
int spindlesupplyMotorSpeed_UPPER_LIMIT_INT = (int)(spindlesupplyMotorSpeed_UPPER_LIMIT);
int spindlesupplyMotor_closed_loop_DIRECTION = FORWARD;

//  Main Loop Timer variables and constants

unsigned long MAIN_LOOP_TIMER = 0;
const unsigned long   MAIN_LOOP_TIMER_INCREMENT = 50; //50 msec
unsigned long  CURRENT_TIME = 0;
 

// Serail Comm Variables

String inputString = "";              // a string to hold incoming data
boolean stringComplete = false;       // whether the string is complete
char COMMAND = '/';                   // Command from serial input
int COMMAND_PARAMETER = 0;            // Command Parameter from serial input      
double COMMAND_PARAMETER_DOUBLE = 0.0;       

//boolean LED_STATE = false;

boolean photointState1 = true;
boolean photointState2 = true;

// Motor variables
int spindlesupplyMotorSpeed = 0;
int spindletakeupMotorSpeed = 0;
int pellonMotor1Speed = 0;
int pellonMotor2Speed = 0;
boolean spindletakeupMotorSpeed_OPEN_LOOP_RUN_FLAG = false;
double spindletakeupMotorSpeed_OPEN_LOOP_RPM = 0.0;

PID spindletakeupMotor_PID(&TAPE_SPEED_PERIOD_DOUBLE,&spindletakeupMotorSpeed_CLOSED_LOOP,&INPUT_PERIOD_DOUBLE,
spindletakeupMotor_PID_Kp,spindletakeupMotor_PID_Ki,spindletakeupMotor_PID_Kd,REVERSE);

PID spindlesupplyMotor_PID(&tensionRoller_POT_VOLTAGE_READING_DOUBLE,&spindlesupplyMotorSpeed_CLOSED_LOOP,
&tensionRoller_POT_VOLTAGE_SETPOINT, spindlesupplyMotor_PID_Kp, spindlesupplyMotor_PID_Ki, spindlesupplyMotor_PID_Kd,DIRECT); 
 

// General functions

// Toggles arduino on-board LED for easy debugging
//void toggle_LED(void)  
//{
//  LED_STATE = !LED_STATE;
//  digitalWrite(LED, LED_STATE);
//}

void SHOW_MOTOR_COMMAND_CHOICES(void)
{
 /* 
   Serial.println(F("1 (Run spindlesupplyMotor Forward at current speed"));
  Serial.println(F("2 Run spindlesupplyMotor Backward at current speed"));
  Serial.println(F("3 Brake spindlesupplyMotor"));
  Serial.println(F("4 STOP spindlesupplyMotor"));
  Serial.println(F("5 Set spindlesupplyMotor Speed"));
  
  Serial.println(F("6 Run spindletakeupMotor Forward at current speed"));
  Serial.println(F("7 Run spindletakeupMotor Backward at current speed"));
  Serial.println(F("8 Brake spindletakeupMotor"));
  Serial.println(F("9 STOP spindletakeupMotor"));
  Serial.println(F("A Set spindletakeupMotor Speed"));

  Serial.println(F("B Run pellonMotor1 forward at current speed"));
  Serial.println(F("C Stop pellonMotor1"));
  Serial.println(F("D Set pellonMotor1 Speed"));

  Serial.println(F("E Run pellonMotor2 forward at current speed"));
  Serial.println(F("F Stop pellonMotor2"));
  Serial.println(F("G Set pellonMotor2 Speed"));
  
  Serial.println(F("H Read tensionRoller_POT_VOLTAGE"));

  Serial.println(F("I Change Tape duration"));
  Serial.println(F("J Change Direction"));
  Serial.println(F("K Run"));
  Serial.println(F("L Off"));

  Serial.println(F("M Ramp spindlesupplyMotor 0-255"));
  Serial.println(F("N Read Tape Speed Opto"));
  Serial.println(F("O Run the spindletakeupMotor at closed loop speed"));

  Serial.println(F("P Run spindlesupplyMotor at closed loop tension"));
  */
}

// Serial Port Support

void CLEAR_SERIAL_BUFFER(void)
{
 int junk = 0;
  while (Serial.available())
  {
  }
}

void PARSE_COMMAND (String COMMAND_STRING) 
{
  int count = 0;
  char COMMAND_STRING_CHAR = '/';
  String PARAMETER_STRING = "";
  COMMAND = COMMAND_STRING[0];
  count = 1;
  while(COMMAND_STRING_CHAR != '\n')
  {
    COMMAND_STRING_CHAR = COMMAND_STRING[count];
    if (COMMAND_STRING_CHAR != '\n')
    {
    PARAMETER_STRING+= COMMAND_STRING[count];
    count++;
    }
  }
//  switch (COMMAND)
//  { 
//  case SET_LCD_STRING:
//    LCD_STRING = PARAMETER_STRING;
//  break;
//  }
  COMMAND_PARAMETER = PARAMETER_STRING.toInt();
  COMMAND_PARAMETER_DOUBLE = PARAMETER_STRING.toFloat();
  PARAMETER_STRING = "";
  CLEAR_SERIAL_BUFFER();
}

void TIMER_ISR(void)
{
// Timer interrupt service routine.
// Fires once every 1 msec. all the time.
// NOTE:  I tried updating the motor speed in side theis ISR.  It dies not work.  No motor shield functions
// can be called from inside any ISR since they must use interrupts for the I2C interface and interrupts are
// disabled when ISRs fire.  Consequently, the speed update is called in the timer loop inside the main loop
//-------------------------- Tape Speed Monitor ---------------------------
//    toggle_LED();
//    if(spindlesupplyMotor_CLOSED_LOOP_TENSION_RUN_FLAG)
//    {
    tensionRoller_POT_VOLTAGE_READING = analogRead(tensionRoller);
    tensionRoller_POT_VOLTAGE_READING_DOUBLE = (double)(tensionRoller_POT_VOLTAGE_READING);
    TAPE_SPEED_OPTO_STATE  = digitalRead(TAPE_SPEED_OPTO);
    photointState1 = digitalRead(photointerrupt1);
    photointState2 = digitalRead(photointerrupt2);
    TAPE_SPEED_COUNTER++;

//    }

    if(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG || spindletakeupMotorSpeed_OPEN_LOOP_RUN_FLAG)
    {
      if (!TAPE_SPEED_OPTO_STATE && LAST_TAPE_SPEED_OPTO_STATE)
      {
          TAPE_SPEED_PERIOD = TAPE_SPEED_COUNTER;                               // Read the time from the last pulse on the OPTO.
          TAPE_SPEED_PERIOD_DOUBLE = (double)( TAPE_SPEED_PERIOD);
          CALCULATE_spindletakupMotor_CLOSED_LOOP_SPEED_ERROR_AND_CORRECT();
          TAPE_SPEED_COUNTER = 0;   
      }
      LAST_TAPE_SPEED_OPTO_STATE = TAPE_SPEED_OPTO_STATE;
    }
/*      TAPE_SPEED_COUNTER++;
      TAPE_SPEED_OPTO_STATE  = digitalRead(TAPE_SPEED_OPTO);
      tensionRoller_POT_VOLTAGE_READING = analogRead(tensionRoller);
      tensionRoller_POT_VOLTAGE_READING_DOUBLE = (double)(tensionRoller_POT_VOLTAGE_READING);
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
          TAPE_SPEED_PERIOD_DOUBLE = (double)( TAPE_SPEED_PERIOD);
          CALCULATE_spindletakupMotor_CLOSED_LOOP_SPEED_ERROR_AND_CORRECT();
          TAPE_SPEED_COUNTER = 0;                                               //   and set the counter to 0 to start counting until
          TAPE_SPEED_OPTO_DEBOUNCE_COMPLETE_FLAG_LOW = false;                   //   and initialize varaibles for the next rotation
          TAPE_SPEED_OPTO_DEBOUNCE_COUNTER = 0;
        }// End if(TAPE_SPEED_OPTO_STATE) 
      }// End if(TAPE_SPEED_OPTO_DEBOUNCE_COUNTER == TAPE_SPEED_OPTO_DEBOUNCE_COUNT)                                                                      //   the next pulse.
    }// End f(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG)
*/
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

void RUN_spindletakupMotor_AT_CLOSED_LOOP_SPEED(float CL_SPEED, int DIRECTION)
// The unit of the variable CL_SPEED is in RPS (Revolutions per second or Hz).
{
   INPUT_PERIOD = (int)(((1.0/(CL_SPEED/60.0))*1000.0));  //msec
   INPUT_PERIOD_DOUBLE = (double)(INPUT_PERIOD);
   TAPE_SPEED_OPTO_STATE = false;
   LAST_TAPE_SPEED_OPTO_STATE = false; 
   TAPE_SPEED_OPTO_DEBOUNCE_COUNTER = 0;  
   TAPE_SPEED_COUNTER = 0;  
   TAPE_SPEED_OPTO_DEBOUNCE_DELAY_COUNTER = 0;  
   spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG = true;
   Serial.print("DESURED PERIOD = ");
   Serial.println(INPUT_PERIOD); 
   spindletakeupMotor->run(DIRECTION);
  spindletakeupMotor_PID.SetMode(AUTOMATIC); 
}

void CALCULATE_spindletakupMotor_CLOSED_LOOP_SPEED_ERROR_AND_CORRECT(void)
{
// The interrupt service routine TIMER_ISR measures the period of the idler pulley and puts it in
// the variable TAPE_SPEED_PERIOD.  The unit of TAPE_SPEED_PERIOD is msec.
// This routine variaes the PWM command to thespindletakeupMotor to servo the motor to match 
// TAPE_SPEED_PERIOD to 1/(CL_SPEED/60) or 1/rev/sec. or 1/f.
  PERIOD_ERROR = INPUT_PERIOD - TAPE_SPEED_PERIOD;
}

void RUN_spindlesupplyMotor_at_CLOSED_LOOP_TENSION(double Tension_POT_Setting,int DIRECTION)
{
  tensionRoller_POT_VOLTAGE_SETPOINT = Tension_POT_Setting;
  spindlesupplyMotor_CLOSED_LOOP_TENSION_RUN_FLAG = true;
  spindlesupplyMotor_PID.SetMode(AUTOMATIC); 
  spindlesupplyMotor->run(DIRECTION);
}

//------------------------------END CLOSED LOOP TEST FUNCTIONS -----------------------------------

//==========================================  SETUP  ==============================================
void setup() 
{
// set up Serial library at 115200 bps
  Serial.begin(9600);           

// Arduino I/O pins I/O setup

pinMode(LED,OUTPUT);
pinMode(TAPE_SPEED_OPTO,INPUT);
pinMode(photointerrupt1,INPUT_PULLUP);
pinMode(photointerrupt2,INPUT_PULLUP);

// Timer initialization 

  Timer1.initialize(1000);              // Set the timer resolution to 1 msec.  (1000 usec.) 
  Timer1.attachInterrupt(TIMER_ISR);

//Read Parameters from EEPROM

#define spindletakeupMotor_PID_Kp_EEPROM_ADDRESS 0
#define spindletakeupMotor_PID_Ki_EEPROM_ADDRESS 4
#define spindletakeupMotor_PID_Kd_EEPROM_ADDRESS 8
#define spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND_EEPROM_ADDRESS 12
#define spindletakeupMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS 16
#define spindletakeupMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS 20
#define spindletakeupMotor_closed_loop_DIRECTION_EEPROM_ADDRESS 24

#define spindlesupplyMotor_PID_Kp_EEPROM_ADDRESS 28
#define spindlesupplyMotor_PID_Ki_EEPROM_ADDRESS 32
#define spindlesupplyMotor_PID_Kd_EEPROM_ADDRESS 36
#define spindlesupplyMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS 40
#define spindlesupplyMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS 44
#define spindlesupplyMotor_closed_loop_DIRECTION_EEPROM_ADDRESS 48
#define tensionRoller_POT_VOLTAGE_SETPOINT_EEPROM_ADDRESS 52
// ===================================================================
// =================== EEPROM INITIALIZATION =========================
//         UNCOMMENT THIS SECTION TO INITIALIZE THE EEPROM
//         OF A BOARD THAT HAS NEVER HAD THE EEPROM WIRTTEN TOO.
//         DO IT ONCE AND THEN UN-COMMMENT.
//         THE PARAMS ARE WRITTEN TO FROM THE VB APP AFTER THAT.
/*
EEPROM.writeDouble(spindletakeupMotor_PID_Kp_EEPROM_ADDRESS,0.05);
EEPROM.writeDouble(spindletakeupMotor_PID_Ki_EEPROM_ADDRESS,0.1);
EEPROM.writeDouble(spindletakeupMotor_PID_Kd_EEPROM_ADDRESS,0.1);
EEPROM.writeInt(spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND_EEPROM_ADDRESS,30);
EEPROM.writeDouble(spindletakeupMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS,15);
EEPROM.writeDouble(spindletakeupMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS,40);
EEPROM.writeInt(spindletakeupMotor_closed_loop_DIRECTION_EEPROM_ADDRESS,FORWARD);

EEPROM.writeDouble(spindlesupplyMotor_PID_Kp_EEPROM_ADDRESS,0.5);
EEPROM.writeDouble(spindlesupplyMotor_PID_Ki_EEPROM_ADDRESS,0.1);
EEPROM.writeDouble(spindlesupplyMotor_PID_Kd_EEPROM_ADDRESS,0.1);
EEPROM.writeDouble(spindlesupplyMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS,20);
EEPROM.writeDouble(spindlesupplyMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS,100);
EEPROM.writeInt(spindlesupplyMotor_closed_loop_DIRECTION_EEPROM_ADDRESS,FORWARD);
EEPROM.writeDouble(tensionRoller_POT_VOLTAGE_SETPOINT_EEPROM_ADDRESS,700.0);

// ===============  END EEPROM INITIALIZATION =========================
*/
spindletakeupMotor_PID_Kp = EEPROM.readDouble(spindletakeupMotor_PID_Kp_EEPROM_ADDRESS);
spindletakeupMotor_PID_Ki = EEPROM.readDouble(spindletakeupMotor_PID_Ki_EEPROM_ADDRESS);
spindletakeupMotor_PID_Kd = EEPROM.readDouble(spindletakeupMotor_PID_Kd_EEPROM_ADDRESS);
spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND = EEPROM.readInt(spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND_EEPROM_ADDRESS);
spindletakeupMotorSpeed_LOWER_LIMIT = EEPROM.readDouble(spindletakeupMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS);
spindletakeupMotorSpeed_LOWER_LIMIT_INT = (int)(spindletakeupMotorSpeed_LOWER_LIMIT);
spindletakeupMotorSpeed_UPPER_LIMIT = EEPROM.readDouble(spindletakeupMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS);
spindletakeupMotorSpeed_UPPER_LIMIT_INT = (int)(spindletakeupMotorSpeed_UPPER_LIMIT);
spindletakeupMotor_closed_loop_DIRECTION =  EEPROM.readInt(spindletakeupMotor_closed_loop_DIRECTION_EEPROM_ADDRESS);

spindlesupplyMotor_PID_Kp = EEPROM.readDouble(spindlesupplyMotor_PID_Kp_EEPROM_ADDRESS);
spindlesupplyMotor_PID_Ki = EEPROM.readDouble(spindlesupplyMotor_PID_Ki_EEPROM_ADDRESS);
spindlesupplyMotor_PID_Kd = EEPROM.readDouble(spindlesupplyMotor_PID_Kd_EEPROM_ADDRESS);
spindlesupplyMotorSpeed_LOWER_LIMIT = EEPROM.readDouble(spindlesupplyMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS);
spindlesupplyMotorSpeed_LOWER_LIMIT_INT = (int)(spindlesupplyMotorSpeed_LOWER_LIMIT);
spindlesupplyMotorSpeed_UPPER_LIMIT = EEPROM.readDouble(spindlesupplyMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS);
spindlesupplyMotorSpeed_UPPER_LIMIT_INT = (int)(spindlesupplyMotorSpeed_UPPER_LIMIT);
spindlesupplyMotor_closed_loop_DIRECTION = EEPROM.readInt(spindlesupplyMotor_closed_loop_DIRECTION_EEPROM_ADDRESS);
tensionRoller_POT_VOLTAGE_SETPOINT = EEPROM.readDouble(tensionRoller_POT_VOLTAGE_SETPOINT_EEPROM_ADDRESS);

// Motor Setup
  
  AFMS.begin();  // Sets default frequency at 1.6KHz
  
// Default Speeds (60 of 255 counts.  Abount nominal speed.
// NOTEL  We always use the speed variables i.e spindlesupplyMotorSpeed so we can query the speed at any time  
 
  spindlesupplyMotorSpeed = 60;
  spindlesupplyMotor->setSpeed(spindlesupplyMotorSpeed);

  spindletakeupMotorSpeed = 10;
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

// PID INSTANTIATION

  spindletakeupMotor_PID.SetMode(MANUAL);          // Wake up with the PID OFF
                                                   // Turn it ON in 
                                                   //          void RUN_spindletakupMotor_AT_CLOSED_LOOP_SPEED(float CL_SPEED)
  spindletakeupMotor_PID.SetOutputLimits(spindletakeupMotorSpeed_LOWER_LIMIT, spindletakeupMotorSpeed_UPPER_LIMIT);
  spindletakeupMotor_PID.SetSampleTime(10);   

  spindlesupplyMotor_PID.SetOutputLimits(spindlesupplyMotorSpeed_LOWER_LIMIT, spindlesupplyMotorSpeed_UPPER_LIMIT);
  spindlesupplyMotor_PID.SetSampleTime(50); 
  Serial.println(F("Opencleaner_CLOSED_LOOP_TEST_ARDUINO_PID"));
}// end void setup()

uint8_t i=0;
//================================== END SETUP  =============================

//=================================  MAIN LOOP ==============================
void loop() 
{
// --------------------------------- Timer Loop ------------------------------

CURRENT_TIME = millis();
if (CURRENT_TIME >= MAIN_LOOP_TIMER)
{
   SERIAL_STATUS_OUTPUT_UPDATE_COUNTER++;
   MAIN_LOOP_TIMER = CURRENT_TIME + MAIN_LOOP_TIMER_INCREMENT; 
      if(SERIAL_STATUS_OUTPUT_UPDATE_COUNTER >= SERIAL_STATUS_OUTPUT_UPDATE_COUNT)  // We are just going to send a serial print of 
      {                                                                             //    the rotation period every 500 msec.
      SERIAL_STATUS_OUTPUT_UPDATE_COUNTER = 0;
      if(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG)     
       {                                                                            
          Serial.println(F("-----------------------"));
          Serial.print(F("TARGET = ")); 
          Serial.println(INPUT_PERIOD); 
          Serial.print(F("TARGET DOUBLE = "));       
          Serial.println(INPUT_PERIOD_DOUBLE);         
          Serial.println(F("TAPE_SPEED_PERIOD = "));                                       //    The serial port is not fast enough to do it 
          Serial.print(GET_spindletakeupMotor_TAPE_SPEED_PERIOD);  //for VB
          Serial.println(TAPE_SPEED_PERIOD);                                          //    every time this ISR fires.
          Serial.print(F("TAPE_SPEED_PERIOD_DOUBLE = "));
          Serial.println(TAPE_SPEED_PERIOD_DOUBLE); 
          Serial.print(F("PERIOD ERROR = "));
          Serial.println(PERIOD_ERROR); 
          Serial.print("spindletakeupMotorSpeed = ");    
          Serial.println(spindletakeupMotorSpeed);
          Serial.println("spindletakeupMotorSpeed_CLOSED_LOOP = ");
          Serial.print(GET_spindletakeupMotorSpeed_CLOSED_LOOP);   //for VB
          Serial.println(spindletakeupMotorSpeed_CLOSED_LOOP,4);
          spindletakeupMotorSpeed_OPEN_LOOP_RPM = ((1.0/(TAPE_SPEED_PERIOD_DOUBLE / 1000.0))*60.0);
          Serial.print(GET_spindletakeupMotorSpeed_OPEN_LOOP_RPM);
          Serial.println(spindletakeupMotorSpeed_OPEN_LOOP_RPM,4);
          
       }// End if(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG) 
       if(spindlesupplyMotor_CLOSED_LOOP_TENSION_RUN_FLAG)
       {
          Serial.println(F("-----------------------")); 
          Serial.print(F("TARGET = ")); 
          Serial.println(tensionRoller_POT_VOLTAGE_SETPOINT);
          Serial.print(F("tensionRoller_POT_VOLTAGE_READING_DOUBLE"));
          Serial.println(tensionRoller_POT_VOLTAGE_READING_DOUBLE);
          Serial.print(F("spindlesupplyMotorSpeed_CLOSED_LOOP"));
          Serial.println(spindlesupplyMotorSpeed_CLOSED_LOOP,4);
       }
       if(spindletakeupMotorSpeed_OPEN_LOOP_RUN_FLAG)
       {
          Serial.print(GET_spindletakeupMotor_TAPE_SPEED_PERIOD);  //for VB
          Serial.println(TAPE_SPEED_PERIOD); 
          spindletakeupMotorSpeed_OPEN_LOOP_RPM = ((1.0/(TAPE_SPEED_PERIOD_DOUBLE / 1000.0))*60.0);
          Serial.print(GET_spindletakeupMotorSpeed_OPEN_LOOP_RPM);
          Serial.println(spindletakeupMotorSpeed_OPEN_LOOP_RPM);        
       }
      }// if(SERIAL_STATUS_OUTPUT_UPDATE_COUNTER >= SERIAL_STATUS_OUTPUT_UPDATE_COUNT)
}// End if (CURRENT_TIME >= MAIN_LOOP_TIMER)
// ------------------------------- End Timer Loop ------------------------------
// ------------------------- ARDUINO PID INPLEMENTATION ------------------------
/* PID MEMBER FUNCTIONS
PID()
  Syntax: PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, Direction)
  Parameters:
    Input: The variable we're trying to control (double)
    Output: The variable that will be adjusted by the pid (double)
    Setpoint: The value we want to Input to maintain (double)
    Kp, Ki, Kd: Tuning Parameters. these affect how the pid will chage the output. (double>=0)
    Direction: Either DIRECT or REVERSE. determines which direction the output will move when faced with a given error. DIRECT is most common.
  Returns: None 
  
Compute()
  Contains the pid algorithm. it should be called once every loop(). 
  Most of the time it will just return without doing anything.  
  At a frequency specified by SetSampleTime it will calculate a new Output. 
  Syntax: Compute()
  Parameters: None
  Returns: True: when the output is computed
          False: when nothing has been done  
SetMode()
  Specifies whether the PID should be on (Automatic) or off (Manual.) 
  The PID defaults to the off position when created. 
  Syntax: SetMode(mode)
  Parameters: mode: AUTOMATIC or MANUAL
  Returns: None 
  
SetOutputLimits()
  The PID controller is designed to vary its output within a given range. 
  By default this range is 0-255: the arduino PWM range. 
  There's no use sending 300, 400, or 500 to the PWM.
  Depending on the application though, a different range may be desired. 
  Syntax: SetOutputLimits(min, max)
  Parameters: min: Low end of the range. must be < max (double)
              max: High end of the range. must be > min (double)
  Returns: None 

SetTunings()
  Tuning parameters (or "Tunings") dictate the dynamic behavior of the PID.
  Will it oscillate or not? Will it be fast or slow? 
  An initial set of Tunings is specified when the PID is created.
  For most users this will be enough. 
  There are times however, tunings need to be changed during run-time.
  At those times this function can be called.
  Syntax: SetTunings(Kp, Ki, Kd)
  Parameters: Kp: Determines how aggressively the PID reacts to the current amount of error
                  (Proportional) (double >=0).
              Ki: Determines how aggressively the PID reacts to error over time (Integral) (double>=0)
              Kd: Determines how aggressively the PID reacts to the change in error (Derivative) (double>=0) 
  Returns: None   
          
SetSampleTime()
  Determines how often the PID algorithm evaluates. 
  The default is 200mS. For robotics applications this may need to be faster, 
  but for the most part 200mS is plenty fast.
  Syntax: SetSampleTime(SampleTime)
  Parameters: SampleTime: How often, in milliseconds, the PID will be evaluated. (int>0)
  Returns: None 
  
SetControllerDirection()
  If my Input is above Setpoint, should the output be increased or decreased? 
  Depending on what the PID is connected to, either could be true.
  With a car, the output should be decreased to bring the speed down.
  For a refrigerator, the opposite is true. The output (cooling) needs to be 
  increased to bring my temperature down. This function specifies which type of 
  process the PID is connected to. This information is also specified when the PID constructed.
  Since it's unlikely that the process will switch from direct to reverse, 
  it's equally unlikely that anyone will actually use this function.
  Syntax: SetControllerDirection(Direction);
  Parameters: Direction: DIRECT (like a car) or REVERSE (like a refrigerator)
  Returns: None 
  
Display Functions 
  These functions query the PID internals to get current values. These are useful for display purposes.
  GetKp()
  GetKi()
  GetKd()
  GetMode()
  GetDirection()
  Syntax: GetKp()
          GetKi()
          GetKd()
          GetMode()
          GetDirection() 
   Parameters: None
   Returns: The corresponding internal value 
*/

      if(spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG) 
       {
        spindletakeupMotor_PID.Compute();
        spindletakeupMotorSpeed = (int)(spindletakeupMotorSpeed_CLOSED_LOOP);
        spindletakeupMotor->setSpeed(spindletakeupMotorSpeed);   
       }
     
       if(spindlesupplyMotor_CLOSED_LOOP_TENSION_RUN_FLAG)
       {
        spindlesupplyMotor_PID.Compute();
        spindlesupplyMotorSpeed = (int)(spindlesupplyMotorSpeed_CLOSED_LOOP);
        spindlesupplyMotor->setSpeed(spindlesupplyMotorSpeed);
       }

// ---------------------END ARDUINO PID INPLEMENTATION ------------------------

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
 if (stringComplete) 
 {
   PARSE_COMMAND (inputString); 
    //Serial.print(COMMAND); 
    //Serial.print(COMMAND_PARAMETER); 
  //Serial.print('\n');
  switch (COMMAND)
  {
// ==============================================
//        COMMANDS AND PARAMETER SETTING
//================================================    
//------------------------------------------------ 
    case Run_spindlesupplyMotor_Forward_at_current_Speed:
//	    lcd.clear();
//	    lcd.setCursor(0,0);
//	    lcd.print("FORWARD ");
      spindlesupplyMotor->run(FORWARD);
    break;
//------------------------------------------------ 
   case Run_spindlesupplyMotor_Backward_at_current_Speed: 
//	    lcd.clear();
//	    lcd.setCursor(0,0);
//	    lcd.print("BACKWARD ");
      spindlesupplyMotor->run(BACKWARD);
   break;
//------------------------------------------------ 
   case SET_spindlesupplyMotor_closed_loop_DIRECTION:
      spindlesupplyMotor_closed_loop_DIRECTION = COMMAND_PARAMETER;
   break;
//------------------------------------------------ 
   case STOP_spindlesupplyMotor:
      spindlesupplyMotor_CLOSED_LOOP_TENSION_RUN_FLAG = false;
      spindlesupplyMotor->run(RELEASE);
    break;
//------------------------------------------------ 
   case Set_spindlesupplyMotor_Speed:
      Serial.print(F("Current Speed = "));
      Serial.println(spindlesupplyMotorSpeed);
      Serial.println(F("Enter spindlesupplyMotorSpeed(0-255)"));
      spindlesupplyMotorSpeed  = COMMAND_PARAMETER;
      spindlesupplyMotor->setSpeed(spindlesupplyMotorSpeed);
      SHOW_MOTOR_COMMAND_CHOICES();
   break;
//------------------------------------------------ 
	 case Run_spindletakeupMotor_Forward_at_current_Speed:
      spindletakeupMotor->run(FORWARD);
      spindletakeupMotorSpeed_OPEN_LOOP_RUN_FLAG = true;
   break;  
//------------------------------------------------ 
   case Run_spindletakeupMotor_Backward_at_current_Speed:
      spindletakeupMotor->run(BACKWARD);
      spindletakeupMotorSpeed_OPEN_LOOP_RUN_FLAG = true;
   break;
//------------------------------------------------ 
  case SET_spindletakeupMotor_closed_loop_DIRECTION:
      spindletakeupMotor_closed_loop_DIRECTION = COMMAND_PARAMETER;
  break;
//------------------------------------------------ 
   case STOP_spindletakeupMotor:
      spindletakeupMotor_CLOSED_LOOP_SPEED_RUN_FLAG = false;
      spindletakeupMotor_PID.SetMode(MANUAL);                // Turn OFF the PID loopWake up with the PID OFF
      spindletakeupMotor->run(RELEASE);                       // Turn off the motor
      spindletakeupMotorSpeed_OPEN_LOOP_RUN_FLAG = false;
   break;
//------------------------------------------------ 
   case Set_spindletakeupMotor_Speed:
      Serial.print("Current Speed = ");
      Serial.println(spindletakeupMotorSpeed);
//     Serial.println("Enter speed (0-255)");
//      delay(10);
      spindletakeupMotorSpeed  = COMMAND_PARAMETER;
      spindletakeupMotor->setSpeed(spindletakeupMotorSpeed);
    break;
//------------------------------------------------ 
    case Run_pellonMotor1_forward_at_current_Speed:
      pellonMotor1->run(FORWARD);
    break;
//------------------------------------------------ 
   case Stop_pellonMotor1:
      pellonMotor1->run(RELEASE);
   break;
//------------------------------------------------ 
   case Set_pellonMotor1_Speed:
      Serial.println("Enter speed (0-255)");
      delay(10);
      pellonMotor1Speed  = COMMAND_PARAMETER;
      pellonMotor1->setSpeed(pellonMotor1Speed);
   break;
//------------------------------------------------ 
	 case Run_pellonMotor2_forward_at_current_Speed:
      pellonMotor2->run(FORWARD);
   break;
//------------------------------------------------ 
   case Stop_pellonMotor2:
      pellonMotor2->run(RELEASE);
   break;
//------------------------------------------------ 
   case Set_pellonMotor2_Speed:
      Serial.println("Enter speed (0-255)");
      delay(10);
      pellonMotor2Speed  = COMMAND_PARAMETER;
      pellonMotor2->setSpeed(pellonMotor2Speed);
      SHOW_MOTOR_COMMAND_CHOICES();
   break;
//------------------------------------------------ 
   case Read_tensionRoller_POT_VOLTAGE:
      tensionRoller_POT_VOLTAGE_READING = analogRead(tensionRoller);
      Serial.print("tensionRoller_POT_VOLTAGE_READING = ");
      Serial.println(tensionRoller_POT_VOLTAGE_READING);
   break;  
//------------------------------------------------ 
   case Change_Tape_duration:
   break;
//------------------------------------------------ 
   case Change_Direction:
      Serial.println("Enter Direction");
      Serial.println("1 = FORWARD");
      Serial.println("2 = REVERSE");
      Serial.println("3 = BRAKE");
      Serial.println("4 = RELEASE");
      delay(10);
      DIR_RELEASE_BRAKE  = COMMAND_PARAMETER;
    break;
//------------------------------------------------ 
    case Run:
    break;
//------------------------------------------------ 
    case Off:
    break;
 //------------------------------------------------ 
    case Ramp_spindlesupplyMotor_0_255:
      Serial.println("RAMP");
      spindlesupplyMotor->setSpeed(0);
      spindlesupplyMotor->run(FORWARD);
      for (int i=0; i <= 255; i++)
      {
       spindlesupplyMotor->setSpeed(i);
       delay(200);
      } 
    break;
//------------------------------------------------   
    case Read_Tape_Speed_Opto:
       TAPE_SPEED_OPTO_STATE  = digitalRead(TAPE_SPEED_OPTO);
       Serial.print("TAPE_SPEED_OPTO_STATE = ");
       Serial.println(TAPE_SPEED_OPTO_STATE);
    break;
//------------------------------------------------ 
    case Run_the_spindletakeupMotor_at_closed_loop_Speed:
       delay(10);
       Serial.println("Enter speed (RPM)");
       spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND = COMMAND_PARAMETER;
       Serial.println(spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND);
       delay(1000);
       RUN_spindletakupMotor_AT_CLOSED_LOOP_SPEED(spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND,
       spindletakeupMotor_closed_loop_DIRECTION); 
    break;
//------------------------------------------------ 
    case Run_spindlesupplyMotor_at_closed_loop_tension:
     tensionRoller_POT_VOLTAGE_SETPOINT = COMMAND_PARAMETER_DOUBLE;
     RUN_spindlesupplyMotor_at_CLOSED_LOOP_TENSION(tensionRoller_POT_VOLTAGE_SETPOINT,spindlesupplyMotor_closed_loop_DIRECTION);
    break;
//------------------------------------------------ 
    case SET_spindletakeupMotor_PID_Kp :
    spindletakeupMotor_PID_Kp = COMMAND_PARAMETER_DOUBLE;
    spindletakeupMotor_PID.SetTunings(spindletakeupMotor_PID_Kp,spindletakeupMotor_PID_Ki,spindletakeupMotor_PID_Kd);
    break;
//------------------------------------------------ 
    case SET_spindletakeupMotor_PID_Ki:
     spindletakeupMotor_PID_Ki = COMMAND_PARAMETER_DOUBLE;
     spindletakeupMotor_PID.SetTunings(spindletakeupMotor_PID_Kp,spindletakeupMotor_PID_Ki,spindletakeupMotor_PID_Kd);
    break;
//------------------------------------------------ 
    case SET_spindletakeupMotor_PID_Kd:
     spindletakeupMotor_PID_Kd = COMMAND_PARAMETER_DOUBLE;
     spindletakeupMotor_PID.SetTunings(spindletakeupMotor_PID_Kp,spindletakeupMotor_PID_Ki,spindletakeupMotor_PID_Kd);
    break;
//------------------------------------------------ 
    case SET_spindlesupplyMotor_PID_Kp:
      spindlesupplyMotor_PID_Kp = COMMAND_PARAMETER_DOUBLE;
      spindlesupplyMotor_PID.SetTunings(spindlesupplyMotor_PID_Kp,spindlesupplyMotor_PID_Ki,spindlesupplyMotor_PID_Kd);
    break;
//------------------------------------------------ 
    case SET_spindlesupplyMotor_PID_Ki:
      spindlesupplyMotor_PID_Ki = COMMAND_PARAMETER_DOUBLE;
      spindlesupplyMotor_PID.SetTunings(spindlesupplyMotor_PID_Kp,spindlesupplyMotor_PID_Ki,spindlesupplyMotor_PID_Kd);
     break;
//------------------------------------------------ 
    case SET_spindlesupplyMotor_PID_Kd:
      spindlesupplyMotor_PID_Kd = COMMAND_PARAMETER_DOUBLE;
      spindlesupplyMotor_PID.SetTunings(spindlesupplyMotor_PID_Kp,spindlesupplyMotor_PID_Ki,spindlesupplyMotor_PID_Kd);
    break;
//------------------------------------------------ 
    case SET_spindletakeupMotor_PID_Speed_Upper_limit:
      spindletakeupMotorSpeed_UPPER_LIMIT_INT = COMMAND_PARAMETER;
      spindletakeupMotorSpeed_UPPER_LIMIT = COMMAND_PARAMETER_DOUBLE;
      spindletakeupMotor_PID.SetOutputLimits(spindletakeupMotorSpeed_LOWER_LIMIT, spindletakeupMotorSpeed_UPPER_LIMIT);
    break;
//------------------------------------------------ 
    case SET_spindletakeupMotor_PID_Speed_Lower_limit:
    spindletakeupMotorSpeed_LOWER_LIMIT_INT = COMMAND_PARAMETER;
    spindletakeupMotorSpeed_LOWER_LIMIT = COMMAND_PARAMETER_DOUBLE;
    spindletakeupMotor_PID.SetOutputLimits(spindletakeupMotorSpeed_LOWER_LIMIT, spindletakeupMotorSpeed_UPPER_LIMIT);
    break;
//------------------------------------------------ 
    case SET_spindlesupplyMotor_PID_Speed_Upper_limit:
      spindlesupplyMotorSpeed_UPPER_LIMIT_INT = COMMAND_PARAMETER;
      spindlesupplyMotorSpeed_UPPER_LIMIT = COMMAND_PARAMETER_DOUBLE;
      spindlesupplyMotor_PID.SetOutputLimits(spindlesupplyMotorSpeed_LOWER_LIMIT, spindlesupplyMotorSpeed_UPPER_LIMIT);
    break;
//------------------------------------------------ 
    case SET_spindlesupplyMotor_PID_Speed_Lower_limit:
      spindlesupplyMotorSpeed_LOWER_LIMIT_INT = COMMAND_PARAMETER;
      spindlesupplyMotorSpeed_LOWER_LIMIT = COMMAND_PARAMETER_DOUBLE;
      spindlesupplyMotor_PID.SetOutputLimits(spindlesupplyMotorSpeed_LOWER_LIMIT, spindlesupplyMotorSpeed_UPPER_LIMIT);
    break;
    case spindletakeupMotor_PID_EEPROM_SAVE:
     EEPROM.writeDouble(spindletakeupMotor_PID_Kp_EEPROM_ADDRESS,spindletakeupMotor_PID_Kp);
     EEPROM.writeDouble(spindletakeupMotor_PID_Ki_EEPROM_ADDRESS,spindletakeupMotor_PID_Ki);
     EEPROM.writeDouble(spindletakeupMotor_PID_Kd_EEPROM_ADDRESS,spindletakeupMotor_PID_Kd);
     EEPROM.writeInt(spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND_EEPROM_ADDRESS,spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND);
     EEPROM.writeDouble(spindletakeupMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS,spindletakeupMotorSpeed_LOWER_LIMIT);
     EEPROM.writeDouble(spindletakeupMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS,spindletakeupMotorSpeed_UPPER_LIMIT);
     EEPROM.writeInt(spindletakeupMotor_closed_loop_DIRECTION_EEPROM_ADDRESS,spindletakeupMotor_closed_loop_DIRECTION);
    break;
    case spindlesupplyMotor_PID_EEPROM_SAVE:
     EEPROM.writeDouble(spindlesupplyMotor_PID_Kp_EEPROM_ADDRESS,spindlesupplyMotor_PID_Kp);
     EEPROM.writeDouble(spindlesupplyMotor_PID_Ki_EEPROM_ADDRESS,spindlesupplyMotor_PID_Ki);
     EEPROM.writeDouble(spindlesupplyMotor_PID_Kd_EEPROM_ADDRESS,spindlesupplyMotor_PID_Kd);
     EEPROM.writeDouble(spindlesupplyMotorSpeed_LOWER_LIMIT_EEPROM_ADDRESS,spindlesupplyMotorSpeed_LOWER_LIMIT);
     EEPROM.writeDouble(spindlesupplyMotorSpeed_UPPER_LIMIT_EEPROM_ADDRESS,spindlesupplyMotorSpeed_UPPER_LIMIT);
     EEPROM.writeInt(spindlesupplyMotor_closed_loop_DIRECTION_EEPROM_ADDRESS,spindlesupplyMotor_closed_loop_DIRECTION);
     EEPROM.writeDouble(tensionRoller_POT_VOLTAGE_SETPOINT_EEPROM_ADDRESS,tensionRoller_POT_VOLTAGE_SETPOINT);
   break;
    
// ==============================================
//              PARAMETER  RETRIEVAL
//================================================
/*
//------------------------------------------------ 
    case GET_spindlesupplyMotor_SPEED:
     Serial.print("#a");
     Serial.println(spindlesupplyMotorSpeed);
    break;
//------------------------------------------------ 
    case GET_spindletakeupMotor_SPEED:
     Serial.print("#b");
     Serial.println(spindletakeupMotorSpeed);
    break;
//------------------------------------------------ 
    case GET_pellonMotor1_SPEED:
     Serial.print("#b");
     Serial.println(pellonMotor1Speed);
    break;
//------------------------------------------------ 
    case GET_pellonMotor2_SPEED:
     Serial.print("#b");
     Serial.println(pellonMotor2Speed);
    break;
//------------------------------------------------ 
*/
  case GET_system_STATES:
     Serial.print(GET_spindlesupplyMotor_SPEED);
     Serial.print(spindlesupplyMotorSpeed);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindletakeupMotor_SPEED);
     Serial.print(spindletakeupMotorSpeed);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_pellonMotor1_SPEED);
     Serial.print(pellonMotor1Speed);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_pellonMotor2_SPEED);
     Serial.print(pellonMotor2Speed);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindletakeupMotor_PID_Kp);
     Serial.print(spindletakeupMotor_PID_Kp,4);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindletakeupMotor_PID_Ki);
     Serial.print(spindletakeupMotor_PID_Ki,4);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindletakeupMotor_PID_Kd);
     Serial.print(spindletakeupMotor_PID_Kd,4);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindlesupplyMotor_PID_Kp);
     Serial.print(spindlesupplyMotor_PID_Kp,4);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindlesupplyMotor_PID_Ki);
     Serial.print(spindlesupplyMotor_PID_Ki,4);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindlesupplyMotor_PID_Kd);
     Serial.print(spindlesupplyMotor_PID_Kd,4);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND);
     Serial.print(spindletakeupMotor_CLOSED_LOOP_SPEED_COMMAND);
     Serial.print("\n");
     delay(20);
     Serial.print(GET_tensionRoller_POT_VOLTAGE_SETPOINT);
     Serial.print(tensionRoller_POT_VOLTAGE_SETPOINT,2);
     Serial.print("\n");
      delay(20);
     Serial.print(GET_spindletakeupMotor_PID_Speed_Upper_limit);
     Serial.print(spindletakeupMotorSpeed_UPPER_LIMIT_INT); 
     Serial.print("\n");
      delay(20);
     Serial.print(GET_spindletakeupMotor_PID_Speed_Lower_limit);
     Serial.print(spindletakeupMotorSpeed_LOWER_LIMIT_INT);   
     Serial.print("\n");
      delay(20);
     Serial.print(GET_spindlesupplyMotor_PID_Speed_Upper_limit);
     Serial.print(spindlesupplyMotorSpeed_UPPER_LIMIT_INT);
     Serial.print("\n");
      delay(20);
     Serial.print(GET_spindlesupplyMotor_PID_Speed_Lower_limit);
     Serial.print(spindlesupplyMotorSpeed_LOWER_LIMIT_INT);
     Serial.print("\n");
      delay(20);
     Serial.print(GET_spindlesupplyMotor_closed_loop_DIRECTION);
     Serial.print(spindlesupplyMotor_closed_loop_DIRECTION);
     Serial.print("\n");
      delay(20);
     Serial.print(GET_spindletakeupMotor_closed_loop_DIRECTION);
     Serial.print(spindletakeupMotor_closed_loop_DIRECTION);
     Serial.print("\n");
      delay(20);
     Serial.print(Read_tensionRoller_POT_VOLTAGE);
     Serial.print(tensionRoller_POT_VOLTAGE_READING);
     Serial.print("\n");
      delay(20);
     Serial.print(Read_Tape_Speed_Opto);
     Serial.print(TAPE_SPEED_OPTO_STATE);
     Serial.print("\n");
      delay(20);
     Serial.print(GET_photointState1);
     Serial.print(photointState1);
     Serial.print("\n");
      delay(20);
     Serial.print(GET_photointState2);
     Serial.print(photointState2);
     Serial.print("\n");
    break;
    
  } // end switch (COMMAND)
    inputString = "";
    stringComplete = false;
    COMMAND = ' ';
 } // end if (stringComplete)
} // end loop

void serialEvent()

// SerialEvent occurs whenever a new data comes in the
// hardware serial RX.  This routine is run between each
// time loop() runs, so using delay inside loop can delay
// response.  Multiple bytes of data may be available.
 
{
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
     if (inChar >= '\n')
     inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
    {
      stringComplete = true;
    } 
  }
}// End void serialEvent()

// ------------------------ End Serial Port Interface ------------------------------

