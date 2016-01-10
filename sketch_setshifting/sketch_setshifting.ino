
//pin assignments
const int lLEDpin = 2;
const int rLEDpin = 3;
const int lSpeakerPin = 4;
const int rSpeakerPin = 5;
const int LEDpin = 6; //replace with "door"
const int lSensorPin = 7; 
const int rSensorPin = 8;
const int motorPin = 9;
const int sensorWallIR = A0;

//experimental conditions. change as necessary
const int preDoorStimDur = 2000; //in milliseconds
const int postDoorStimDur = 2000; //in milliseconds
const int motorDuration = 5000; //in milliseconds
const int trialDuration = 30000; //max trial length in milliseconds
const int itiDuration = 5000; //time between trials in milliseconds

//trial variables 
int trialNo = 1;
int currStim;
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
  
}

void onLED(int ledPin, int duration) {

  unsigned long prevMillis = millis();
  unsigned long stimMillis = prevMillis + duration;

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


void playStimulus(int stim, int duration) {

  if (stim == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    noTone(lSpeakerPin);
    noTone(rSpeakerPin);
  }
  
  if (stim == 1) {
    tone(lSpeakerPin, 3000, duration);
    onLED(lLEDpin, duration);
    }
    
  
  if (stim == 2) {  
    tone(rSpeakerPin, 3000, duration);  
    onLED(lLEDpin, duration);
  }

  if (stim == 3) {   
    tone(lSpeakerPin, 3000, duration);
    onLED(rLEDpin, duration); 
  }
  
   if (stim == 4) {
    tone(rSpeakerPin, 3000, duration);
     onLED(rLEDpin, duration); 
   }
}






int mouseChoose(){ //1 is left, 2 is right
  unsigned long prevMillis = millis();
  unsigned long trialMillis = prevMillis + trialDuration;
  
  while (prevMillis<trialMillis){
    lSensorState = digitalRead(lSensorPin);
    rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(LEDpin, HIGH);        
        Serial.print("Mouse chose LEFT "); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(LEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(LEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT " ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(LEDpin, LOW); 
        return 2;      
        } 
    // if exceeded trial time
    digitalWrite(LEDpin, LOW);
    unsigned long currentMillis = millis();
    prevMillis = currentMillis;   
       }  
    Serial.print("Mouse OMITTED trial ");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 0;
          
}

void wallUp(){
  wallIRval = analogRead(sensorWallIR);
  while (wallIRval < 500){
    wallIRval = analogRead(sensorWallIR);  
  }
  digitalWrite(LEDpin, HIGH);
  Serial.print("Wall moving up ");
  unsigned long wallUpMillis = millis();
  Serial.println(wallUpMillis);
  onMotor(motorPin, 200); 
  digitalWrite(LEDpin, LOW);
  
}

void itiDelay(){
  unsigned long prevMillis = millis();
  unsigned long itiMillis = prevMillis + itiDuration;
  
  while (prevMillis<itiMillis){
    unsigned long currentMillis = millis();
    prevMillis = currentMillis; 
  }
}


 
void loop() {
  // put your main code here, to run repeatedly:


String trialString = "START TRIAL ";
String dispCurrTrial = trialString + trialNo + " " ;
Serial.print(dispCurrTrial);
unsigned long currentMillis = millis();
Serial.println(currentMillis);
currStim = rand() % 4 + 1;
String dispStim = "Current stimulus is: ";
String dispCurrStim = dispStim + currStim ;
Serial.println(dispCurrStim);
Serial.print("Play pre-door stim ");
currentMillis = millis();
Serial.println(currentMillis);
playStimulus(currStim, preDoorStimDur);

Serial.print("Wall moving down "); 
currentMillis = millis();
Serial.println(currentMillis);
onMotor(motorPin, 200);


Serial.print("Replay post-door stim ");
currentMillis = millis();
Serial.println(currentMillis);
playStimulus(currStim, postDoorStimDur);
Serial.println("Waiting for mouse to choose...");
int choice = mouseChoose();


Serial.print("Dispensing reward ");
currentMillis = millis();
Serial.println(currentMillis);
//filler for rewardMouse function

Serial.println("Waiting for mouse to reset trial...");

wallUp();
Serial.print("Wall closed ");
currentMillis = millis();
Serial.println(currentMillis);

trialNo = trialNo + 1;
Serial.println("---------"); 
Serial.println("Between trial delay...");
itiDelay();
Serial.println("---------"); 


 

}


