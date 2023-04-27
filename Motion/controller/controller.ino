//defines pins
const int raStepPin = 27;  //RA PUL - Pulse
const int raDirPin = 33;   //RA DIR - Direction

const int decStepPin = 15;  //DEC PUL - Pulse
const int decDirPin = 32;   //DEC DIR - Direction


const float siderealSecs = 86164.0905; //seconds in a sidereal day
const int reduction = 30 * 26.85; //total gear reduction
const int microsteps = 8; //current microstep setting

const int pulseTime = 100; //this is in microseconds
const int delayTime = 0.5; //this is in milliseconds



int stepsMissed;

const long siderealRate = 360 * (1/siderealSecs); //degrees per second for tracking 
const long siderealRateDelay = 1/(siderealRate * reduction * microsteps / (1.8*1000000)); // microseconds delay between each RA microstep, 

const float stepsPerDeg = reduction/(1.8/microsteps);


int lastCommand = millis();
int startMotion = millis();
int endMotion = millis();

long lastTick = micros();

float correction;

String ackString;
String raStepsStr;
String decStepsStr;

int raStepsInt;
int decStepsInt;

void askForCommand() {
  Serial.print("ask\n");
}

void doIdleStep() {
  // digitalWrite(raDirPin, LOW);
  // digitalWrite(raStepPin,HIGH); 
  // delayMicroseconds(pulseTime); 
  // digitalWrite(raStepPin,LOW); 
  // delayMicroseconds(pulseTime);
  //delay(2*delayTime);
  if (micros() - lastTick > siderealRateDelay) {
    lastTick = micros();
    digitalWrite(decStepPin,HIGH); 
    delayMicroseconds(pulseTime); 
    digitalWrite(decStepPin,LOW); 
    delayMicroseconds(pulseTime);
  }

  delay(delayTime); 
}

void doMotion(int raSteps, int decSteps) {
  if (raSteps > 0){
    digitalWrite(raDirPin,HIGH);
  }
  else {
    digitalWrite(raDirPin,LOW);    
  }
  raSteps = abs(raSteps);


  //switch this high and low if spins the wrong way (or just the inequality)
  if (decSteps < 0){
    digitalWrite(decDirPin,HIGH);
  }
  else {
    digitalWrite(decDirPin,LOW);    
  }
  decSteps = abs(decSteps);


  if (decSteps > raSteps) {
    for (int i = 0; i < raSteps; i++) { 
      digitalWrite(raStepPin,HIGH); 
      digitalWrite(decStepPin,HIGH); 
      delayMicroseconds(pulseTime); 
      digitalWrite(raStepPin,LOW); 
      digitalWrite(decStepPin,LOW); 
      delayMicroseconds(pulseTime);
      delay(delayTime);
    }

    for (int i = 0; i < decSteps - raSteps; i++) {  
      digitalWrite(decStepPin,HIGH); 
      delayMicroseconds(pulseTime); 
      digitalWrite(decStepPin,LOW); 
      delayMicroseconds(pulseTime);
      delay(delayTime);
    }
  }
  else {
    for (int i = 0; i < decSteps; i++) { 
      digitalWrite(raStepPin,HIGH); 
      digitalWrite(decStepPin,HIGH); 
      delayMicroseconds(pulseTime); 
      digitalWrite(raStepPin,LOW); 
      digitalWrite(decStepPin,LOW); 
      delayMicroseconds(pulseTime);
      delay(delayTime);
    }

    for (int i = 0; i < raSteps - decSteps; i++) {  
      digitalWrite(raStepPin,HIGH); 
      delayMicroseconds(pulseTime); 
      digitalWrite(raStepPin,LOW); 
      delayMicroseconds(pulseTime);
      delay(delayTime);
    }
  }
}

void makeUpIdle(int missedTime){
  stepsMissed = (missedTime * 1000) / siderealRateDelay;
  digitalWrite(raDirPin,HIGH);
  
  //uncomment this if need be after testing the rest of makeUpIdle
  correction = 1; //+ (2*pulseTime/1000 + delayTime)/missedTime;
  
  for (int i = 0; i < stepsMissed * correction; i++) {  
    digitalWrite(raStepPin,HIGH); 
    delayMicroseconds(pulseTime); 
    digitalWrite(raStepPin,LOW); 
    delayMicroseconds(pulseTime);
    delay(delayTime);
  }
}

void setup() {
  // Sets the pins as Outputs
  pinMode(raStepPin, OUTPUT); 
  pinMode(raDirPin, OUTPUT);

  pinMode(decStepPin, OUTPUT); 
  pinMode(decDirPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  doIdleStep();

  if (Serial.available()) {
    startMotion = millis();
    //ra_steps+','+dec_steps+','
    raStepsStr = Serial.readStringUntil(',');
    raStepsInt = raStepsStr.toInt();

    decStepsStr = Serial.readStringUntil(',');
    decStepsInt = decStepsStr.toInt();

    //combine into one print line
    ackString = "Received RA Value: "+raStepsStr + " and Dec Value: " + decStepsStr;
    Serial.println(ackString);

    doMotion(raStepsInt, decStepsInt);

    endMotion = millis();
    
    //uncomment this after everything else is working
    //makeUpIdle(endMotion-startMotion);

    lastCommand = millis();     
  }

  if (millis() - lastCommand > 5000) {
    askForCommand(); 
  }
  else if (!((raStepsInt == 0) && (decStepsInt == 0))) {
    askForCommand();    
  }  

