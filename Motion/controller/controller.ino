//defines pins
const int raStepPin = 6;  //RA PUL - Pulse
const int raDirPin = 7;   //RA DIR - Direction
const int raEnPin = 8;    //RA ENA - Enable
const int decStepPin = 10;  //DEC PUL - Pulse
const int decDirPin = 11;   //DEC DIR - Direction
const int decEnPin = 12;    //DEC ENA - Enable

const unsigned long siderealRateDelay = 16621; // microseconds delay between each RA microstep

void setup() {
  // Sets the pins as Outputs
  pinMode(raStepPin, OUTPUT); 
  pinMode(raDirPin, OUTPUT);
  pinMode(raEnPin, OUTPUT);
  pinMode(decStepPin, OUTPUT); 
  pinMode(decDirPin, OUTPUT);
  pinMode(decEnPin, OUTPUT);
  digitalWrite(raEnPin, LOW);
  digitalWrite(decEnPin, LOW);
}

void loop() {
  static unsigned long lastUpdateTime = 0;
  static bool isTracking = false;

  if (Serial.available() > 0) {
    int raMicrosteps = 0;
    int decMicrosteps = 0;
    int readBytes = Serial.readBytes((char *)&raMicrosteps, sizeof(raMicrosteps));
    if (readBytes == sizeof(raMicrosteps)) {
      Serial.readBytes((char *)&decMicrosteps, sizeof(decMicrosteps));

      // Set the direction of the motors
      digitalWrite(raDirPin, raMicrosteps > 0 ? HIGH : LOW);
      digitalWrite(decDirPin, decMicrosteps > 0 ? HIGH : LOW);

      // Enable the motors
      digitalWrite(raEnPin, HIGH);
      digitalWrite(decEnPin, HIGH);

      // Move the motors
      for (int i = 0; i < abs(raMicrosteps); i++) {
        digitalWrite(raStepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(raStepPin, LOW);
        delayMicroseconds(500);
      }
      for (int i = 0; i < abs(decMicrosteps); i++) {
        digitalWrite(decStepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(decStepPin, LOW);
        delayMicroseconds(500);
      }

      // Disable the motors
      digitalWrite(raEnPin, LOW);
      digitalWrite(decEnPin, LOW);

      // Indicate that the instruction has been completed
      Serial.write("done", 4);
      
      // Start tracking if the instruction is 0 steps
      if (raMicrosteps == 0 && decMicrosteps == 0) {
        isTracking = true;
      }
      else {
        isTracking = false;
      }
    }
  }

  if (isTracking) {
    // Only update the RA motor when the sidereal time has elapsed
    unsigned long currentTime = micros();
    if (currentTime - lastUpdateTime >= siderealRateDelay) {
      digitalWrite(raEnPin, HIGH);
      digitalWrite(raDirPin, HIGH);
      digitalWrite(raStepPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(raStepPin, LOW);
      delayMicroseconds(500);
      digitalWrite(raEnPin, LOW);
      lastUpdateTime = currentTime;
    }
  }
  else {
    // Request a new observation every 5 seconds
    if (millis() - lastUpdateTime >= 5000) {
      Serial.write("obs", 4);
      lastUpdateTime = millis();
    }
  }
}