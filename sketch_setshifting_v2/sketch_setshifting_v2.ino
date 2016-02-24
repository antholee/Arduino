char mouseID[] = "INSERT MOUSE ID HERE";
char date[] = "INSERT TODAY'S DATE";

//CUE PARAMETERS. CHANGE FOR EACH MOUSE

int stimForTraining[] = {1, 4,2, 1, 3, 3,2,4}; //order of stimulus given
int lastTrainTrial = 8; //length(stimForTrial)-1; it should match length of stimForTrial[];

int cueForBlock[] = {2,1}; //1 is light, 2 is sound. 1st element will be correct cue for 1st block. NOTE: arrays are zero-indexed
int stimForTrial[] = {2,  4, 1, 1, 3, 1, 4, 1, 2, 1, 3, 4, 1, 4, 3, 4, 3, 2, 1, 4, 1, 2, 4, 2, 1, 2, 2, 3, 3, 2, 2, 3, 4, 3, 3, 3, 1, 3, 3, 4, 1, 1, 4, 2, 4, 3, 1, 2, 3, 4, 4, 1, 4, 2, 3, 2, 2, 3, 2, 2, 2, 2, 4, 3, 1, 4, 3, 4, 2, 1, 1, 4, 1, 3, 1, 3, 1, 4}; //order of cues given
int lastStimTrial = 80; //length(stimForTrial)-1; it should match length of stimForTrial[];
//stim1 is leftLED,leftSound
//stim2 is leftLED, rightSound
//stim3 is rightLED, leftSound
//stim4 is right LED, rightSound


//experimental conditions. change as necessary
const int lSpeakerPitch = 7000; //in Hz
const int rSpeakerPitch = 11000;
const int stimInterval = 500; //ISI between blinks and pulses; in ms
unsigned long stimDuration = 60000;
const int rewardDuration = 350; //time water pump is on in ms
const int motorDuration = 5000; //in milliseconds

const int itiDuration = 5000; //time between trials in milliseconds
const int puffDuration = 1000;
const int beepDur = 500; //ISI of Beeps, in ms
const int maxTrials = 100;
const int trialsBack = 20; //calculates 80% of the n trialsBack
const float criteriaThreshold = 0.8; 

//pin assignments
const int lLEDpin = 2;
const int rLEDpin = 3;
const int lSpeakerPin = 4;
const int rSpeakerPin = 5;
const int npLEDpin = 6; //nosepoke sensor
const int lSensorPin = 7; 
const int rSensorPin = 8;
const int airPuffPin = 10;
const int lWaterPin = 12;
const int rWaterPin = 13;
const int sensorWallIR = A0;
const int resetIRpin = 11;

//trial variables 
unsigned long currentMillis = millis();
int stimTrainCounter = 0;
int stimCounter = 0; //counts what index of stimForTrial to use as currStim. If mouse makes mistake/omits trial, stimCounter doesn't advance.
int lastTrialOutcome = 1; //records if last trial was successful or not. 1 means correct, 2 means wrong, 9 means omitted
int lastStim; //records stimulus of last trial in order to repeat stimulus if failed trial
int trialNo = 1;
int trialNo_TRAIN = 1;
int rsTrialNo = 0;
int trialOutcomes[maxTrials] = {}; //trialOutcomes[0] is trial 1; 1
int correctCue;
int currStim;
int wallIRval = 0; //over 600au is closest distance to sensor
/* stimulus 0: turn off all stimulus
stimulus 1: left LED + left speaker
stimulus 2: left LED + right speaker
stimulus 3: right LED + left speaker
stimulus 4: right LED + right speaker */
int ledState = LOW;             // ledState used to set the door LED
int lSensorState = 0, last_lSensorState = 0, rSensorState = 0, last_rSensorState = 0, resetState = 0;
int trialState; //1 is pre-stim; 2 is after choice
int rsState =0 ; //0 is training; 1 is pre-ruleShift; 2 is post-ruleShift;

