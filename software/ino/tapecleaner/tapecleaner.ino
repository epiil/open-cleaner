//In-progress sketch relaying photoresistor info to DC motor


int ruptPin = 2; // select analog 2 input pin to the interrupter
int photoVal; // variable to store the value coming from the sensor

void setup() 
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(8, OUTPUT); //Initiates Brake Channel B pin
}

void loop() 
{
  photoVal = analogRead(ruptPin); // read the value from photointerrupter
  Serial.println(photoVal); // print the sensor value to the serial monitor
  delay(50);
  if (photoVal > 10)
{
  digitalWrite(13, LOW); //Stops Channel A
  digitalWrite(8, HIGH);   //Engages the Brake for Channel A
  digitalWrite(12, LOW); //Establishes forward direction of Channel B
  digitalWrite(9, HIGH);   //Disengages the Brake for Channel B
}
  else
{
  digitalWrite(13, HIGH); //Establishes forward direction of Channel A
  digitalWrite(8, LOW);   //Disengages the Brake for Channel A
  analogWrite(3, 80);   //Spins the motor on Channel A at full speed
  digitalWrite(12, HIGH); //Establishes backward direction of Channel B
  digitalWrite(9, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 200);   //Spins the motor on Channel B at half speed
}
}

