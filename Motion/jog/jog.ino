// defines pins
const int raStepPin = 15;  // RA PUL - RA Pulse
const int raDirPin = 32;   // RA DIR - RA Direction
//const int raEnPin = 14;    // RA ENA - RA Enable


//FIX THESE BEFORE RUNNING!!!!!!!

const int decStepPin = 6;  // Dec PUL - Dec Pulse FIX THESE BEFORE RUNNING!!!!!!!
const int decDirPin = 7;   // Dec DIR - Dec Direction
//const int decEnPin = 8;    // Dec ENA - Dec Enable

const int reduction = 30 * 27; //total gear reduction
const int microsteps = 32; //current microstep setting

int stepPin;
int dirPin;
int enPin;




void setup() {
  // sets the pins as Outputs
  pinMode(raStepPin, OUTPUT); 
  pinMode(raDirPin, OUTPUT);

  pinMode(decStepPin, OUTPUT); 
  pinMode(raDirPin, OUTPUT);
  //pinMode(enPin, OUTPUT);
  //digitalWrite(enPin, LOW);
  // initialize serial communication
  Serial.begin(115200);
}

void loop() {

  //wait until motor selection comes in
  while (Serial.available() == 0)  {
    delay(100);    
  }

  String motorData = Serial.readStringUntil('\n');
  int motorNum = motorData.toInt();

  //wait until degrees to turn comes in 
  while (Serial.available() == 0)  {
    delay(100);    
  }

  String stepData = Serial.readStringUntil('\n');
  int steps = stepData.toInt();

  if (motorNum == 0) {
    stepPin = raStepPin;
    dirPin = raStepPin;
    //enPin = raEnPin;

  } else {
    stepPin = decStepPin;
    dirPin = decStepPin;
    //enPin = decEnPin;    
  }
    
  digitalWrite(dirPin, steps > 0 ? HIGH : LOW);
  // make pulses to rotate the motor
  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
  }
    
  // send response to Python
  Serial.println("Motor rotated " + String(steps) + " microsteps");
  
}