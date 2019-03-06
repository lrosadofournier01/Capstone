String msg;
char c;
void setup() {
  Serial.begin(9600);
}

void loop() {
  c = Serial.read();  //conveting the value of chars to integer
  msg = String(msg + c);
  if(Serial.available()){         //From RPi to Arduino
    Serial.println(msg);
  }
  delay(1000);
}
