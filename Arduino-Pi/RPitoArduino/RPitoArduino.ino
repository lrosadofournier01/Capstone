String msg;
//char c;
void setup() {
  Serial.begin(9600);
}

void loop() {
  //c = Serial.read();  //conveting the value of chars to integer
  while(Serial.available()> 0){
    msg = Serial.readString();
  }
  //if(Serial.available()){         //From RPi to Arduino
  Serial.println(msg);
  //}
  delay(1000);
}