void setup() {
  // put your setup code here, to run once:
  pinMode(lLEDpin, OUTPUT); //left LED
  pinMode(rLEDpin, OUTPUT); //right LED
  pinMode(lSpeakerPin, OUTPUT); //left speaker
  pinMode(rSpeakerPin, OUTPUT); //right speaker
  pinMode(npLEDpin, OUTPUT); 
  pinMode(lSensorPin, INPUT); //IR in left nose poke
  pinMode(rSensorPin, INPUT); //IR in right nose poke
//  pinMode(motorPin, OUTPUT);
  pinMode(resetIRpin, INPUT); 
  pinMode(lWaterPin, OUTPUT);
  pinMode(rWaterPin, OUTPUT);
  pinMode(airPuffPin, OUTPUT);
  digitalWrite(lSensorPin, HIGH); // turn on the pullup
  digitalWrite(rSensorPin, HIGH); // turn on the pullup
  digitalWrite(resetIRpin, HIGH);
  correctCue = cueForBlock[0]; //
  //memset(trialOutcomes, 9, sizeof(trialOutcomes));
  Serial.begin(9600); 
  Serial.println("This is RULE SHIFT TEST. TWO cues/trial. Omitted trials are repeated");
   Serial.print("MouseID:\t");
  Serial.println(mouseID); 
  Serial.print("Date:\t");
  Serial.println(date);
  //remove prime event
//  digitalWrite(lWaterPin, HIGH);
//  digitalWrite(rWaterPin, HIGH);
//  delay(250);
//   digitalWrite(lWaterPin, LOW);
//  digitalWrite(rWaterPin, LOW);
  delay(10000);   
}

