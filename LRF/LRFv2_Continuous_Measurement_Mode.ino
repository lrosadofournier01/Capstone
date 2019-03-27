
byte cmd1[] = { 0x80, 0x06, 0x03, 0x77 };         // Continious Measurement Mode
byte cmd3[] = { 0x80, 0x06, 0x05, 0x01, 0x74 };   // LaserPointerOn
byte cmd4[] = { 0x80, 0x06, 0x05, 0x00, 0x75 };   // LaserPointerOff

byte cmd5[] = { 0xFA, 0x04, 0x0A, 0x00, 0xF8 };   // 2Hz
byte cmd6[] = { 0xFA, 0x04, 0x0A, 0x05, 0xF3 };   // 5Hz
byte cmd7[] = { 0xFA, 0x04, 0x0A, 0x0A, 0xEE };   // 10Hz
byte cmd8[] = { 0xFA, 0x04, 0x0A, 0x14, 0xE4 };   // 20Hz

byte cmd_9[] = { 0xFA, 0x04, 0x09, 0x05, 0xF4 };    // 5m Range
byte cmd_10[] = { 0xFA, 0x04, 0x09, 0x0A, 0xEF };   // 10m Range
byte cmd_11[] = { 0xFA, 0x04, 0x09, 0x1E, 0xDB };   // 30m Range
byte cmd_12[] = { 0xFA, 0x04, 0x09, 0x32, 0xC7 };   // 50m Range
byte cmd_13[] = { 0xFA, 0x04, 0x09, 0x50, 0xA9 };   // 80m Range

byte cmd_14[] = { 0xFA, 0x04, 0x0C, 0x02, 0xF4 };   // 0.1mm Resolution
byte cmd_15[] = { 0xFA, 0x04, 0x0C, 0x01, 0xF5 };   // 1mm Resolution



bool CMM_EN = true;   // Continious Measurement Mode Enabled

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

  LRF.write(cmd7, sizeof(cmd7)); // Set LRF Module Frequency to 10Hz
  delay(1000);

  LRF.write(cmd_10, sizeof(cmd_10)); // Set the LRF Module Range to 10m
  USB.println("LRF Range Set to 10m");
  delay(1000);

  LRF.write(cmd_14, sizeof(cmd_14)); // Set the LRF Module Resolution to O.1mm
  USB.println("LRF Resolution Set to O.1mm");
  delay(1000);

}

void loop() {

  // ENABLE the Continious Measurement mode for one time only
  if (CMM_EN) {
    LRF.write(cmd1, sizeof(cmd1));
    CMM_EN = false;
  }


  if (LRF.available() > 0 && Bytes_counter < 12) {
    data = LRF.read();
    LRF_feedback += data;
    Bytes_counter++;
  }


  if (Bytes_counter == 12) {
    measurement = LRF_feedback.substring(6, 12);
    measurement += LRF_feedback.substring(0, 2);

    USB.print("Raw Feedback: ");
    USB.print(LRF_feedback);
    USB.print(" || Bytes Received: ");
    USB.print(Bytes_counter);
    USB.print(" || LRF Measurement: ");
    USB.print(measurement);
    USB.println(" DONE");

    LRF_feedback = "";
    Bytes_counter = 0;
  }

}
