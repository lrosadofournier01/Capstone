/*
--------------------------------------------------------------
This is a sketch that recieves a message from Arduino Nano A
over radio and sends it to Raspberry Pi B over the serial port.
Raspberry Pi B then sends a reply back to Arduino Nano B and
Arduino Nano B sends that to Arduino Nano A over radio.

Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
--------------------------------------------------------------
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//NRF
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"}; //Communication Pipes
//Arduino-Pi Communication
const byte numChars = 32;
char rPiBChars[numChars]; // an array to store data from Raspberry Pi B
char arduinoNanoAChars[32] = ""; // an array to store data from Arduino Nano A
boolean newData = false;

void setup() {
  Serial.begin(9600);
  //Set up NRF
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  //Need to wait for Rasspberry Pi A to send the message
  delay(2000);
  //Arduino Nano B gets the msg from Arduino Nano A
  NRFGetMsg();
  //Raspberry Pi B gets the msg from Arduino Nano B
  ArduinoToRPi();
  //Raspberry Pi B sends a reply to Arduino Nano B
  RPiToArduino();
  //Arduino Nano B sends the reply to Arduino Nano A
  NRFSendReply();
  //Wait before doing the loop again
  delay(5000);
}

/*
--------------------------------------------------
NRF communication between the two Arduino Nano's
--------------------------------------------------
*/
//Get message from Arduino Nano A
void NRFGetMsg(){
  delay(5);
  radio.startListening();
  if (radio.available()) {
    radio.read(&arduinoNanoAChars, sizeof(arduinoNanoAChars));
  }
}

//Send reply to Arduino Nano A
void NRFSendReply(){
  delay(5);
  radio.stopListening();
  radio.write(&rPiBChars, sizeof(rPiBChars));
}
/*
--------------------------------------------------------
Communication between the Raspberry Pi and Arduino Nano
--------------------------------------------------------
*/
//Function to send a message to Raspberry Pi B over the Serial port
void ArduinoToRPi(){
  Serial.println(arduinoNanoAChars);
}

//Function to recieve a reply from Raspberry Pi B over the Serial port
void RPiToArduino() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      rPiBChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      rPiBChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
