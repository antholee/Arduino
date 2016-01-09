
const int lLEDpin = 2;
const int rLEDpin = 3;
const int lSpeaker = 4;
const int rSpeaker = 5;
const int time_stimDelays = 500;
const int sensorWallIR = A0;
const long ledInterval = 250;           // in milliseconds
const int stimDuration = 2000; //in milliseconds

int sensorWallVal = 0; //over 600au is closest distance to sensor
/* stimulus 0: turn off all stimulus
stimulus 1: left LED + left speaker
stimulus 2: left LED + right speaker
stimulus 3: right LED + left speaker
stimulus 4: right LED + right speaker */

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;   // will store last time LED was updated


void setup() {
  // put your setup code here, to run once:
  pinMode(lLEDpin, OUTPUT); //left LED
  pinMode(rLEDpin, OUTPUT); //right LED
  pinMode(lSpeaker, OUTPUT); //left speaker
  pinMode(rSpeaker, OUTPUT); //right speaker
  Serial.begin(9600);
  Serial.println("Enter stimulus # 1 to 4 or '0' for no stimulus");
  
  
}

void onLED(int ledPin) {

  unsigned long prevMillis=millis();
  unsigned long stimMillis = prevMillis + stimDuration;

  while (prevMillis<stimMillis){
    digitalWrite(ledPin, HIGH);
    unsigned long currentMillis = millis();
    prevMillis = currentMillis;
    }
  
    digitalWrite(ledPin, LOW);
}


void playStimulus(int stim) {

  if (stim == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    noTone(lSpeaker);
    noTone(rSpeaker);
  }
  
  if (stim == 1) {
    tone(lSpeaker, 3000, stimDuration);
    onLED(lLEDpin);
    }
    
  
  if (stim == 2) {  
    tone(rSpeaker, 3000, stimDuration);  
    onLED(lLEDpin);
  }

  if (stim == 3) {   
    tone(lSpeaker, 3000, stimDuration);
    onLED(rLEDpin); 
  }
  
   if (stim == 4) {
    tone(rSpeaker, 3000, stimDuration);
     onLED(rLEDpin); 
   }
}
  
  


  



void loop() {
  // put your main code here, to run repeatedly:


 if (Serial.available())
  {
  
    char ch = Serial.read();
    int tempStim = ch - '0';
    
    String dispStim = "Current stimulus is: ";
    String curStim = dispStim + tempStim;
    Serial.println(curStim);
    playStimulus(tempStim);
    sensorWallVal = analogRead(sensorWallIR);
    String dispWallIR = "Distance to IR sensor is: ";
    String curWallIR = dispWallIR + sensorWallVal;
    Serial.println(curWallIR);
    
  }    
 
 

}


