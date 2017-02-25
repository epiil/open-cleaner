/* 
Opencleaner
20 February 2017
https://github.com/epiil/open-cleaner
*/
// ==============================================
//        LIBRARIES
//===============================================

#include <Wire.h> // Library for communicating with I2C devices
#include <Adafruit_MotorShield.h> // Library for Adafruit Motor Shield
#include <Adafruit_MCP23017.h> // Library for MCP23017 I2c Port Expander
#include <Adafruit_RGBLCDShield.h> // Adafruit RGB LCD Shield Library
#include <SimpleTimer.h> // SimpleTimer Library for measuring RPM
#include <digitalWriteFast.h> //digitalWriteFast Library
#include <PID_v1.h> // PID Library
#include <EEPROMex.h> // Library for writing to EEPROM
#include <EEPROMVar.h> // Library for writing to EEPROM

// ==============================================
//        ADAFRUIT MOTOR SHIELD v.2 - for PELLON MOTORS
//===============================================

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0X61); // Creates motor shield object with alternate 0x61 I2C address
Adafruit_DCMotor *pellonMotor1 = AFMS.getMotor(3); // assigns Supply-side Pellon motor to M3 on the Adafruit Motor Shield
Adafruit_DCMotor *pellonMotor2 = AFMS.getMotor(4); // assigns Takeup-side Pellon motor to M4 on the Adafruit Motor Shield

Adafruit_DCMotor *testmotor = AFMS.getMotor(1); // 

// Variables

int pellonMotorSpeed = 100; // Initial pellon motor speed (100/255)

// ==============================================
//        CYTRON MDD10A DC MOTOR DRIVER - for SPINDLE MOTORS
//===============================================

#define spindlesupplyMotor_DIR1 8   // Assigns DIR1 on MDD10A to Digital Pin 8
#define spindlesupplyMotor_PWM1 9   // Assigns PWM1 on MDD10A to Digital Pin 9  (PWM pin)
#define spindletakeupMotor_PWM2 10  // Assigns PWM2 on MDD10A to Digital Pin 10 (PWM pin)
#define spindletakeupMotor_DIR2 11  // Assigns DIR2 on MDD10A to Digital Pin 11

// Variables

int supplyMotorSpeed = 100; // Initial supply spindle motor speed (100/255)
int spindlesupplyMotor_PWM = 100; // PWM value for supply spindle motor


int takeupMotorSpeed = 100; // Initial takeup spindle motor speed (100/255)
int spindletakeupMotor_PWM = 100; // PWM value for takeup spindle motor

int spindlesupplyMotor_DIR = 0;
int spindletakeupMotor_DIR = 0; 

boolean spindlesupplyMotor_RUN_FLAG = false;
boolean spindletakeupMotor_RUN_FLAG = false;

// ==============================================
//        PITTMAN E30 INCREMENTAL OPTICAL ENCODERS - 512 counts per resolution (CPR)
//===============================================

#define encoder0PinA  2 // Assigns Takeup Spindle Motor Optical Encoder Channel A to Arduino Digital Pin 2
//#define encoder0PinB  3 // Assigns Takeup Spindle Motor Optical Encoder Channel B to Arduino Digital Pin 5

#define spindlesupplyOpticalEncoderPinA 2
#define spindlesupplyOpticalEncoderIsReversed
volatile bool spindlesupplyOpticalEncoderASet;
volatile long spindlesupplyOpticalEncoderCount = 0;

unsigned int spindlesupplymotorSpeedRPM=0;                      //    Calculated motor speed from encoder value.
SimpleTimer timer;                                             //     Timer object for polling counter value every 1 sec.
const long updateInterval=1000L;                              //      Timer update value (1000 ms)
const float pulsesPerRevolution=256*2.0;                     //       Pulses per revolution (512 CPR)


// ==============================================
//        OPTEK OPB819Z SLOTTED OPTICAL SWITCH for TAPESENSORREAD - 64 pulses per revolution (PPM)
//===============================================

int optotapeSensor = 5; //Assigns Opto Switch (tape sensor) collector to Arduino Digital Pin 5

// ==============================================
//        BOURNS EM14 ROTARY OPTICAL ENCODER (aka TENSION ENCODER)
//===============================================

int val; 
int tensionencoder0PinA = 6;          // Stores rotary optical encoder Channel A as Pin 6
int tensionencoder0PinB = 7;          // Stores rotary optical encoder Channel B as Pin 7
int tensionencoder0Pos = 0;           // Rotary optical encoder position at 0
int tensionencoder0PinALast = LOW;
int n = LOW;

// ==============================================
//        ADAFRUIT RGB LCD KEYPAD 
//===============================================

