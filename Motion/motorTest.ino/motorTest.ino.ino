// defines pins
const int stepPin = 15;  // PUL - Pulse
const int dirPin = 32;   // DIR - Direction
//const int enPin = 14;    // ENA - Enable
const int idleDelay = 250;  // delay in microseconds for idle mode (32 microsteps per second)
const int idleRequestDelay = 5000;  // delay in milliseconds to request new input during idle mode

void setup() {
  // sets the pins as Outputs
  pinMode(stepPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
  //pinMode(enPin, OUTPUT);
  //digitalWrite(enPin, LOW);
  // initialize serial communication
  Serial.begin(115200);
}

void loop() {
  // check for incoming serial data
  if (Serial.available() > 0) {
    // read the incoming data
    String data = Serial.readStringUntil('\n');
    // convert the data to an integer value
    int steps = data.toInt();
    // enable motor direction to move
    digitalWrite(dirPin, HIGH);
    // make pulses to rotate the motor
    for (int i = 0; i < steps; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
    // send response to Python
    Serial.println("Motor rotated " + String(steps) + " microsteps");
  } else {
    // idle mode
    digitalWrite(dirPin, LOW);  // change direction to prevent cumulative error
    unsigned long startTime = millis();  // get start time
    while (millis() - startTime < idleRequestDelay) {
      if (Serial.available() > 0) {
        // read the incoming data
        String data = Serial.readStringUntil('\n');

        // convert the data to an integer value
        int steps = data.toInt();
        // enable motor direction to move
        digitalWrite(dirPin, HIGH);
        // make pulses to rotate the motor
        for (int i = 0; i < steps; i++) {
          digitalWrite(stepPin, HIGH);
          delayMicroseconds(500);
          digitalWrite(stepPin, LOW);
          delayMicroseconds(500);
          
        }
        // send response to Python
        Serial.println("Motor rotated " + String(steps) + " microsteps");
        break;  // exit while loop if new input received
      }
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(idleDelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(idleDelay);
      // for(int x = 0; x < 200 * 27; x++){
      //   digitalWrite(stepPin,HIGH); 
      //   delayMicroseconds(250); 
      //   digitalWrite(stepPin,LOW); 
      //   delayMicroseconds(250); 
      //   Serial.println(String(x));
      //}      
    }
  }
}