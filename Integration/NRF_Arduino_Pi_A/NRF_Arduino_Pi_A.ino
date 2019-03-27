/*
--------------------------------------------------------------
This is a sketch that recieves a message from Raspberry Pi A,
sends it to Arduino Nano A over the serial port, and Arduino
Nano A sends it over a radio to Arduino Nano B. It then waits
for a reply from Arduino Nano B and sends this reply back to
Raspberry Pi A.

Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
--------------------------------------------------------------
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//NRF
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"}; //Communication pipes
//Arduino-Pi Communication
const byte numChars = 32;
char rPiAChars[numChars]; // an array to store data from Raspberry Pi A
char arduinoNanoBChars[32] = ""; // an array to store data from Arduino Nano B
boolean newData = false;

void setup() {
  Serial.begin(9600);
  //Set up NRF
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  //Raspberry Pi A sends a message to Arduino Nano A
  RPiToArduino();
  //Arduino Nano A sends the message to Arduino Nano B
  NRFSendMsg();
  //Need to wait for Raspberry Pi B to get the message and reply
  delay(4000);
  //Arduino Nano A gets the reply from Arduino Nano B
  NRFGetReply();
  //Raspberry Pi A gets the reply from Arduino Nano A
  ArduinoToRPi();
  //Wait before doing the loop again
  delay(5000);
}

/*
--------------------------------------------------
NRF communication between the two Arduino Nano's
--------------------------------------------------
*/
//Send message to Arduino Nano B
void NRFSendMsg(){
  delay(5);
  radio.stopListening();
  radio.write(&rPiAChars, sizeof(rPiAChars));
}

//Get reply from Arduino Nano B
void NRFGetReply(){
  delay(5);
  radio.startListening();
  if (radio.available()) {
    radio.read(&arduinoNanoBChars, sizeof(arduinoNanoBChars));
  }
}

/*
--------------------------------------------------------
Communication between the Raspberry Pi and Arduino Nano
--------------------------------------------------------
*/
//Function to recieve a message from Raspberry Pi A over the Serial port
void RPiToArduino() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      rPiAChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      rPiAChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

//Function to send a message to Raspberry Pi A over the Serial port
void ArduinoToRPi(){
  Serial.println(arduinoNanoBChars);
}