#define LED 13 // Adafruit RGB LCD keypad
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield(); // LCD shield instantiation, creates LCD shield object
#define WHITE 0x7 // Sets background color (for RGB keypads)
#define BUTTON_SHIFT BUTTON_SELECT 

// Variables
unsigned long lastInput = 0; // last button press
const int logInterval = 10000; // log every 10 seconds
long lastLogTime = 0;

// ==============================================
//        OPERATING STATES for LCD KEYPAD MENU
//===============================================

enum operatingState { OFFSTATUS = 0, RUNF, RUNB, CLOSEDLOOP, PELRUN, MENUSUPPLYSPEED, MENUTAKEUPSPEED, TUNE_P, TUNE_I, TUNE_D, OPTOREAD, TENSIONREAD, TAPESENSORREAD};
operatingState opState = OFFSTATUS;

boolean TAPE_SPEED_OPTO_STATE = false;
boolean LAST_TAPE_SPEED_OPTO_STATE = false;         // Used for debouncing

float spindletakeupMotorSpeed_FLOAT = 0.0;
float spindlesupplyMotorSpeed_FLOAT = 0.0;

int spindletakeupMotorSpeed = 0;
int spindlesupplyMotorSpeed = 0;


// ==============================================
//        PID VARIABLES AND CONSTANTS
//===============================================

double Setpoint, Input, Output;

double spindlesupplyMotor_PID_Kp = 0.05;
double spindlesupplyMotor_PID_Ki = 0.1;
double spindlesupplyMotor_PID_Kd = 0.05;

PID myPID(&Input, &Output, &Setpoint, spindlesupplyMotor_PID_Kp, spindlesupplyMotor_PID_Ki, spindlesupplyMotor_PID_Kd, DIRECT);

// ==============================================
//        FUNCTION: UPDATE SPEED EVENT
//===============================================

void updateSpeedEvent()
{
  spindlesupplymotorSpeedRPM=((spindlesupplyOpticalEncoderCount/(updateInterval/1000.0))*60.0)/pulsesPerRevolution; //RPM speed
  spindlesupplyOpticalEncoderCount=0; //reset counter;
} // =========== End updateSpeedEvent Function =============


// ==============================================
//        FUNCTION: MC3886 MOTOR DRIVER - SPEED
//===============================================

void spindlesupplyMotor_SET_SPEED(int SPEED)
{
  spindlesupplyMotor_PWM = SPEED;
  if(spindlesupplyMotor_RUN_FLAG)
  switch (spindlesupplyMotor_DIR)
  {
//    case FORWARD:
//      spindlesupplyMotor_RUN_FLAG = true;
//      digitalWrite(spindlesupplyMotor_DIR1,HIGH); // writes 0/1023 duty to spindletakeupMotor_DIR1 pwm pin
//      analogWrite(spindlesupplyMotor_PWM1,SPEED); // Configures spindletakeupMotor_PWM1 pwm pin, duty SPEED
//    break;
    case BACKWARD:
      spindlesupplyMotor_RUN_FLAG = true;
      digitalWrite(spindlesupplyMotor_DIR1,LOW); // writes 0/1023 duty to spindletakeupMotor_PWM1 pwm pin
      analogWrite(spindlesupplyMotor_PWM1,SPEED); // Configures spindletakeupMotor_PWM12 pwm pin, duty SPEED
    break;
  }
} // =========== End MC3886 Motor Driver Supply Set Speed Function =============

void spindletakeupMotor_SET_SPEED(int SPEED)
{
  spindletakeupMotor_PWM = SPEED;
  if(spindletakeupMotor_RUN_FLAG)
  switch (spindletakeupMotor_DIR)
  {
    case FORWARD:
      spindletakeupMotor_RUN_FLAG = true;
      digitalWrite(spindletakeupMotor_DIR2,HIGH); // writes 1023/1023 duty to spindletakeupMotor_DIR2 pin
      analogWrite(spindletakeupMotor_PWM2,SPEED); // Configures spindletakeupMotor_PWM1 pwm pin, duty SPEED
    break;
//    case BACKWARD:
//      spindletakeupMotor_RUN_FLAG = true;
//      digitalWrite(spindletakeupMotor_DIR2,LOW); // writes 0/1023 duty to spindletakeupMotor_DIR2
//      analogWrite(spindletakeupMotor_PWM2,SPEED); // Configures spindletakeupMotor_PWM12 pwm pin, duty SPEED
//    break;
  }
} // =========== End MC3886 Motor Driver Takeup Set Speed Function =============

// ==============================================
//        FUNCTION: MC3886 MOTOR DRIVER - DIRECTION
//===============================================

