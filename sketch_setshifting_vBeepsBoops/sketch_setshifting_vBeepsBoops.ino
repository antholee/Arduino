
//pin assignments
const int lLEDpin = 2;
const int rLEDpin = 3;
const int lSpeakerPin = 4;
const int rSpeakerPin = 5;
const int npLEDpin = 6; //nosepoke sensor
const int lSensorPin = 7; 
const int rSensorPin = 8;
const int motorPin = 9;
const int sensorWallIR = A0;

//experimental conditions. change as necessary

const int lSpeakerPitch = 7000; //in Hz
const int rSpeakerPitch = 11000;
const int stimInterval = 500; //ISI between blinks and pulses; in ms
const int stimDuration = 29000;

const int motorDuration = 5000; //in milliseconds
const int trialDuration = 30000; //max trial length in milliseconds
const int itiDuration = 5000; //time between trials in milliseconds
const int beepDur = 500; //ISI of Beeps, in ms



//trial variables 
unsigned long currentMillis = millis();
int correctCue = rand() % 2 + 1; //1 is light, 2 is sound
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
int trialState; //1 is pre-stim; 2 is after choice




void setup() {
  // put your setup code here, to run once:
  pinMode(lLEDpin, OUTPUT); //left LED
  pinMode(rLEDpin, OUTPUT); //right LED
  pinMode(lSpeakerPin, OUTPUT); //left speaker
  pinMode(rSpeakerPin, OUTPUT); //right speaker
  pinMode(npLEDpin, OUTPUT); 
  pinMode(lSensorPin, INPUT); //IR in left nose poke
  pinMode(rSensorPin, INPUT); //IR in right nose poke
  pinMode(motorPin, OUTPUT);
  digitalWrite(lSensorPin, HIGH); // turn on the pullup
  digitalWrite(rSensorPin, HIGH); // turn on the pullup
  Serial.begin(9600);
  
}


    

void playStimulus(int stim) {
  if (stim == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    noTone(lSpeakerPin);
    noTone(rSpeakerPin);
  }
  
  if (stim == 1) {
    //tone(lSpeakerPin, 3000, duration); original code
    
    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
    
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                tone(lSpeakerPin, 4000, beepDur);
                } 
              else  {
                    ledState = LOW;
                    }
              stimStartMillis = millis();
              stimStopMillis = stimStartMillis + stimInterval;
              
        }
      stimStartMillis = millis();
      prevMillis = millis();
      digitalWrite(lLEDpin, ledState);
      }
    
    }
    
  
  if (stim == 2) {  
    

    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){
    
      
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                tone(rSpeakerPin, 7000, beepDur);
                } 
              else  {
                    ledState = LOW;
                    }
              stimStartMillis = millis();
              stimStopMillis = stimStartMillis + stimInterval;
              
      }
      stimStartMillis = millis();
      prevMillis = millis();
      digitalWrite(lLEDpin, ledState);
    
    }
    
  }

  if (stim == 3) {   
   

    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){
    
      
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                  tone(lSpeakerPin, 4000, beepDur);
                } 
              else  {
                    ledState = LOW;
                    }
              stimStartMillis = millis();
              stimStopMillis = stimStartMillis + stimInterval;
              
      }
      stimStartMillis = millis();
      prevMillis = millis();
      digitalWrite(rLEDpin, ledState);
    
    }
     
  }
  
   if (stim == 4) {
   

     unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){
       
      
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                tone(rSpeakerPin, 7000, beepDur);
                } 
              else  {
                    ledState = LOW;
                    }
              stimStartMillis = millis();
              stimStopMillis = stimStartMillis + stimInterval;
              
      }
      stimStartMillis = millis();
      prevMillis = millis();
      digitalWrite(rLEDpin, ledState);
    
    }
   }
}
}



  


int mouseChoose(){ //1 is left, 2 is right, 0 is omit
  unsigned long prevMillis = millis();
  unsigned long trialMillis = prevMillis + trialDuration;
  
  while (prevMillis<trialMillis){
    lSensorState = digitalRead(lSensorPin);
    rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t\t\t\t\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); trialState = 2;
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t\t\t\t\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); trialState = 2;
        return 2;      
        } 
    // if exceeded trial time
    digitalWrite(npLEDpin, LOW);
    currentMillis = millis();
    prevMillis = currentMillis;   
       }  
    Serial.print("Mouse OMITTED trial\t\t\t\t\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis); trialState = 2;
    return 0;       
}

boolean choiceEval(int correctCue, int currentStim, int choice){
  if (correctCue == 1){
    if(currentStim ==1 || currentStim ==2){
      if (choice == 1){    
        Serial.print("Mouse made CORRECT choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return true;      
      }
      else if(choice ==2){
        Serial.print("Mouse made WRONG choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return false;  
      }
    }
    
    if(currentStim == 3 || currentStim == 4){
      if (choice == 2){    
        Serial.print("Mouse made CORRECT choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return true;      
      }
      else if(choice ==1){
        Serial.print("Mouse made WRONG choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return false;  
      }
    }
  }

  if (correctCue == 2){
    if(currentStim ==1 || currentStim ==3){
      if (choice == 1){    
        Serial.print("Mouse made CORRECT choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return true;      
      }
      else if(choice ==2){
        Serial.print("Mouse made WRONG choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return false;  
      }
    }
    
    if(currentStim == 2 || currentStim == 4){
      if (choice == 2){    
        Serial.print("Mouse made CORRECT choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return true;      
      }
      else if(choice ==1){
        Serial.print("Mouse made WRONG choice \t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return false;  
      }
    }
  }
}
  

void itiDelay(){
  unsigned long prevMillis = millis();
  unsigned long itiMillis = prevMillis + itiDuration;
  
  while (prevMillis<itiMillis){
    currentMillis = millis();
    prevMillis = currentMillis; 
  }
}


 
void loop() {
  // put your main code here, to run repeatedly:
  trialState = 1;
  String trialString = "START TRIAL: ";
  String dispCurrTrial = trialString + trialNo;
  Serial.print(dispCurrTrial);
  Serial.print("\t\t\t\t\t");
  currentMillis = millis();
  Serial.println(currentMillis);
  
  Serial.print("Correct cue: ");
  if(correctCue == 1){
    Serial.print("LIGHT ");
  }
  if(correctCue == 2){
    Serial.print("SOUND ");
  }
  
  Serial.println(correctCue);
  
  currStim = rand() % 4 + 1;
  String dispStim = "Current stimulus: ";
  String dispCurrStim = dispStim + currStim ;
  Serial.println(dispCurrStim);
  Serial.print("Play stimulus \t\t\t\t\t");
  currentMillis = millis();
  Serial.println(currentMillis);
  Serial.println("Waiting for mouse to choose...");
  
  int choice;
  choice = playStimulus(currStim);

  
  
  
  
  
  boolean outcome = choiceEval(correctCue, currStim, choice);
  
  if(outcome == true){
    Serial.print("Dispensing reward\t\t\t\t\t");
    currentMillis = millis();
    Serial.println(currentMillis);
    //filler for rewardMouse function
  }
  
  
  Serial.println("Waiting for mouse to reset trial...");
  
  
  Serial.print("Mouse reset trial ");
  currentMillis = millis();
  Serial.println(currentMillis);
  
  trialNo = trialNo + 1;
  Serial.println("---------"); 
  Serial.println("Between trial delay...");
  itiDelay();
  Serial.println("---------"); 


 

}


