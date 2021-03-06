const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  RPiToArduino();
  ArduinoToRPi();
  //Wait before doing the loop again
  delay(5000);
}

void ArduinoToRPi(){
  Serial.print("Sending message to Raspberry Pi... ");
  Serial.println("Hello from the Arduino Nano!");
  //Wait for RPi to print the message
  delay(3000);
}

void RPiToArduino() {
  //Waiting for RPi to send message
  delay(3000);
  recvWithEndMarker();
  showNewData();
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
    //Serial.println("New Message... ");
    Serial.println(receivedChars);
    newData = false;
  }
}
