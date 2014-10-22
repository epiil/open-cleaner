#include <Servo.h> 
 
Servo pellonservo1;  // create pellonservo1 object 
Servo pellonservo2;  // create pellonservo2 object
                
int pos = 0;    // variable to store servo position 

#include <AFMotor.h>

AF_DCMotor motorsupply(1);
AF_DCMotor motortakeup(2);
 
void setup() 
{ 
  pellonservo1.attach(9);  // attaches the servo on pin 9 to the servo object 
  pellonservo2.attach(10);  // attaches the servo on pin 9 to the servo object
  for(pos = 0; pos < 1; pos += 1)  // goes from 0 degrees to 1 degrees 
  {                                  // in steps of 1 degree 
    pellonservo1.write(pos);              // tell servo to go to position in variable 'pos'
    pellonservo2.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(100);                       // waits 15ms for the servo to reach the position 
  } 
   Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor 1, 2, 3, 4 test");
    motorsupply.setSpeed(1000);
    motortakeup.setSpeed(1000);
  motorsupply.run(RELEASE);
  motortakeup.run(RELEASE);
}

int i;

void loop() {
  motorsupply.run(FORWARD);
  motortakeup.run(FORWARD);
  for (i=0; i<255; i++) {
    motorsupply.setSpeed(i); 
    motortakeup.setSpeed(i); 
    delay(70);
 }
 
  for (i=255; i!=0; i--) {
    motorsupply.setSpeed(i);
    motortakeup.setSpeed(i);  
    delay(70);
 }
 
  motorsupply.run(BACKWARD);
  motortakeup.run(BACKWARD);
  for (i=0; i<255; i++) {
    motorsupply.setSpeed(i); 
    motor2.setSpeed(i); 
    delay(70);
 }
 
  for (i=255; i!=0; i--) {
    motorsupply.setSpeed(i); 
    motortakeup.setSpeed(i); 
    delay(70);
 }
}
