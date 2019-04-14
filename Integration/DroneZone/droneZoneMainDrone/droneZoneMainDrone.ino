//droneZoneMainDrone.ino
//Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

//Get joystick values over NRF:  NRF <--
//Move servos
//Send joystick click to RPI:    RPI -->
//Get the calculations from RPI: RPI <--
//Send calculations over NRF:    NRF -->

#include <SPI.h> //NRF
#include <nRF24L01.h> //NRF
#include <RF24.h> //NRF
#include <Servo.h> //Servo

//Servo
//Declare the 2 servos
Servo servo1;
Servo servo2;
//Set the pins the servos are connected to
int servo1_pin = 9;
int servo2_pin = 10;

//NRF
//Set CE and CSN
RF24 radio(7, 8);
//Set communication pipes
const bye addresses[][6] = {"00001", "00002"};
//Define array to hold joy stick values sent over NRF
int joyArray[32];

//Arduino-Pi
//Set array to hold calculations from RPI
char calculations[32];
//Set boolean to see if there's new data
boolean newData = false;

void setup() {
  //Arduino-Pi
  //Set baud rate
  Serial.begin(9600);

  //NRF
  //Start the radio
  radio.begin();
  //Open writing on pipe 00002
  radio.openWritingPipe(addresses[0]);
  //Open reading on pipe 00001
  radio.openReadingPipe(1, addresses[1]);
  //Set PA level to min
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  //Wait for the ground Arduino to send the click
  delay(1000);
  
  //Get joystick values over NRF
  //Need to delay
  delay(5);
  //Start listening to get the joystick values
  radio.startListening();
  //If the radio is available, read in the joystick values
  if (radio.available()) {
    radio.read(&joyArray, sizeof(joyArray));
  }

  //Move servos
  //Scale down joystick values
  joyArray[0] = map(joyArray[0], 0, 1023, 0, 180)
  joyArray[1] = map(joyArray[1], 0, 1023, 0, 180)
  //If the x value is between 400 and 600, do nothing because that's the joysticks resting state
  if (joyArray[0] > 400 && joyArray[0] < 600) {
    //Do nothing
  } else {
    //Move servo's based on the joyArray values
    servo1.write(joyArray[0]);
  }
  //If the y value is between 400 and 600, do nothing because that's the joysticks resting state
  if (joyArray[1] > 400 && joyArray[1] < 600) {
    //Do nothing
  } else {
    //Move servo's based on the joyArray values
    servo2.write(joyArray[1]);
  }

  //Send joystick click to RPI
  Serial.println(joyArray[2]);

  //Wait for RPI to do calculations
  delay(1000);

  //Get calculations from RPI
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (rc != endMarker) {
      calculations[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      calulations[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }

  //Send calculations over NRF
  //Need to delay
  delay(5);
  //Stop listening to send
  radio.stopListening();
  //write calculations to the radio
  radio.write(&calculations, sizeof(calculations))
}
