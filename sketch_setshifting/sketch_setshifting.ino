
const int lLEDpin = 2;
const int rLEDpin = 3;
const int lSpeakerPin = 4;
const int rSpeakerPin = 5;
const int LEDpin = 6; //replace with "door"
const int lSensorPin = 7; 
const int rSensorPin = 8;
const int motorPin = 9;
const int time_stimDelays = 500;
const int sensorWallIR = A0;
const long ledInterval = 250;           // in milliseconds
const int stimDuration = 2000; //in milliseconds
const int motorDuration = 5000; //in milliseconds
 
int stimArray[4] = {1, 2, 3, 4};

int wallIRval = 0; //over 600au is closest distance to sensor
/* stimulus 0: turn off all stimulus
stimulus 1: left LED + left speaker
stimulus 2: left LED + right speaker
stimulus 3: right LED + left speaker
stimulus 4: right LED + right speaker */
int ledState = LOW;             // ledState used to set the door LED
int lSensorState = 0, last_lSensorState = 0, rSensorState = 0, last_rSensorState = 0;




void setup() {
  // put your setup code here, to run once:
  pinMode(lLEDpin, OUTPUT); //left LED
  pinMode(rLEDpin, OUTPUT); //right LED
  pinMode(lSpeakerPin, OUTPUT); //left speaker
  pinMode(rSpeakerPin, OUTPUT); //right speaker
  pinMode(LEDpin, OUTPUT); 
  pinMode(lSensorPin, INPUT); //IR in left nose poke
  pinMode(rSensorPin, INPUT); //IR in right nose poke
  pinMode(motorPin, OUTPUT);
  digitalWrite(lSensorPin, HIGH); // turn on the pullup
  digitalWrite(rSensorPin, HIGH); // turn on the pullup
  Serial.begin(9600);
  Serial.println("Enter stimulus # 1 to 4 or '0' for no stimulus");
  
}

void onLED(int ledPin) {

  unsigned long prevMillis = millis();
  unsigned long stimMillis = prevMillis + stimDuration;

  while (prevMillis<stimMillis){
    digitalWrite(ledPin, HIGH);
    unsigned long currentMillis = millis();
    prevMillis = currentMillis;
    }
  
    digitalWrite(ledPin, LOW);
}

void onMotor(int motorPin, int speed){
  unsigned long prevMillis = millis();
  unsigned long stimMillis = prevMillis + motorDuration;

  while (prevMillis<stimMillis){
    analogWrite(motorPin, speed);
    unsigned long currentMillis = millis();
    prevMillis = currentMillis;
    }
  
    analogWrite(motorPin, 0);
}


void playStimulus(int stim) {

  if (stim == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    noTone(lSpeakerPin);
    noTone(rSpeakerPin);
  }
  
  if (stim == 1) {
    tone(lSpeakerPin, 3000, stimDuration);
    onLED(lLEDpin);
    }
    
  
  if (stim == 2) {  
    tone(rSpeakerPin, 3000, stimDuration);  
    onLED(lLEDpin);
  }

  if (stim == 3) {   
    tone(lSpeakerPin, 3000, stimDuration);
    onLED(rLEDpin); 
  }
  
   if (stim == 4) {
    tone(rSpeakerPin, 3000, stimDuration);
     onLED(rLEDpin); 
   }
}
  
 
void loop() {
  // put your main code here, to run repeatedly:
asdf


// code for left and right poke sensors
lSensorState = digitalRead(lSensorPin);
  if (lSensorState == LOW) {     
    // turn LED on:
    digitalWrite(LEDpin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(LEDpin, LOW); 
  }
  
  if (lSensorState && !last_lSensorState) {
    Serial.println("left sensor UNBROKEN");
  } 
  if (!lSensorState && last_lSensorState) {
    Serial.println("left sensor BROKEN");
  }
  last_lSensorState = lSensorState;


rSensorState = digitalRead(rSensorPin);
  if (rSensorState == LOW) {     
    // turn LED on:
    digitalWrite(LEDpin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(LEDpin, LOW); 
  }
  
  if (rSensorState && !last_rSensorState) {
    Serial.println("right sensor UNBROKEN");
  } 
  if (!rSensorState && last_rSensorState) {
    Serial.println("right sensor BROKEN");
  }
  last_rSensorState = rSensorState;
  


 if (Serial.available())
  {
    char ch = Serial.read();
    int tempStim = ch - '0';
    String dispStim = "Current stimulus is: ";
    String curStim = dispStim + tempStim;
    Serial.println(curStim);
    playStimulus(tempStim);
    wallIRval = analogRead(sensorWallIR);
    String dispWallIR = "Distance to IR sensor is: ";
    String curWallIR = dispWallIR + wallIRval;
    Serial.println(curWallIR);
  }  

  wallIRval = analogRead(sensorWallIR);
  if (wallIRval > 600){
    digitalWrite(LEDpin, HIGH);
    onMotor(motorPin, 200);
  }
  else digitalWrite(LEDpin, LOW);



  
 

}


