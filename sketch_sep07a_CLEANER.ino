
#include <Wire.h>
#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor 1 test");
    motor1.setSpeed(1000);
    motor2.setSpeed(1000);
    motor3.setSpeed(1000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
}

int i;

// Test the DC motor, stepper and servo ALL AT ONCE!
void loop() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  for (i=0; i<255; i++) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    motor3.setSpeed(i);
    delay(70);
 }
 
  for (i=255; i!=0; i--) {
    motor1.setSpeed(i);
    motor2.setSpeed(i);  
    motor3.setSpeed(i);
    delay(70);
 }
 
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  for (i=0; i<255; i++) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    motor3.setSpeed(i);
    delay(70);
 }
 
  for (i=255; i!=0; i--) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    motor3.setSpeed(i);
    delay(70);
 }
}