void spindletakeupMotor_RUN(int DIRECTION)
{
  spindletakeupMotor_DIR = DIRECTION;
  switch (DIRECTION)
  {
    case FORWARD:
      spindletakeupMotor_RUN_FLAG = true; 
      digitalWrite(spindletakeupMotor_DIR2,HIGH);
      analogWrite(spindletakeupMotor_PWM2, spindletakeupMotor_PWM);
    break;
//    case BACKWARD:
//      spindletakeupMotor_RUN_FLAG = true;
//      digitalWrite(spindletakeupMotor_DIR2,LOW);
//      analogWrite(spindletakeupMotor_PWM2, spindletakeupMotor_PWM); 
//    break;
    case BRAKE:
      spindletakeupMotor_RUN_FLAG = false;
      digitalWrite(spindletakeupMotor_PWM2,LOW);
      digitalWrite(spindletakeupMotor_DIR2, LOW);
    break;
    case RELEASE: 
      spindletakeupMotor_RUN_FLAG = false;
      digitalWrite(spindletakeupMotor_PWM2,LOW);
      digitalWrite(spindletakeupMotor_DIR2, LOW);
    break;
  }
} // =========== End MC3886 Motor Driver Takeup Direction Function =============

void spindlesupplyMotor_RUN(int DIRECTION)
{
  spindlesupplyMotor_DIR = DIRECTION;
  switch (DIRECTION)
  {
//    case FORWARD:
//      spindlesupplyMotor_RUN_FLAG = true;
//      digitalWrite(spindlesupplyMotor_DIR1,HIGH);
//      analogWrite(spindlesupplyMotor_PWM1, spindlesupplyMotor_PWM); 
//    break;
    case BACKWARD:
      spindlesupplyMotor_RUN_FLAG = true;
      digitalWrite(spindlesupplyMotor_DIR1,LOW);
      analogWrite(spindlesupplyMotor_PWM1, spindlesupplyMotor_PWM); 
    break;
    case BRAKE:
      spindlesupplyMotor_RUN_FLAG = false;
      digitalWrite(spindlesupplyMotor_PWM1,LOW);
      digitalWrite(spindlesupplyMotor_DIR1,LOW);
    break;
    case RELEASE: 
      spindlesupplyMotor_RUN_FLAG = false;
      digitalWrite(spindlesupplyMotor_PWM1,LOW);
      digitalWrite(spindlesupplyMotor_DIR1,LOW);
    break;
  }
} // // =========== End MC3886 Motor Driver Supply Direction Function =============

// ==============================================
//        MAIN SETUP FUNCTION
//===============================================

void setup() {
  
  Serial.begin (9600); 
 
  //pinMode(encoder0PinB, INPUT);                                     // Configures takeup motor encoder channel B pin as input

  pinMode(spindlesupplyOpticalEncoderPinA, INPUT);                    // sets pin A as input
  digitalWrite(spindlesupplyOpticalEncoderPinA, LOW);                 // turn on pullup resistors
  attachInterrupt(0, spindlesupplyOpticalEncoderInterruptA, RISING);
  timer.setInterval(updateInterval, updateSpeedEvent);                // Interrupt for accurate counting of encoder pulses

// Opening Display with version control
   lcd.begin(16, 2);
   lcd.print(F("Opencleaner v0.9"));
   delay(900);

   pinMode(LED,OUTPUT); // Configures LED pin as output
   pinMode(spindlesupplyMotor_PWM1,OUTPUT); // Configures supply motor pin as output
   pinMode(spindlesupplyMotor_DIR1,OUTPUT); // Configures supply motor pin as output
   pinMode(spindletakeupMotor_PWM2,OUTPUT); // Configures takeup motor pin as output
   pinMode(spindletakeupMotor_DIR2,OUTPUT); // Configures takeup motor pin as output

   pinMode(tensionencoder0PinA,INPUT); // Configures tension rotary encoder channel A as input
   pinMode(tensionencoder0PinB,INPUT); // Configures tension rotary encoder channel B as input

   AFMS.begin();  // Sets default frequency at 1.6KHz
   TCCR1B = (TCCR1B & 0b11111000) | 1;  // Adjust PWM frequency of PWM Pins 9 & 10 to 31372.55 Hz (31 kHz), removes audible PWM squeel

   pellonMotorSpeed = 100; // Intial spindle motor speed at 100/255
   pellonMotor1->setSpeed(pellonMotorSpeed); // Sets initial Supply motor to 100/255
   pellonMotor2->setSpeed(pellonMotorSpeed); // Sets initial Takeup motor to 100/255
   pellonMotor1->run(RELEASE); // Turns pellon 1 motor off
   pellonMotor2->run(RELEASE); // Turns pellon 2 motor off

   testmotor->setSpeed(30); // Sets initial Takeup motor to 100/255

   //PID

   Input = spindlesupplymotorSpeedRPM;
    Setpoint = 170; // 100=100/255=39% power / rpm for dc motor
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(20, (int) 220);

   //Read Parameters from EEPROM

   #define spindlesupplyMotor_PID_Kp_EEPROM_ADDRESS 0
   #define spindlesupplyMotor_PID_Ki_EEPROM_ADDRESS 4
   #define spindlesupplyMotor_PID_Kd_EEPROM_ADDRESS 8

   EEPROM.writeDouble(spindlesupplyMotor_PID_Kp_EEPROM_ADDRESS,0.5);
   EEPROM.writeDouble(spindlesupplyMotor_PID_Ki_EEPROM_ADDRESS,0.1);
   EEPROM.writeDouble(spindlesupplyMotor_PID_Kd_EEPROM_ADDRESS,0.1);

   spindlesupplyMotor_PID_Kp = EEPROM.readDouble(spindlesupplyMotor_PID_Kp_EEPROM_ADDRESS);
   spindlesupplyMotor_PID_Ki = EEPROM.readDouble(spindlesupplyMotor_PID_Ki_EEPROM_ADDRESS);
   spindlesupplyMotor_PID_Kd = EEPROM.readDouble(spindlesupplyMotor_PID_Kd_EEPROM_ADDRESS);

} // =========== End Setup Function =============

