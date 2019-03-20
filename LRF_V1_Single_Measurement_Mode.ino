// Please connect your LRF module with arduino like that
// The LCD display is just an option because the resylt will be shown also in your arduino terminal! You can remove it if you want ;)
// Please note that your LRF module should be POWERED with 3.3v VCC

// * LCD RS pin to digital pin 12
// * LCD Enable pin to digital pin 11
// * LCD D4 pin to digital pin 7
// * LCD D5 pin to digital pin 6
// * LCD D6 pin to digital pin 5
// * LCD D7 pin to digital pin 4
// * LCD R/W pin to ground
// * LCD VSS pin to ground
// * LCD VCC pin to 5V

// * LRF RX pin to D10
// * LRF TX pin to D0

// * Push button (Trigger) pins on D8 & GND (D8 is already configured to internal pullup Mode)

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

// This software serial will be used to send instruction to the LRF module (with the Tx pin/D10 only,, the Rx pin will not be used)
// If you are using an arduino with multipl built-in serial ports like arduino Mega, then ther is no need to use a software serial!! just replace it with a free SerialPort
SoftwareSerial LRF(9, 10); // RX(D9) / TX(D10)

// Variables
int Trigger = 8; // For the Push button/Trigger that will activate the single mode measurement
int led = 13;
int counter = 0;
boolean Validation = false;
boolean hold_trigger = false;

String inputString = "";         // This string variable  ill save the incoming data from the LRF module
String result = "";              // This string variable will save the measured distance in Metter (m)
String checkReply = "";          // This string variable will save the first 6 character to check if its the LRF reply of the asked measurement (OK confirmation reply)
boolean stringComplete = false;  // this bolean will change when the LRF feedback is complete

void setup()
{
  // setup the LCD
  lcd.begin(16, 2);

  // setup the arduino Uno Serial Port(Tx0/Rx0)
  Serial.begin(115200);

  // setup the Software serial port (The software serial will just be used to send instruction to the LRF module)
  LRF.begin(115200);
  LRF.print("$0003260130&"); // LRF light pointer ON

 // lcd.clear();
 // lcd.print("LRF Sensor");

// setup the pin D8 as a pullup Input pin
  pinMode(Trigger, INPUT_PULLUP);
  
  pinMode(led, OUTPUT);
}

void loop()
{
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(100);


  /* add main program code here */
  if (digitalRead(Trigger) == 0 & hold_trigger == false) {

    lcd.clear();
    lcd.print("LRF Measuring...");

    digitalWrite(led, HIGH);
    LRF.print("$00022123&");
  }

  if (stringComplete & counter == 1 & inputString == "$00023335&") {
    Serial.println("Valide Measurement");
    stringComplete = false;
    inputString = "";
    Validation = true;
    hold_trigger = true;
  }

  if (stringComplete & counter == 2 & Validation) {
    Serial.println(inputString); //$0006210000239444&

    m_mm();




    stringComplete = false;
    inputString = "";
    result = "";
    checkReply = "";

    counter = 0;
    Validation = false;
    hold_trigger = false;

    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);


    LRF.print("$0003260130&"); // LRF light pointer ON
    delay(100);

    while (Serial.available())
      char clean = (char)Serial.read();
  }
}

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;

    if (inChar == '&') {
      stringComplete = true;
      counter++;
    }
  }
}

void m_mm() { // convert data to metter & millimetter values
  result += inputString[10];
  result += inputString[11];
  result += '.';
  result += inputString[12];
  result += inputString[13];
  result += inputString[14];
  result += inputString[15];
  result += inputString[16];
  result += " M";

  // set the cursor to column 0, line 0
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.clear();
  lcd.setCursor(0, 0);
  // print data
  lcd.print(result); // in metter

  Serial.println(result);
  result = "";

  result += inputString[10];
  result += inputString[11];
  result += inputString[12];
  result += inputString[13];
  result += inputString[14];
  result += '.';
  result += inputString[15];
  result += inputString[16];
  result += " MM";

  // set the cursor to column 0, line 0
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print data
  lcd.print(result); // in millimetter

  Serial.println(result);

}
