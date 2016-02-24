const int airPuffPin = 10;

void setup() {
  // put your setup code here, to run once:
 pinMode(airPuffPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(airPuffPin, HIGH);
  delay(5000);
  digitalWrite(airPuffPin, LOW);
  delay(1000);

}