// ==============================================
//        MAIN LOOP FUNCTION
//===============================================

void loop() { 
  while(ReadButtons() != 0) {}
  lcd.clear();
  switch (opState) {
    case OFFSTATUS:
    OffStatus();
    break;
    case RUNF: // Menu: Run tape forward
    RunF();
    break;
    case RUNB:// Menu: Run tape backward
    RunB();
    break;
    case CLOSEDLOOP: // Menu: Run Closed Loop
    ClosedLoop();
    break;
    case PELRUN: // Menu: Run Pellon Motors Forward, Set Pellon Motor Speed
    PelRun();
    break;
    case MENUSUPPLYSPEED: // Menu: Set Supply Spindle Speed
    MenuSupplySpeed();
    break;
    case MENUTAKEUPSPEED: // Menu: Set Menu Takeup Speed
    MenuTakeUpSpeed();
    break;
    case TUNE_P: // Menu: Tune Proportional settings of PID loop
    TuneP();
    break;
    case TUNE_I: // Menu: Tune Integral settings of PID loop
    TuneI();
    break;
    case TUNE_D: // Menu: Tune Damping/Derivative settings of PID loop
    TuneD();
    break;
    case OPTOREAD: // Menu: Read tape speed optical encoder
    OptoRead();
    break;
    case TENSIONREAD: // Menu: Read tension encoder
    TensionRead();
    break;
    case TAPESENSORREAD: // Menu: Read tape tension opto switch
    TapeSensorRead();
    break;
    timer.run(); // Initiates SimpleTimer
   }
} // =========== End Loop Function =============

// ==============================================
//        READBUTTONS - All keypad buttons are read at once, returns a variable that has individual bits set for the buttons
//===============================================
uint8_t ReadButtons()
{
  uint8_t buttons = lcd.readButtons();
  if (buttons != 0)
  {
    lastInput = millis();
  }
  return buttons;
} // =========== End ReadButtons Function =============

// ===============================================
// OFF STATUS
// SHIFT and RIGHT for autotune
// RIGHT - Setpoint
// LEFT - TENSIONREAD
// ===============================================
void OffStatus()
{
  lcd.print(F("  Operations:"));
  lcd.setCursor(0, 1);
  lcd.print(F("< Back    Menu >"));
  uint8_t buttons = 0;
  while(true)
  {
      buttons = ReadButtons();
      if (buttons & BUTTON_SHIFT)
      {
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = TAPESENSORREAD;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = RUNF;
         return;
      }
  }
} // =========== End OffStatus Function =============

// ===============================================
// Run Tape in Forward (Supply to Takeup) aka RUNF
// RIGHT button: for RUNB
// LEFT button: for SETP
// SHIFT button: Run Tape in Forward Mode
// ===============================================
void RunF()
{
  lcd.print(F("1.Clean Forward"));
  lcd.setCursor(0,1);
  lcd.print(F("RPM:"));
  uint8_t buttons = 0;
  while(true)
  {
      buttons = ReadButtons();
      
      if (buttons & BUTTON_SHIFT)
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE);
        pellonMotor2->run(RELEASE);
        lcd.setCursor(13,1);
        lcd.print(F("STP"));
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = OFFSTATUS;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = RUNB;
         return;
      }
      if (buttons & BUTTON_UP)
      { 
        spindletakeupMotor_SET_SPEED(takeupMotorSpeed);
        spindletakeupMotor_RUN(FORWARD);
        pellonMotor1->run(FORWARD);
        pellonMotor2->run(FORWARD);
        lcd.setCursor(13,1);
        lcd.print(F("FWD"));
      }
      if (buttons & BUTTON_DOWN)
      {
      }
      if  (digitalRead(optotapeSensor) == 0)  //Opto switch beam is seen
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE);
        pellonMotor2->run(RELEASE);
      }
      else
      {
      }
      timer.run(); // Initiates SimpleTimer
      Serial.print("supply motor speed (RPM) = ");
      Serial.print(spindlesupplymotorSpeedRPM);
      Serial.print("\n");
      lcd.setCursor(5,1);
      lcd.print(spindlesupplymotorSpeedRPM);
      delay(20);
  }
} // =========== End RunF Function =============

