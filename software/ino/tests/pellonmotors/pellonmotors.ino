int motor = 10;
int potentiometer = 5;

void setup()
{
  pinMode(10,OUTPUT);
  pinMode(5,INPUT);
  
  Serial.begin(9600);
}

void loop(){
  int value = analogRead(potentiometer);//read input value: range between (0,1023)
  int motor_speed = value/4;//PWM can only ouput 255 different values
  
  analogWrite(motor,motor_speed);
  
  Serial.println(motor_speed);//for testing purposes
}

