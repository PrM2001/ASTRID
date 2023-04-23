// defines pins
const int raStepPin = 15;  // RA PUL - RA Pulse
const int raDirPin = 33;   // RA DIR - RA Direction
//const int raEnPin = 14;    // RA ENA - RA Enable


//FIX THESE BEFORE RUNNING!!!!!!!

const int decStepPin = 32;  // Dec PUL - Dec Pulse FIX THESE BEFORE RUNNING!!!!!!!
const int decDirPin = 14;   // Dec DIR - Dec Direction
//const int decEnPin = 8;    // Dec ENA - Dec Enable

const int reduction = 30 * 26.85; //total gear reduction
const int microsteps = 1; //current microstep setting

String raStepsStr;
String decStepsStr;

int raStepsInt;
int decStepsInt;


void setup() {
  if (Serial.available()){
    raStepsStr = Serial.readString();
    Serial.print("Received RA Value: ");
    Serial.println(raStepsStr);
    raStepsInt = raStepsStr.toInt();
    
<<<<<<< HEAD
    while (!Serial.available()) {
      delay(1)
=======
    while !(Serial.available()) {
      delay(1);
>>>>>>> 1a84ddbb38f821405878c94aa9bd33e5ce93db0d
    }

    decStepsStr = Serial.readString();
    Serial.print("Received Dec Value: ");
    Serial.println(decStepsStr);
    decStepsInt = decStepsStr.toInt();
  }

  if (raStepsInt > 0){
    digitalWrite(raDirPin,HIGH);
  }
  else {
    digitalWrite(raDirPin,LOW);    
  }
  
  for (int i = 0; i < abs(raStepsInt); i++) {
    
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(200); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(200);
    delay(1);
  }

  if (decStepsInt > 0){
    digitalWrite(decDirPin,HIGH);
  }
  else {
    digitalWrite(decDirPin,LOW);    
  }

  for (int i = 0; i < abs(decStepsInt); i++)) {
    digitalWrite(dirPin,HIGH);
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(200); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(200);
    delay(1);  
  }
}




// void setup() {
//   // sets the pins as Outputs
//   pinMode(raStepPin, OUTPUT); 
//   pinMode(raDirPin, OUTPUT);

//   pinMode(decStepPin, OUTPUT); 
//   pinMode(raDirPin, OUTPUT);
//   //pinMode(enPin, OUTPUT);
//   //digitalWrite(enPin, LOW);
//   // initialize serial communication
//   Serial.begin(115200);
// }

// void loop() {

//   //wait until motor selection comes in
//   while (Serial.available() == 0)  {
//     delay(100);    
//   }

//   String motorData = Serial.readStringUntil('\n');
//   int motorNum = motorData.toInt();

//   //wait until degrees to turn comes in 
//   while (Serial.available() == 0)  {
//     delay(100);    
//   }

//   String stepData = Serial.readStringUntil('\n');
//   int steps = stepData.toInt();

//   if (motorNum == 0) {
//     stepPin = raStepPin;
//     dirPin = raStepPin;
//     //enPin = raEnPin;

//   } else {
//     stepPin = decStepPin;
//     dirPin = decStepPin;
//     //enPin = decEnPin;    
//   }
    
//   digitalWrite(dirPin, steps > 0 ? HIGH : LOW);
//   // make pulses to rotate the motor
//   for (int i = 0; i < abs(steps); i++) {
//     digitalWrite(stepPin, HIGH);
//     delayMicroseconds(100);
//     digitalWrite(stepPin, LOW);
//     delayMicroseconds(100);
//   }
  
// }