#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 2);

//Photoop

int sensorPin = 0;
int ledPin = 7;
int sensorValue = 0;

void setup() {
	
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  lcd.print("FORWARD"); 
  
  //Motor
  //Setup Channel A
  //pinMode(12, OUTPUT); //Initiates Motor Channel A pin
 // pinMode(9, OUTPUT); //Initiates Brake Channel A pin
 // pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  //pinMode(8, OUTPUT); //Initiates Brake Channel B pin
}

void loop() {
	
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print("transistor activating");
  
  //forward @ full speed
  //digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  //digitalWrite(9, LOW);   //Disengages the Brake for Channel A
 // analogWrite(3, 80);   //Spins the motor on Channel A at full speed
  
  //digitalWrite(13, HIGH); //Establishes backward direction of Channel A
  //digitalWrite(8, LOW);   //Disengage the Brake for Channel A
 // analogWrite(11, 200);   //Spins the motor on Channel A at half speed
}

