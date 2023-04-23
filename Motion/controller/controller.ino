//defines pins
const int raStepPin = 27;  //RA PUL - Pulse
const int raDirPin = 33;   //RA DIR - Direction

const int decStepPin = 15;  //DEC PUL - Pulse
const int decDirPin = 32;   //DEC DIR - Direction


const float siderealSecs = 86164.0905; //seconds in a sidereal day
const int reduction = 30 * 26.85; //total gear reduction
const int microsteps = 8; //current microstep setting

const int pulseTime = 100;
const int delayTime = 0.5;

int stepsMissed;


const long siderealRate = 360 * (1/siderealSecs); //degrees per second for tracking
const long siderealRateDelay = 1/(siderealRate * reduction * microsteps / 1000000); // microseconds delay between each RA microstep, 

const float stepsPerDeg = reduction/(1.8/microsteps)


int lastCommand = millis();
int startMotion = millis();
int endMotion = millis();

float correction;

void askForCommand() {
  Serial.print(" ");
}

void doIdleStep() {
      digitalWrite(raStepPin,HIGH); 
      delayMicroseconds(pulseTime); 
      digitalWrite(raStepPin,LOW); 
      delayMicroseconds(pulseTime);
      delayMicroseconds(siderealRateDelay - 2 * pulseTime);  
}

void doMotion(int raSteps, int decSteps) {
  if (raSteps > 0){
    digitalWrite(raDirPin,HIGH);
  }
  else {
    digitalWrite(raDirPin,LOW);    
  }
  raSteps = abs(raSteps);

  if (decSteps > 0){
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
  stepsMissed = (missedTime * 1000) / siderealRateDelay
  digitalWrite(raDirPin,HIGH)
  
  correction = 1 + (2*pulseTime/1000 + delayTime)/missedTime
  
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

    raStepsStr = Serial.readStringUntil(',');
    raStepsInt = raStepsStr.toInt();
    Serial.print("Received RA Value: ");
    Serial.println(raStepsStr);
    
    decStepsStr = Serial.readStringUntil(',');
    decStepsInt = decStepsStr.toInt();
    Serial.print("Received Dec Value: ");
    Serial.println(decStepsStr);

    doMotion(raStepsInt, decStepsInt);

    endMotion = millis();
    makeUpIdle(endMotion-startMotion);

    lastCommand = millis();     
  }

  if (!((raStepsInt == 0) && (decStepsInt == 0)) || (millis() - lastCommand > 10000)){
    askForCommand();    
  }
}

// void loop() {
//   static unsigned long lastUpdateTime = 0;
//   static bool isTracking = false;

//   if (Serial.available() > 0) {
//     int raError = 0;
//     int decError = 0;

//     //check the below
//     int readBytes = Serial.readBytes((char *)&raError, sizeof(raError));
//     if (readBytes == sizeof(raError)) {
//       Serial.readBytes((char *)&decError, sizeof(decError));

//       // Set the direction of the motors
//       digitalWrite(raDirPin, raError > 0 ? HIGH : LOW);
//       digitalWrite(decDirPin, decError > 0 ? HIGH : LOW);

//       // Enable the motors check if want do this
//       //digitalWrite(raEnPin, HIGH);
//       //digitalWrite(decEnPin, HIGH);

//       // Move the motors

//     if abs()
//     for (decsteps){
//         digitalWrite(raStepPin, HIGH);
//         digitalWrite(decStepPin, HIGH);
//         delayMicroseconds(100);
//         digitalWrite(raStepPin, LOW);
//         digitalWrite(decStepPin, LOW);
//         delayMicroseconds(100);
//     }

//     for(rasteps-decsteps)





//       for (int i = 0; i < abs(raError * stepsPerDeg); i++) {
//         digitalWrite(raStepPin, HIGH);
//         delayMicroseconds(250);
//         digitalWrite(raStepPin, LOW);
//         delayMicroseconds(250);
//       }
//       for (int i = 0; i < abs(decError * stepsPerDeg); i++) {
//         digitalWrite(decStepPin, HIGH);
//         delayMicroseconds(250);
//         digitalWrite(decStepPin, LOW);
//         delayMicroseconds(250);
//       }

//       // Disable the motors
//       //digitalWrite(raEnPin, LOW);
//       //digitalWrite(decEnPin, LOW);

//       // Indicate that the instruction has been completed
//       Serial.write("done", 4);
      
//       // Start tracking if the instruction is 0 steps
//       if (raError == 0 && decError == 0) {
//         isTracking = true;
//       }
//       else {
//         isTracking = false;
//       }
//     }
//   }

//   if (isTracking) {
//     // Only update the RA motor when the sidereal time has elapsed
//     unsigned long currentTime = micros();
//     if (currentTime - lastUpdateTime >= siderealRateDelay) {
//       //digitalWrite(raEnPin, HIGH);
//       digitalWrite(raDirPin, HIGH);
//       digitalWrite(raStepPin, HIGH);
//       delayMicroseconds(500);
//       digitalWrite(raStepPin, LOW);
//       delayMicroseconds(500);
//       //digitalWrite(raEnPin, LOW);
//       lastUpdateTime = currentTime;
//     }
//   }
//   else {
//     // Request a new observation every 5 seconds
//     if (millis() - lastUpdateTime >= 5000) {
//       Serial.write("obs", 4);
//       lastUpdateTime = millis();
//     }
//   }
// }