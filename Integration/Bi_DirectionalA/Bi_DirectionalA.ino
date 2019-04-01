#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";

int x_key = A1; //Defined analog inputs                                               
int y_key = A0;                                               
int posArray[2];//Array to hold values of the position

void setup() {
  radio.begin(); //Start radio
  radio.openWritingPipe(address); //set the write address
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode (x_key, INPUT) ; //set the analog inputs                    
  pinMode (y_key, INPUT) ;    
}

void loop() {
  posArray[0] = analogRead (x_key) ; //set the analog input to indicies of the array 
  posArray[1] = analogRead (y_key) ;   
  radio.write(&posArray, sizeof(posArray)); //send the array
  delay(100);
}
