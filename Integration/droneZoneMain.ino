//Gets joystick instructions
//Moves servos
//Sends joystick click to the RPi
//Gets the calculations from the RPi
//Sends the calculations
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
//Servo's
Servo servo1;
Servo servo2;
int servo1_pin = 9; //pins the servos are connected to
int servo2_pin = 10;
//NRF
RF24 radio(7, 8); // CSN, CE
const byte addresses[][6] = {"00001", "00002"}; //Communication pipes
//Arduino-Pi
const byte numChars = 32;
char rPiAChars[numChars]; // an array to store data from Raspberry Pi A
boolean newData = false;

void setup() {
  //Serial
  Serial.begin(9600);
  //NRF
  radio.begin(); //start radio
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  //Servos
  servo1.attach (servo1_pin ) ; //attach servos to pins
  servo2.attach (servo2_pin ) ;

}

void loop() {
  radio.startListening();
  if (radio.available()) {
    int joyArray[3]; //array to hold analog joystick values
    //Get joystick values
    radio.read(&joyArray, sizeof(joyArray));

    //Move Servos
    for (byte i=0; i<2; i++){ //print them to see what the values are for troubleshooting
      Serial.println(joyArray[i]);
    }
    Serial.println();

    joyArray[0] = map(joyArray[0], 0, 1023, 0, 180); //scale down the values
    joyArray[1] = map(joyArray[1], 0, 1023, 0, 180);

    if (joyArray[0]>400 && joyArray[0]<600) {
      //do nothing because that is the servo's resting state
    }
    else {
      servo1.write (joyArray[0]) ;
    }
    if (joyArray[1]>400 && joyArray[1]<600) {
      //do nothing because that is the servo's resting state
    }
    else {
      servo2.write (joyArray[1]) ;
    }

    //Send Joystick click to RPi
    Serial.println(joyArray[2]);

    //Gets calculation from RPi
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
  //Sends calculations
  delay(5);
  radio.stopListening();
  radio.write(&rPiAChars, sizeof(rPiAChars));
}