// ===============================================
// Run Tape in Reverse aka RUNB
// RIGHT button: for PELRUN
// LEFT button: for RUNF
// SHIFT button: - Run Tape in Reverse
// ===============================================
void RunB()
{
  lcd.print(F("2.Clean(Reverse)"));
  lcd.setCursor(0,1);
  lcd.print(F("UP to RUN"));
  uint8_t buttons = 0;
  while(true)
  {
      buttons = ReadButtons();
      
      if (buttons & BUTTON_SHIFT)
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE);
        pellonMotor2->run(RELEASE);
        lcd.setCursor(13,1);
        lcd.print(F("STP"));
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = RUNF;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = PELRUN;
         return;
      }
      if (buttons & BUTTON_UP)
      {
        opState = CLOSEDLOOP;
         return;
        /*//analogWrite(spindlesupplyMotor_PWM1, 10);
        //spindletakeupMotor_RUN(FORWARD);
        //analogWrite(spindletakeupMotor_PWM2, 10);
        delay(7000);
        spindlesupplyMotor_SET_SPEED(155);
        spindlesupplyMotor_RUN(BACKWARD);
        analogWrite(spindletakeupMotor_PWM2, 5);
        spindletakeupMotor_RUN(FORWARD);
        pellonMotor1->run(FORWARD);
        pellonMotor2->run(FORWARD);
        lcd.setCursor(13,1);
        lcd.print(F("REV"));*/
        
      }
      if (buttons & BUTTON_DOWN)
      {
      }
      if  (digitalRead(optotapeSensor) == 0)  //Opto switch beam is seen
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE);
        pellonMotor2->run(RELEASE);
      }
      else
      {
      }
  }
} // =========== End RunB Function =============





// ===============================================
// BACKWARDS CLOSED LOOP STATUS aka CLOSEDLOOP
// SHIFT and RIGHT for autotune
// RIGHT - Setpoint
// LEFT - TENSIONREAD
// ===============================================
void ClosedLoop()
{
  lcd.print(F("2.Clean(Reverse)"));
  lcd.setCursor(0,1);
  lcd.print(F("RPM:"));
  uint8_t buttons = 0;
  while(true)
  {
      buttons = ReadButtons();
      if (buttons & BUTTON_SHIFT)
      {
        testmotor->setSpeed(0); // Turns pellon 2 motor off
        testmotor->run(RELEASE); // Turns pellon 2 motor off
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE); // Turns pellon 1 motor off
        pellonMotor2->run(RELEASE); // Turns pellon 2 motor off
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = RUNF;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = PELRUN;
         return;
      }
      testmotor->run(FORWARD); // Turns pellon 2 motor off
      timer.run(); // Initiates SimpleTimer
      Serial.print("supply motor speed (RPM) = ");
      Serial.print(spindlesupplymotorSpeedRPM);
      Serial.print("\n");
      lcd.setCursor(5,1);
      lcd.print(spindlesupplymotorSpeedRPM);
      lcd.print("    ");
      if  (digitalRead(optotapeSensor) == 0)  //Opto switch beam is seen
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE); // Turns pellon 1 motor off
        pellonMotor2->run(RELEASE); // Turns pellon 2 motor off
        testmotor->setSpeed(0); // Turns pellon 2 motor off
        testmotor->run(RELEASE); // Turns pellon 2 motor off
      }
      else
      {
        pellonMotor1->run(FORWARD);
        pellonMotor2->run(FORWARD);
        runClosedLoop();
      } 
      delay(20);
      
  }
} // =========== End ClosedLoop Function =============


































