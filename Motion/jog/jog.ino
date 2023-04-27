// defines pins
const int raStepPin = 27;  // RA PUL - RA Pulse
const int raDirPin = 33;   // RA DIR - RA Direction



//FIX THESE BEFORE RUNNING!!!!!!!

const int decStepPin = 15;  // Dec PUL - Dec Pulse FIX THESE BEFORE RUNNING!!!!!!!
const int decDirPin = 32;   // Dec DIR - Dec Direction


const int reduction = 30 * 26.85 ; //total gear reduction
const int microsteps = 1; //current microstep setting

String raStepsStr;
String decStepsStr; 

int raStepsInt;
int decStepsInt;

void setup() {
  // sets the pins as Outputs
  pinMode(raStepPin, OUTPUT); 
  pinMode(raDirPin, OUTPUT);

  pinMode(decStepPin, OUTPUT); 
  pinMode(decDirPin, OUTPUT);
  //pinMode(enPin, OUTPUT);
  //digitalWrite(enPin, LOW);
  // initialize serial communication
  Serial.begin(115200);
}




void loop() {
  Serial.print(" ");

  while (!Serial.available()) {
    delay(1);
  }

  raStepsStr = Serial.readString();
  raStepsInt = raStepsStr.toInt();
  Serial.print("Received RA Value: ");
  Serial.println(raStepsStr);
  
  while (!Serial.available()) {
    delay(1);
  }

  decStepsStr = Serial.readString();
  decStepsInt = decStepsStr.toInt();
  Serial.print("Received Dec Value: ");
  Serial.println(decStepsStr);


  if (raStepsInt > 0){
    digitalWrite(raDirPin,HIGH);
  }
  else {
    digitalWrite(raDirPin,LOW);    
  }
  raStepsInt = abs(raStepsInt);

  if (decStepsInt < 0){
    digitalWrite(decDirPin,HIGH);
  }
  else {
    digitalWrite(decDirPin,LOW);    
  }
  decStepsInt = abs(decStepsInt);


  if (decStepsInt > raStepsInt) {
    for (int i = 0; i < raStepsInt; i++) { 
      digitalWrite(raStepPin,HIGH); 
      digitalWrite(decStepPin,HIGH); 
      delayMicroseconds(100); 
      digitalWrite(raStepPin,LOW); 
      digitalWrite(decStepPin,LOW); 
      delayMicroseconds(100);
      delay(0.5);
    }

    for (int i = 0; i < decStepsInt - raStepsInt; i++) {  
      digitalWrite(decStepPin,HIGH); 
      delayMicroseconds(100); 
      digitalWrite(decStepPin,LOW); 
      delayMicroseconds(100);
      delay(0.5);
    }
  }
  else {
    for (int i = 0; i < decStepsInt; i++) { 
      digitalWrite(raStepPin,HIGH); 
      digitalWrite(decStepPin,HIGH); 
      delayMicroseconds(100); 
      digitalWrite(raStepPin,LOW); 
      digitalWrite(decStepPin,LOW); 
      delayMicroseconds(100);
      delay(0.5);
    }

    for (int i = 0; i < raStepsInt - decStepsInt; i++) {  
      digitalWrite(raStepPin,HIGH); 
      delayMicroseconds(100); 
      digitalWrite(raStepPin,LOW); 
      delayMicroseconds(100);
      delay(0.5);
    }
  }

  

///////////////////////////////////////////
//The Code below is for sequential motion//
///////////////////////////////////////////

  // for (int i = 0; i < raStepsInt; i++) { 
  //   digitalWrite(raStepPin,HIGH); 
  //   digitalWrite(decStepPin,HIGH); 
  //   delayMicroseconds(200); 
  //   digitalWrite(raStepPin,LOW); 
  //   digitalWrite(decStepPin,LOW); 
  //   delayMicroseconds(200);
  //   delay(1);
  // }

  // for (int i = 0; i < decStepsInt; i++) {
  //   digitalWrite(decStepPin,HIGH); 
  //   delayMicroseconds(200); 
  //   digitalWrite(decStepPin,LOW); 
  //   delayMicroseconds(200);
  //   delay(1);  
  // }
}


// void loop() {
//   Serial.print(" ");

//   while (!Serial.available()) {
//     delay(1);
//   }

//   raStepsStr = Serial.readString();
//   raStepsInt = raStepsStr.toInt();
//   Serial.print("Received RA Value: ");
//   Serial.println(raStepsStr);
  
//   while (!Serial.available()) {
//     delay(1);
//   }

//   decStepsStr = Serial.readString();
//   decStepsInt = decStepsStr.toInt();
//   Serial.print("Received Dec Value: ");
//   Serial.println(decStepsStr);


//   if (raStepsInt > 0){
//     digitalWrite(raDirPin,HIGH);
//   }
//   else {
//     digitalWrite(raDirPin,LOW);    
//   }
  
//   for (int i = 0; i < abs(raStepsInt); i++) {
    
//     digitalWrite(raStepPin,HIGH); 
//     delayMicroseconds(200); 
//     digitalWrite(raStepPin,LOW); 
//     delayMicroseconds(200);
//     delay(1);
//   }

//   if (decStepsInt > 0){
//     digitalWrite(decDirPin,HIGH);
//   }
//   else {
//     digitalWrite(decDirPin,LOW);    
//   }

//   for (int i = 0; i < abs(decStepsInt); i++) {
//     digitalWrite(decStepPin,HIGH); 
//     delayMicroseconds(200); 
//     digitalWrite(decStepPin,LOW); 
//     delayMicroseconds(200);
//     delay(1);  
//   }
// }
