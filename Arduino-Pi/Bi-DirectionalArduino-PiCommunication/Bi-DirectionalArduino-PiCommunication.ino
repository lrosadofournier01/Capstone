const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  RPiToArduino();
  ArduinoToRPi();
}

void ArduinoToRPi(){
  Serial.println("Sending message to Raspberry Pi...");
  Serial.println("Hello from the Arduino Nano!");
  delay(1000);
}

void RPiToArduino() {
  recvWithEndMarker();
  showNewData();
  delay(1000);
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
 
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChars);
    newData = false;
  }
}