// ===============================================
// MENU: Run Pellon aka PELRUN
// RIGHT button: for MENUSUPPLYSPEED
// LEFT button: for RUNB
// SHIFT button: - Run Pellon
// ===============================================
void PelRun()
{
  lcd.print(F("3.Pellon"));
  lcd.setCursor(12,0);
  lcd.print(F("/255"));
  uint8_t buttons = 0;
  while(true)
  {
      buttons = ReadButtons();
      float increment = 5.0;
      if (buttons & BUTTON_SHIFT)
      {
      pellonMotor1->run(RELEASE);
      pellonMotor2->run(RELEASE);
      lcd.setCursor(13,1);
      lcd.print(F("STP"));
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = RUNB;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = MENUSUPPLYSPEED;
         return;
      }
      if (buttons & BUTTON_UP)
      {
        pellonMotorSpeed += increment;
        delay(200);
        pellonMotor1->setSpeed(pellonMotorSpeed);
        pellonMotor2->setSpeed(pellonMotorSpeed);
        pellonMotor1->run(FORWARD);
        pellonMotor2->run(FORWARD);
        lcd.setCursor(9,0);
        lcd.print("   ");
        lcd.setCursor(9,0);
        lcd.print(pellonMotorSpeed);
      }
      if (buttons & BUTTON_DOWN)
      {
        pellonMotorSpeed -= increment;
        delay(200);
        pellonMotor1->setSpeed(pellonMotorSpeed);
        pellonMotor2->setSpeed(pellonMotorSpeed);
        pellonMotor1->run(FORWARD);
        pellonMotor2->run(FORWARD);
        lcd.setCursor(9,0);
        lcd.print("   ");
        lcd.setCursor(9,0);
        lcd.print(pellonMotorSpeed);
      }
  }
} // =========== End PelRun Function =============

// ===============================================
// MENU: Set Supply Spindle Speed aka MENUSUPPLYSPEED
// UP/DOWN button: Change Speed (Increments 0-255)
// RIGHT button: MENUTAKEUPSPEED
// LEFT button: PELRUN
// SHIFT button: Turn Motor On
// ===============================================
void MenuSupplySpeed()
{
  lcd.print(F("4.Supply"));
  lcd.setCursor(12,0);
  lcd.print(F("/255"));
  lcd.setCursor(0,1);
  lcd.print(F("Dir:"));
  lcd.setCursor(8,1);
  lcd.print(F("RPM:"));
  lcd.setCursor(9,0);
  lcd.print(supplyMotorSpeed);
  lcd.setCursor(12,1);
  lcd.print(spindlesupplymotorSpeedRPM);
  uint8_t buttons = 0;
  while(true)
  {
    buttons = ReadButtons();
    
      float increment = 5.0;
      if (buttons & BUTTON_SHIFT)
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        lcd.setCursor(4,1);
        lcd.print(F("STP"));
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = PELRUN;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = MENUTAKEUPSPEED;
         return;
      }
      if (buttons & BUTTON_UP)
      {
        supplyMotorSpeed += increment;
        delay(200);
        analogWrite(spindlesupplyMotor_PWM1, supplyMotorSpeed);
        lcd.setCursor(9,0);
        lcd.print("   ");
        lcd.setCursor(9,0);
        lcd.print(supplyMotorSpeed);
        lcd.setCursor(4,1);
        lcd.print(F("REV"));
      }
      if (buttons & BUTTON_DOWN)
      {
        supplyMotorSpeed -= increment;
        delay(200);
        analogWrite(spindlesupplyMotor_PWM1, supplyMotorSpeed);
        lcd.setCursor(9,0);
        lcd.print("   ");
        lcd.setCursor(9,0);
        lcd.print(supplyMotorSpeed);
        lcd.setCursor(4,1);
        lcd.print(F("REV"));
      }
      if  (digitalRead(optotapeSensor) == 0)  //Opto switch beam is seen
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE); // Turns pellon 1 motor off
        pellonMotor2->run(RELEASE); // Turns pellon 2 motor off
      }
      else
      {
      } 
      timer.run(); // Initiates SimpleTimer
      Serial.print("supply motor speed (RPM) = ");
      Serial.print(spindlesupplymotorSpeedRPM);
      Serial.print("\n");
      lcd.setCursor(12,1);
      lcd.print(spindlesupplymotorSpeedRPM);
      lcd.print("    ");
  }
} // =========== End MenuSupplySpeed Function =============

