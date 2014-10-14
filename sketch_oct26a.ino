#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
Servo myservo2;  // create servo object to control a servo
                
int pos = 0;    // variable to store the servo position 

#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(10);  // attaches the servo on pin 9 to the servo object
  for(pos = 0; pos < 1; pos += 1)  // goes from 0 degrees to 1 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(100);                       // waits 15ms for the servo to reach the position 
  } 
   Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor 1, 2, 3, 4 test");
    motor1.setSpeed(1000);
    motor2.setSpeed(1000);
    motor3.setSpeed(1000);
    motor4.setSpeed(1000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

int i;

void loop() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  for (i=0; i<255; i++) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    motor3.setSpeed(i);
    motor4.setSpeed(1);
    delay(70);
 }
 
  for (i=255; i!=0; i--) {
    motor1.setSpeed(i);
    motor2.setSpeed(i);  
    motor3.setSpeed(i);
    motor4.setSpeed(1);
    delay(70);
 }
 
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  for (i=0; i<255; i++) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    motor3.setSpeed(i);
    motor4.setSpeed(i);
    delay(70);
 }
 
  for (i=255; i!=0; i--) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    motor3.setSpeed(i);
    motor4.setSpeed(i);
    delay(70);
 }
}
