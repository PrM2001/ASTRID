// void setup() {
//   Serial.begin(115200); // Set baud rate
// }

// void loop() {
//   if (Serial.available()) {
//     int value = Serial.read(); // Read incoming data
//     Serial.println(Serial.read()); // Print received data
//   }

//   // if (Serial.availableForWrite()) {
//   //   Serial.write(Serial.read()); // Send data to Python
//   // }
// } 

void setup() {
  Serial.begin(115200);
}
 
void loop() {
 
  while(Serial.available()){
    Serial.write(Serial.read());
  }
 
}