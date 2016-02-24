const int motor1= 12; //left pump, from mouse
const int motor2= 13; 

void setup()
{
pinMode(motor1, OUTPUT); //sets the digital pin as output
pinMode(motor2, OUTPUT);
}

void loop()
{
digitalWrite(motor1,HIGH); //turns the motor on
delay(300);
digitalWrite(motor1,LOW);
delay(5000);
digitalWrite(motor2,HIGH); //turns the motor on
delay(300);
digitalWrite(motor2,LOW);
delay(5000);

}


