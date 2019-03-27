/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to set pin numbers:
const int button1 = 5;     // the number of the pushbutton pin
const int button2=6;
const int button3=7;
const int button4=8;
const int button5=9;
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button1, OUTPUT);
  pinMode(button2, OUTPUT);
  pinMode(button3, OUTPUT);
  pinMode(button4, OUTPUT);
  pinMode(button5, OUTPUT);
}

void loop() {
  // read the state of the pushbutton value:
 
    digitalWrite(button1, LOW);
    delay(2000);
    Serial.println("Button 1");
    digitalWrite(button2, LOW);
    delay(2000);
    Serial.println("Button 2");
    digitalWrite(button3, LOW);
    delay(2000);
    Serial.println("Button 3");
    digitalWrite(button4, LOW);
    delay(2000);
    Serial.println("Button 4");
    digitalWrite(button5, LOW);
    delay(2000);
    Serial.println("Button 5");
  
}
