//defines pins
const int raStepPin = 6;  //RA PUL - Pulse
const int raDirPin = 7;   //RA DIR - Direction
const int raEnPin = 8;    //RA ENA - Enable
const int decStepPin = 10;  //DEC PUL - Pulse
const int decDirPin = 11;   //DEC DIR - Direction
const int decEnPin = 12;    //DEC ENA - Enable

const float siderealSecs = 86164.0905; //seconds in a sidereal day
const int reduction = 30 * 27; //total gear reduction
const int microsteps = 32; //current microstep setting

const unsigned long siderealRate = 360 * (1/siderealSecs); //degrees per second for tracking
const unsigned long siderealRateDelay = 1/(2 * siderealRate * reduction // microseconds delay between each RA microstep
                                           * microsteps / 1000000);  //divide by 2 since delay both before and after 

const float stepsPerDeg = reduction/(1.8/microsteps)

void setup() {
  // Sets the pins as Outputs
  pinMode(raStepPin, OUTPUT); 
  pinMode(raDirPin, OUTPUT);
  //pinMode(raEnPin, OUTPUT);
  pinMode(decStepPin, OUTPUT); 
  pinMode(decDirPin, OUTPUT);
  //pinMode(decEnPin, OUTPUT);
  //digitalWrite(raEnPin, LOW);
  //digitalWrite(decEnPin, LOW);
}

void loop() {
  static unsigned long lastUpdateTime = 0;
  static bool isTracking = false;

  if (Serial.available() > 0) {
    int raError = 0;
    int decError = 0;

    //check the below
    int readBytes = Serial.readBytes((char *)&raError, sizeof(raError));
    if (readBytes == sizeof(raError)) {
      Serial.readBytes((char *)&decError, sizeof(decError));

      // Set the direction of the motors
      digitalWrite(raDirPin, raError > 0 ? HIGH : LOW);
      digitalWrite(decDirPin, decError > 0 ? HIGH : LOW);

      // Enable the motors check if want do this
      //digitalWrite(raEnPin, HIGH);
      //digitalWrite(decEnPin, HIGH);

      // Move the motors
      for (int i = 0; i < abs(raError * stepsPerDeg); i++) {
        digitalWrite(raStepPin, HIGH);
        delayMicroseconds(250);
        digitalWrite(raStepPin, LOW);
        delayMicroseconds(250);
      }
      for (int i = 0; i < abs(decError * stepsPerDeg); i++) {
        digitalWrite(decStepPin, HIGH);
        delayMicroseconds(250);
        digitalWrite(decStepPin, LOW);
        delayMicroseconds(250);
      }

      // Disable the motors
      //digitalWrite(raEnPin, LOW);
      //digitalWrite(decEnPin, LOW);

      // Indicate that the instruction has been completed
      Serial.write("done", 4);
      
      // Start tracking if the instruction is 0 steps
      if (raError == 0 && decError == 0) {
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