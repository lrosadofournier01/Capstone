//droneZoneMainGround.ino
//Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

//Get Joystick values and send over NRF: NRF -->
//Get calculations over NRF:             NRF <--
//Send calculations to RPI:              RPI -->

#include <SPI.h> //NRF
#include <nRF24L01.h> //NRF
#include <RF24.h> //NRF

//NRF
//Set CE and CSN
RF24 radio(7, 8);
//Set the communication pipes
const byte addresses[][6] = {"00001", "00002"};
//Array to hold the calculations recieved over NRF. Don't know what data type to use
char calculations[32];

//joystick
//Define pin inputs
int x_key = A1;
int y_key = A0;
int click_key = 4;
//Define array to hold the joystick values
int joyArray[3];

void setup() {
  //Arduino-Pi
  //Set baud rate
  Serial.begin(9600);

  //NRF
  //Start the radio
  radio.begin();
  //Open writing on pipe 00001
  radio.openWritingPipe(addresses[1]);
  //Open reading on pipe 00002
  radio.openReadingPipe(1, addresses[0]);
  //Set the PA level to min
  radio.setPALevel(RF24_PA_MIN);

  //Joystick
  //Set the inputs
  pinMode(x_key, INPUT);
  pinMode(y_key, INPUT);
}

void loop() {
  //Get joystick values
  //Set the joystick values to the array
  joyArray[0] = analogRead(x_key);
  joyArray[1] = analogRead(y_key);
  joyArray[2] = digitalRead(click_key);

  //Send Joystick values over NRF
  //Need to delay
  delay(5);
  //Stop listening to send the message
  radio.stopListening();
  //Write the values to the radio

  //Wait for the calculations to be done and sent
  delay(3000);

  //Get calculations over NRF
  //Need to delay
  delay(5);
  //Start listening to get the message
  radio.startListening();
  //If the radio is avilable, read in the calculations
  if (radio.available()) {
    radio.read(&calculations, sizeof(calculations));
  }

  //Send the calculations to the RPI
  Serial.println(calculations);
}
