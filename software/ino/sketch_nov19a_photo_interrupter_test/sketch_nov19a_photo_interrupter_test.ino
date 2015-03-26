int sensorPin = 0;
int ledPin = 7;
int sensorValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("start"); 
}
void loop() {
  sensorValue = analogRead(sensorPin);
  digitalWrite(ledPin, sensorValue);
  Serial.println("transistor activating");
}
