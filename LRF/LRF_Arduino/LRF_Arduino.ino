void setup() {
  Serial.begin(19200);
}

void loop() {
  //Char to turn on the laser, O
  char instruction = 79;
  //Write the instruction to turn on the laser to the serial
  Serial.write(instruction);
  //Get the laser value
  //String LRFvalue = Serial.readString();
  //Print the values
  //Serial.println(LRFvalue);

  //Char to get the distance, D
  instruction = 68;
  //Write the instruction to the serial
  Serial.write(instruction);
  //Get the laser value
  String LRFvalue = Serial.readString();
  //Give the Laser 3 seconds
  delay(3000);

  //Remove redundant charachters
  //LRFvalue = LRFvalue.trim('D: ');
  //LRFvalue = LRFvalue.trim('m,');
  //Print the values
  Serial.print("Number: ");
  Serial.println(LRFvalue);
}
