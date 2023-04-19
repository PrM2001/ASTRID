// defines pins
const int stepPin = 6;  // PUL - Pulse
const int dirPin = 7;   // DIR - Direction
const int enPin = 8;    // ENA - Enable
const int idleDelay = 15625;  // delay in microseconds for idle mode (32 microsteps per second)
const int idleRequestDelay = 5000;  // delay in milliseconds to request new input during idle mode

void setup() {
  // sets the pins as Outputs
  pinMode(stepPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  // initialize serial communication
  Serial.begin(9600);
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
    }
  }
}