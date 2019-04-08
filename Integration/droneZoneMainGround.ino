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
int joyClick = 4;
int joyArray[3];//Array to hold values of the position

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
  pinMode (joyClick, INPUT) ;
}

void loop() {
  joyArray[0] = analogRead (x_key) ; //set the analog input to indicies of the array
  joyArray[1] = analogRead (y_key) ;
  joyArray[2] = digitalRead(joyClick);
  //Send joystick instruction
  radio.write(&joyArray, sizeof(joyArray)); //send the array
  delay(100);
  //Get calculations
  radio.startListening();
  if (radio.available()) {
    radio.read(&arduinoNanoBChars, sizeof(arduinoNanoBChars));
  }
  //Send to RPi
  Serial.println(arduinoNanoBChars);
}
