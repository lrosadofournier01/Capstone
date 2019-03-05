/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  //Set up serial port
  Serial.begin(9600);
  //Set up NRF
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  //Set up Serial port to Excel
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,Started Time,Registered Value");
  Serial.println("RESETTIMER");
}

void loop() {
  //Receive if nrf is available
  if (radio.available()) {
    //Serial.print("DATA,TIME,TIMER,");
    //Start timing the recieving part
    unsigned long start = micros();
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print(text);
    //Stop timing, calculate the elapsed time, and print it
    unsigned long end = micros();
    unsigned long timeElapsed = end - start;
    Serial.print(" ");
    Serial.println(timeElapsed);
  }
}