// ===============================================
// MENU: Set Takeup Spindle Speed aka MENUTAKEUPSPEED
// UP/DOWN: Change Speed (Increments 0-255)
// RIGHT button: TUNE_P
// LEFTbutton: MENUSUPPLYSPEED
// SHIFT button: Stop Motor
// ===============================================
void MenuTakeUpSpeed()
{
  lcd.print(F("5.Takeup"));
  lcd.setCursor(0,1);
  lcd.print(F("Spd:"));
  lcd.setCursor(12,0);
  lcd.print(F("/255"));
  lcd.setCursor(0,1);
  lcd.print(F("Dir:"));
  lcd.setCursor(8,1);
  lcd.print(F("RPM:"));
  lcd.setCursor(9,0);
  lcd.print(supplyMotorSpeed);
  lcd.setCursor(12,1);
  lcd.print(spindlesupplymotorSpeedRPM);
  uint8_t buttons = 0;
  while(true)
  {
    buttons = ReadButtons();
    
      float increment = 5.0;
      if (buttons & BUTTON_SHIFT)
      {
        lcd.setCursor(4,1);
        lcd.print(F("STP"));
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = MENUSUPPLYSPEED;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = TUNE_P;
         return;
      }
      if (buttons & BUTTON_UP)
      {
        takeupMotorSpeed += increment;
        delay(200);
        analogWrite(spindletakeupMotor_PWM2, takeupMotorSpeed);
        lcd.setCursor(9,0);
        lcd.print("   ");
        lcd.setCursor(9,0);
        lcd.print(takeupMotorSpeed);
        lcd.setCursor(4,1);
        lcd.print(F("FWD"));
      }
      if (buttons & BUTTON_DOWN)
      {
        takeupMotorSpeed -= increment;
        delay(200);
        analogWrite(spindletakeupMotor_PWM2, takeupMotorSpeed);
        lcd.setCursor(9,0);
        lcd.print("   ");
        lcd.setCursor(9,0);
        lcd.print(takeupMotorSpeed);
        lcd.setCursor(4,1);
        lcd.print(F("FWD"));
      }
      if  (digitalRead(optotapeSensor) == 0)  //Opto switch beam is seen
      {
        spindlesupplyMotor_RUN(BRAKE);
        spindletakeupMotor_RUN(BRAKE);
        pellonMotor1->run(RELEASE); // Turns pellon 1 motor off
        pellonMotor2->run(RELEASE); // Turns pellon 2 motor off
      }
      else
      {
      }
  }
} // =========== End MenuTakeUpSpeed Function =============

// ===============================================
// MENU: Proportional Tuning State aka TUNE_P
// UP/DOWN button: to change Kp
// RIGHT button: for TUNE_I
// LEFT button: for MENUTAKEUPSPEED
// SHIFT button: for 10x tuning
// ===============================================

void TuneP()
{
   lcd.print(F("Config: Set Kp"));
   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

      float increment = 1.0;
      if (buttons & BUTTON_SHIFT)
      {
        increment *= 10;
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = MENUTAKEUPSPEED;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = TUNE_I;
         return;
      }
      if (buttons & BUTTON_UP)
      {
         spindlesupplyMotor_PID_Kp += increment;
         delay(200);
      }
      if (buttons & BUTTON_DOWN)
      {
         spindlesupplyMotor_PID_Kp -= increment;
         delay(200);
      }
      lcd.setCursor(0,1);
      lcd.print(spindlesupplyMotor_PID_Kp);
      lcd.print("   ");
   }
} // =========== End TuneP Function =============
// ===============================================
// Integral Tuning State aka TUNE_I
// UP/DOWN to change Ki
// RIGHT for TUNE_D
// LEFT for TUNE_P
// SHIFT for 10x tuning
// ===============================================
void TuneI()
{
   lcd.print(F("Config: Set Ki"));

   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

      float increment = 0.01;
      if (buttons & BUTTON_SHIFT)
      {
        increment *= 10;
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = TUNE_P;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = TUNE_D;
         return;
      }
      if (buttons & BUTTON_UP)
      {
         spindlesupplyMotor_PID_Ki += increment;
         delay(200);
      }
      if (buttons & BUTTON_DOWN)
      {
         spindlesupplyMotor_PID_Ki -= increment;
         delay(200);
      }
      lcd.setCursor(0,1);
      lcd.print(spindlesupplyMotor_PID_Ki);
      lcd.print("   ");
   }
} // =========== End TuneI Function =============

// ===============================================
// Derivative Tuning State aka TUNE_D
// UP/DOWN button: to change Kd
// RIGHT button: for OPTOREAD
// LEFT button: for TUNE_K
// SHIFT button: for 10x tuning
// ===============================================
void TuneD()
{
   lcd.print(F("Config: Set Kd"));

   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();
      float increment = 0.01;
      if (buttons & BUTTON_SHIFT)
      {
        increment *= 10;
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = TUNE_I;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = OPTOREAD;
         return;
      }
      if (buttons & BUTTON_UP)
      {
         spindlesupplyMotor_PID_Kd += increment;
         delay(200);
      }
      if (buttons & BUTTON_DOWN)
      {
         spindlesupplyMotor_PID_Kd -= increment;
         delay(200);
      }
      lcd.setCursor(0,1);
      lcd.print(spindlesupplyMotor_PID_Kd);
      lcd.print("   ");
   }
} // =========== End TuneD Function =============

