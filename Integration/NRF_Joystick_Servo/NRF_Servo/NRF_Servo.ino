#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";
int servo1_pin = 9; //pins the servos are connected to
int servo2_pin = 10;

void setup() {
  Serial.begin(9600);
  radio.begin(); //start radio
  servo1.attach (servo1_pin ) ; //attach servos to pins
  servo2.attach (servo2_pin ) ; 
  radio.openReadingPipe(0, address); //set reading address
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    int posArray[2]; //array to hold analog joystick values
    radio.read(&posArray, sizeof(posArray)); //recieve the joystick values
    
    for (byte i=0; i<2; i++){ //print them to see what the values are for troubleshooting
      Serial.println(posArray[i]);
    }
    Serial.println();
    
    posArray[0] = map(posArray[0], 0, 1023, 0, 180); //scale down the values
    posArray[1] = map(posArray[1], 0, 1023, 0, 180);
    
    if (posArray[0]>400 && posArray[0]<600)
    {
      //do nothing because that is the servo's resting state
    }
    else{
    servo1.write (posArray[0]) ;
    }
    if (posArray[1]>400 && posArray[1]<600)
    {
      //do nothing because that is the servo's resting state
    }
    else{
    servo2.write (posArray[1]) ;
    }
  }
}
