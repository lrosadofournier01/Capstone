//Sends joystick instructions
//Gets Calculations
//send calculations to RPi
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//NRF
RF24 radio(7, 8); // CSN, CE
const byte addresses[][6] = {"00001", "00002"}; //Communication pipes
//Arduino-Pi
const byte numChars = 32;
char arduinoNanoBChars[numChars] = ""; // an array to store data from Arduino Nano B
boolean newData = false;
//joystick
int x_key = A1; //Defined analog inputs
int y_key = A0;
int posArray[2];//Array to hold values of the position

void setup() {
  //NRF
  radio.begin(); //Start radio
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  //Joystick
  pinMode (x_key, INPUT) ; //set the analog inputs
  pinMode (y_key, INPUT) ;
}

void loop() {
  posArray[0] = analogRead (x_key) ; //set the analog input to indicies of the array
  posArray[1] = analogRead (y_key) ;
  //Send joystick instruction
  radio.write(&posArray, sizeof(posArray)); //send the array
  delay(100);
  //Get calculations
  radio.startListening();
  if (radio.available()) {
    radio.read(&arduinoNanoBChars, sizeof(arduinoNanoBChars));
  }
  //Send to RPi
  Serial.println(arduinoNanoBChars);
}
