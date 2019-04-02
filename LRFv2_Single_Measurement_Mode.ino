
byte cmd_2[] = { 0x80, 0x06, 0x02, 0x78 };         // Single Measurement Mode
byte cmd_3[] = { 0x80, 0x06, 0x05, 0x01, 0x74 };   // LaserPointerOn
byte cmd_4[] = { 0x80, 0x06, 0x05, 0x00, 0x75 };   // LaserPointerOff

byte cmd_5[] = { 0xFA, 0x04, 0x09, 0x05, 0xF4 };   // 5m Range
byte cmd_6[] = { 0xFA, 0x04, 0x09, 0x0A, 0xEF };   // 10m Range
byte cmd_7[] = { 0xFA, 0x04, 0x09, 0x1E, 0xDB };   // 30m Range
byte cmd_8[] = { 0xFA, 0x04, 0x09, 0x32, 0xC7 };   // 50m Range
byte cmd_9[] = { 0xFA, 0x04, 0x09, 0x50, 0xA9 };   // 80m Range

byte cmd_10[] = { 0xFA, 0x04, 0x0C, 0x02, 0xF4 };   // 0.1mm Resolution
byte cmd_11[] = { 0xFA, 0x04, 0x0C, 0x01, 0xF5 };   // 1mm Resolution



String LRF_feedback;  // Collect the RAW LRF Feedback
String measurement;   // Collect the Digital LRF Measurement

char data;

int Bytes_counter = 12;

#define USB Serial        // For Teriminal to print results
#define LRF Serial1       // LRF module connected on Serial1 of ARDUINO Micro

void setup() {
  // put your setup code here, to run once:

  USB.begin(9600);
   while (!USB) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  LRF.begin(9600);
  delay(1000);

  LRF.write(cmd_6, sizeof(cmd_6)); // Set the LRF Module Range to 10m
  USB.println("LRF Range Set to 10m");
  delay(1000);

  LRF.write(cmd_10, sizeof(cmd_10)); // Set the LRF Module Resolution to O.1mm
  USB.println("LRF Resolution Set to O.1mm");
  delay(1000);
}

void loop() {

  // ENABLE the Single measurement mode
  LRF.write(cmd_2, sizeof(cmd_2));
  delay(100);

  // Catch the measured distance from the LRF Module
  while (LRF.available() > 0 && Bytes_counter < 12) {
    data = LRF.read();
    LRF_feedback += data;
    Bytes_counter++;
  }


  if (Bytes_counter == 12) { // Feedback for the 0.1mm Resolution Measurement
    measurement = LRF_feedback.substring(3, 11);

    USB.print("Raw Feedback: ");
    USB.print(LRF_feedback);
    USB.print(" || Bytes Received: ");
    USB.print(Bytes_counter);
    USB.print(" || LRF Measurement: ");
    USB.print(measurement);
    USB.println(" DONE");
  }
  else if (Bytes_counter == 11) { // Feedback for the 1mm Resolution Measurement
    measurement = LRF_feedback.substring(3, 10);

    USB.print("Raw Feedback: ");
    USB.print(LRF_feedback);
    USB.print(" || Bytes Received: ");
    USB.print(Bytes_counter);
    USB.print(" || LRF Measurement: ");
    USB.print(measurement);
    USB.println(" DONE");
  }

  LRF_feedback = "";
  Bytes_counter = 0;

}