// ===============================================
// MENU: Config: Read Speed Opto aka OPTOREAD
// RIGHT button: TENSIONREAD
// LEFT button: TUNE_D
// ===============================================
void OptoRead()
{
  lcd.print(F("Config: Encoder"));
  lcd.setCursor(0,1);
  lcd.print(F("RPM:"));
  uint8_t buttons = 0;
  while(true)
  {
      buttons = ReadButtons();
      
      if (buttons & BUTTON_SHIFT)
      {
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = TUNE_D;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = TENSIONREAD;
         return;
      }
      if (buttons & BUTTON_UP)
      {
      }
      if (buttons & BUTTON_DOWN)
      {
      }
      timer.run(); // Initiates SimpleTimer
      lcd.setCursor(4,1);
      lcd.print(spindlesupplymotorSpeedRPM);
      lcd.print("    ");
  }
} // =========== End OptoRead Function =============

// ===============================================
// MENU: Config: Read Tension Encoder aka TENSIONREAD
// RIGHT button: RUNSTATUS
// LEFT button: TUNE_D
// ===============================================
void TensionRead()
{
  readtapetension();
  lcd.print(F("Config: Tension"));
  uint8_t buttons = 0;
  while(true)   
  {
    buttons = ReadButtons();   
    if (buttons & BUTTON_SHIFT)
      {
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = OPTOREAD;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = TAPESENSORREAD;
         return;
      }
      if (buttons & BUTTON_UP)
      {
      }
      if (buttons & BUTTON_DOWN)
      {
      }
      readtapetension();
  }
} // =========== End TensionRead Function =============

// ===============================================
// MENU: Config: Read Tape Sensor aka TAPESENSORREAD
// RIGHT button: OFFSTATUS
// LEFT button: TENSIONREAD
// ===============================================
void TapeSensorRead()
{
  lcd.print(F("Conf: Tape Sensor"));

  uint8_t buttons = 0;
  while(true)   
  {
    buttons = ReadButtons();   
    if (buttons & BUTTON_SHIFT)
      {
      }
      if (buttons & BUTTON_LEFT)
      {
         opState = TENSIONREAD;
         return;
      }
      if (buttons & BUTTON_RIGHT)
      {
         opState = OFFSTATUS;
         return;
      }
      if (buttons & BUTTON_UP)
      {
      }
      if (buttons & BUTTON_DOWN)
      {
      }
      if  (digitalRead(optotapeSensor) == 0)
      {
        Serial.write("NO TAPE PRESENT");
        lcd.setCursor(0,1);
        lcd.print("NO TAPE PRESENT");
        }
        else 
        {
          Serial.write("TAPE");
          lcd.setCursor(0,1);
          lcd.print("TAPE PRESENT   ");
        }  
        delay(20);
    }
} // =========== End TapeSensorRead Function =============

// ==============================================
//        FUNCTION - READ OPTO TAPE SENSOR
//===============================================

void readOptoTapeSensor() 
{
  if  (digitalRead(optotapeSensor) == 0)  //Opto switch beam is seen
  {
    //Serial.write("NO TAPE PRESENT");
    spindlesupplyMotor_RUN(BRAKE);
    spindletakeupMotor_RUN(BRAKE);// now turn off motors
  }
  else 
  {
    //Serial.write("TAPE PRESENT");
  }  
//delay(20);
} // =========== End readOptoTapeSensor Function =============

// ==============================================
//        FUNCTION - READ TAPE TENSION
//===============================================

void readtapetension() {
  n = digitalRead(tensionencoder0PinA);
  if ((tensionencoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(tensionencoder0PinB) == LOW) {
       tensionencoder0Pos--;
       } else {
       tensionencoder0Pos++;
       }
       Serial.print (tensionencoder0Pos);
       Serial.print ("/");
       lcd.setCursor(0,1);
       lcd.print(tensionencoder0Pos);
       } 
       tensionencoder0PinALast = n;
} // =========== End readtapetension Function =============

// ==============================================
//        FUNCTION - SUPPLY OPTICAL ENCODER INTERRUPT 1
//===============================================

void spindlesupplyOpticalEncoderInterruptA()
{
    spindlesupplyOpticalEncoderCount += spindlesupplyOpticalEncoderASet ? -1 : +1;
} // =========== End spindlesupplyOpticalEncoderInterruptA Function =============

// ==============================================
//        FUNCTION - RUN CLOSED LOOP (SUPPLY)
//===============================================

void runClosedLoop() {
   Input=map(spindlesupplymotorSpeedRPM,0,4000,0,255); // Change 1500 to your max. rpm
   myPID.Compute();
   analogWrite(spindlesupplyMotor_PWM1, Output);
} // =========== End runClosedLoop Function =============



