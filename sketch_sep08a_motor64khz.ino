#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR34_64KHZ);

char val;

void setup()
{

  Serial.begin(57600);

  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
}

void loop()
{
  if(Serial.available())
  {
    val = Serial.read();
  }

  if(val == 'U'){
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  } else if(val == 'D'){
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
  } else if(val == 'N'){
    motor3.run(FORWARD);
  } else if(val == 'F'){
    motor3.run(BACKWARD);
  }else if(val == 'S'){
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
  }
  delay(100);
}