//return 0 if mouse omitted trial; return 1 if mouse chose left; return 2 if mouse chose right
int playStimulus(int stim) {
  if (stim == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    noTone(lSpeakerPin);
    noTone(rSpeakerPin);
  }
  
  if (stim == 1) {  
    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t\t\t\t\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t\t\t\t\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){      
              if (ledState == LOW) {
                ledState = HIGH;
                tone(lSpeakerPin, lSpeakerPitch, beepDur);
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
    Serial.print("Mouse OMITTED trial\t\t\t\t\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 0;    
    }
    
  if (stim == 2) {  
    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t\t\t\t\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t\t\t\t\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                tone(rSpeakerPin, rSpeakerPitch, beepDur);
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
    Serial.print("Mouse OMITTED trial\t\t\t\t\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 0;
  }

  if (stim == 3) {   
   unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t\t\t\t\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t\t\t\t\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){       
              if (ledState == LOW) {
                ledState = HIGH;
                tone(lSpeakerPin, lSpeakerPitch, beepDur);
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
    Serial.print("Mouse OMITTED trial\t\t\t\t\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 0; 
  }
  
   if (stim == 4) {
    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t\t\t\t\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t\t\t\t\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                tone(rSpeakerPin, rSpeakerPitch, beepDur);
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
   
    return 0;   
  }
}

//return 9 if omission
int choiceEval(int correctCue, int currentStim, int choice){
  digitalWrite(lLEDpin, LOW);
  digitalWrite(rLEDpin, LOW);
  if (choice ==0){
    Serial.print("Mouse OMITTED trial\t\t\t\t\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 9;
    
  }
  if (correctCue == 1){
    if(currentStim ==1 || currentStim ==2){
      if (choice == 1){    
        Serial.print("Mouse made CORRECT choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
      
        return 1;      
      }
      else if(choice ==2){
        Serial.print("Mouse made WRONG choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return 2;  
      }
    }
    
    if(currentStim == 3 || currentStim == 4){
      if (choice == 2){    
        Serial.print("Mouse made CORRECT choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
       
        return 1;      
      }
      else if(choice ==1){
        Serial.print("Mouse made WRONG choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return 2;  
      }
    }
  }

  if (correctCue == 2){
    if(currentStim ==1 || currentStim ==3){
      if (choice == 1){    
        Serial.print("Mouse made CORRECT choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
      
        return 1;      
      }
      else if(choice ==2){
        Serial.print("Mouse made WRONG choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return 2;  
      }
    }
    
    if(currentStim == 2 || currentStim == 4){
      if (choice == 2){    
        Serial.print("Mouse made CORRECT choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
       
        return 1;      
      }
      else if(choice ==1){
        Serial.print("Mouse made WRONG choice\t\t\t\t\t");
        currentMillis = millis();
        Serial.println(currentMillis);
        return 2;  
      }
    }
  }
}

void giveWater(int choice){
//1 is left, 2 is right
unsigned long currentMillis = millis();
unsigned long stopMillis = currentMillis + rewardDuration;

 if(choice == 1){
  while(currentMillis <=stopMillis){
  digitalWrite(lWaterPin, HIGH);
  currentMillis = millis();
    }
  digitalWrite(lWaterPin, LOW);
  return;
  }
 if(choice == 2){
  while(currentMillis <=stopMillis){
  digitalWrite(rWaterPin, HIGH);
  currentMillis = millis();
    }
  digitalWrite(rWaterPin, LOW);
  return;
  }
}

void resetTrial(){
  int currState;
  resetState = digitalRead(resetIRpin);
  unsigned long currMillis = millis();
  unsigned long timeLimit = currMillis + 600000;
  
  
  while (resetState == HIGH){
    resetState = digitalRead(resetIRpin);

     if (resetState == LOW) {     
        // turn LED on:       
        Serial.print("Mouse reset trial\t\t\t\t\t "); 
        unsigned long resetMillis = millis();
        Serial.println(resetMillis);
    
       
    }
  }
  resetState == HIGH; 
}

void itiDelay(){
  unsigned long prevMillis = millis();
  unsigned long itiMillis = prevMillis + itiDuration;
  
  while (prevMillis<itiMillis){
    currentMillis = millis();
    prevMillis = currentMillis; 
  }
}

void airPuff(){
  unsigned long prevMillis = millis();
  unsigned long puffMillis = prevMillis + puffDuration;
  
  while (prevMillis<puffMillis){
    currentMillis = millis();
    prevMillis = currentMillis; 
    digitalWrite(airPuffPin, HIGH);
  }
  digitalWrite(airPuffPin, LOW);
  
  
}

//return 0 if mouse omitted trial; return 1 if mouse chose left; return 2 if mouse chose right
int playStimulus_TRAIN(int stim) {
  if (stim == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    noTone(lSpeakerPin);
    noTone(rSpeakerPin);
  }
  
  if (stim == 1) {  
    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){      
              if (ledState == LOW) {
                ledState = HIGH;
                //tone(lSpeakerPin, lSpeakerPitch, beepDur);
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
   
    return 0;    
    }
    
  if (stim == 2) {  
    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                //tone(rSpeakerPin, rSpeakerPitch, beepDur);
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
    Serial.print("Mouse OMITTED trial\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 0;
  }

  if (stim == 3) {   
   unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){       
              if (ledState == LOW) {
                ledState = HIGH;
                tone(lSpeakerPin, lSpeakerPitch, beepDur);
                } 
              else  {
                    ledState = LOW;
                    }
              stimStartMillis = millis();
              stimStopMillis = stimStartMillis + stimInterval;         
        }
      stimStartMillis = millis();
      prevMillis = millis();
      //digitalWrite(rLEDpin, ledState);
      }
    Serial.print("Mouse OMITTED trial\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 0; 
  }
  
   if (stim == 4) {
    unsigned long prevMillis = millis();
    unsigned long stimMillis = prevMillis + stimDuration;
    unsigned long stimStartMillis = prevMillis;
    unsigned long stimStopMillis = stimStartMillis + stimInterval;
    ledState = HIGH;
    
    while (prevMillis<stimMillis){ 
      lSensorState = digitalRead(lSensorPin);
      rSensorState = digitalRead(rSensorPin);
      if (lSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose LEFT\t"); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 1;      
        }  
      if (rSensorState == LOW) {     
        // turn LED on:
        digitalWrite(npLEDpin, HIGH);        
        Serial.print("Mouse chose RIGHT\t" ); 
        unsigned long choiceMillis = millis();
        Serial.println(choiceMillis);
        digitalWrite(npLEDpin, LOW); 
        return 2;      
        } 
    
      if(stimStartMillis >= stimStopMillis){
          
              if (ledState == LOW) {
                ledState = HIGH;
                tone(rSpeakerPin, rSpeakerPitch, beepDur);
                } 
              else  {
                    ledState = LOW;
                    }
              stimStartMillis = millis();
              stimStopMillis = stimStartMillis + stimInterval;          
        }
      stimStartMillis = millis();
      prevMillis = millis();
      //digitalWrite(rLEDpin, ledState);
      }
   
    return 0;   
  }
}

int choiceEval_TRAIN(int currentStim, int choice){
  digitalWrite(lLEDpin, LOW);
  digitalWrite(rLEDpin, LOW);
  if (choice ==0){
    Serial.print("Mouse OMITTED trial\t");
    unsigned long omittedMillis = millis();
    Serial.println(omittedMillis);
    return 0;
    
  }

  if(currentStim ==1 || currentStim ==3){
    if (choice == 1){    
      Serial.print("Mouse made CORRECT choice\t");
      currentMillis = millis();
      Serial.println(currentMillis);
    
      return 1;      
    }
    else if(choice ==2){
      Serial.print("Mouse made WRONG choice\t");
      currentMillis = millis();
      Serial.println(currentMillis);
      return 2;  
    }
  }
  
  if(currentStim == 2 || currentStim == 4){
    if (choice == 2){    
      Serial.print("Mouse made CORRECT choice\t");
      currentMillis = millis();
      Serial.println(currentMillis);
     
      return 1;      
    }
    else if(choice ==1){
      Serial.print("Mouse made WRONG choice\t");
      currentMillis = millis();
      Serial.println(currentMillis);
      return 2;  
    }
  }

}

//this function takes the current nTrial and evaluates the sum of correct trials in N trialsBack after trialsBack > nTrials. Also,
//it marks a "1" (for correct trial) in the array trialOutcomes in position of index[nTrial-1]
float outcomeCount(int nTrial, int outcome){
  float percCorrect; //% correct of last n trialsBack
  float nCorrect = 0;

  //marks 1 for correct trial in array of all outcomes called trialOutcomes
  if (outcome ==1){
  trialOutcomes[nTrial-1] = outcome;
  }

  //display list of all trials (maxTrials) with 1 as correct trials and 0 as wrong/omitted trials
  Serial.println("Trial Outcomes:");
  int j;
  for (j = 0; j < maxTrials; j = j + 1) {
    Serial.print(trialOutcomes[j]);
  }
  Serial.println("");

  //calculates nCorrect of last nTrialsback
  if (rsState == 1){
  int i;
  for (i = nTrial-trialsBack; i<nTrial; i=i+1){
      nCorrect = trialOutcomes[i] + nCorrect;
    }
  if (nTrial>=trialsBack){
      Serial.print("Last ");
      Serial.print(trialsBack);
      Serial.print(" trials correct \t\t\t\t\t");
      Serial.println(nCorrect);
      percCorrect = nCorrect/trialsBack;
      Serial.print("% correct: \t\t\t\t\t");
      Serial.println(percCorrect);
      return percCorrect;
    }
    
  }

  if (rsState == 2){
    
  int i;
  for (i = nTrial-trialsBack; i<nTrial; i=i+1){
      nCorrect = trialOutcomes[i] + nCorrect;
    }

  if (rsTrialNo>=trialsBack){
    Serial.print("Last ");
    Serial.print(trialsBack);
    Serial.print(" trials correct \t\t\t\t\t");
    Serial.println(nCorrect);
    percCorrect = nCorrect/trialsBack;
    Serial.print("% correct: \t\t\t\t\t");
    Serial.println(percCorrect);
    return percCorrect;
  }
 
  }

  return 0; 
}



 
void loop() {
  // put your main code here, to run repeatedly:

  if(rsState ==0){
    Serial.println("---------"); 
    Serial.print("TRAINING TRIAL #: \t\t\t\t\t");
    Serial.println(trialNo_TRAIN);
    Serial.print("START TRIAL:\t\t\t\t\t");
    currentMillis = millis();
    Serial.println(currentMillis);

    if(lastTrialOutcome ==1){
    currStim = stimForTraining[stimTrainCounter];
    }
    else{
      currStim = lastStim;
    }

    Serial.print("Current stimulus:\t");
  Serial.println(currStim);
  Serial.print("Play stimulus\t");
  currentMillis = millis();
  Serial.println(currentMillis);
  Serial.println("Waiting for mouse to choose...");

  int choice;
  choice = playStimulus_TRAIN(currStim); //1 is left, 2 is right, 0 is omit

  int outcome = choiceEval_TRAIN(currStim, choice); //1 is true, 2 is false, 0 is omit
  lastTrialOutcome = outcome;
    if(outcome == 1){
    Serial.print("Dispensing reward\t");
    currentMillis = millis();
    Serial.println(currentMillis);
    giveWater(choice);
    stimTrainCounter = stimTrainCounter + 1;
  }

  if(outcome == 2){
    Serial.print("Puffing air\t");
    currentMillis = millis();
    Serial.println(currentMillis);
    airPuff();
  }

  lastStim = currStim;

   if(stimTrainCounter == lastTrainTrial){
    Serial.println("MOUSE FINISHED TRAINING");
    rsState = 1;
    }
  
  Serial.println("Waiting for mouse to reset trial...");
  resetTrial();

  
 
  trialNo_TRAIN = trialNo_TRAIN + 1;
  Serial.println("---------");    
  }

if(rsState != 0){
  Serial.println("---------"); 
  Serial.print("TRIAL #: \t\t\t\t\t");
  Serial.println(trialNo);
  Serial.print("START TRIAL:\t\t\t\t\t");
  currentMillis = millis();
  Serial.println(currentMillis);


  if(lastTrialOutcome ==9){
    currStim = lastStim;
    
  }
  else{
     currStim = stimForTrial[stimCounter];
  }
  

  
  Serial.print("Rewarded cue: ");
  if(correctCue == 1){
    Serial.print("LIGHT ");
  }
  if(correctCue == 2){
    Serial.print("SOUND ");
  }
  
  Serial.println(correctCue);
  String dispStim = "Current stimulus: ";
  String dispCurrStim = dispStim + currStim ;
  Serial.println(dispCurrStim);
  Serial.print("Play stimulus\t\t\t\t\t");
  currentMillis = millis();
  Serial.println(currentMillis);
  Serial.println("Waiting for mouse to choose...");
  
  int choice;
  choice = playStimulus(currStim); //1 is left, 2 is right, 0 is omit

  int outcome = choiceEval(correctCue, currStim, choice); //1 is true, 2 is false, 9 is omit
    lastTrialOutcome = outcome;

    if(outcome == 1){
    Serial.print("Dispensing reward\t\t\t\t\t");
    currentMillis = millis();
    Serial.println(currentMillis);
    giveWater(choice);
    stimCounter = stimCounter + 1;
  }

  if(outcome == 2){
    Serial.print("Puffing air\t\t\t\t\t");
    currentMillis = millis();
    Serial.println(currentMillis);
    airPuff();
    stimCounter = stimCounter + 1;
  }

   lastStim = currStim;

  if(outcome !=9){
  float percCorrect;
  percCorrect = outcomeCount(trialNo, outcome);
  
  
   if(rsState == 1){
    //transitions from pre to post ruleshift
    if(percCorrect >= criteriaThreshold){
      correctCue = cueForBlock[1];
      rsState = 2; //state transition variable; 2 is rule-shift (post)
     }
    }
     if (rsState == 2){
        rsTrialNo = rsTrialNo + 1;
      }
    trialNo = trialNo + 1;
  }
      
  if(stimCounter == lastStimTrial){
    Serial.println("---------"); 
    Serial.println("MOUSE FINISHED TRAINING");
    while(stimCounter!=99999){};
  }
  
  Serial.println("Waiting for mouse to reset trial...");
  resetTrial();

 
  
  Serial.println("---------"); 


}

}


