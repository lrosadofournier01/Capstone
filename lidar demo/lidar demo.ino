/*
Example code for Benewake TFMini time-of-flight distance sensor. 
by Peter Jansen (December 11/2017)
This example code is in the public domain.


This example communicates to the TFMini using a SoftwareSerial port at 115200, 
while communicating the distance results through the default Arduino hardware
Serial debug port. 


SoftwareSerial for some boards can be unreliable at high speeds (such as 115200). 
The driver includes some limited error detection and automatic retries, that
means it can generally work with SoftwareSerial on (for example) an UNO without
the end-user noticing many communications glitches, as long as a constant refresh
rate is not required. 


The (UNO) circuit:
 * Uno RX is digital pin 10 (connect to TX of TF Mini)
 * Uno TX is digital pin 11 (connect to RX of TF Mini)


THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
*/


#include <SoftwareSerial.h>
#include "TFMini.h"


// Setup software serial port 
SoftwareSerial mySerial(10, 11);      // Uno RX (TFMINI TX), Uno TX (TFMINI RX)
SoftwareSerial mySerial2(8, 9);
TFMini tfmini;
TFMini tfmini2;
uint16_t dist = 0;
uint16_t strength = 0;
uint16_t dist2=0;
uint16_t strength2=0;
boolean flag1=false;
boolean flag2=false;
int ranges1[9];
int ranges2[9];
int avg1=0;
int avg2=0;
int temp=0;
int count=0;
int i;
int population;


void setup() {
  // Step 1: Initialize hardware serial port (serial debug port)
  Serial.begin(115200);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);
     
  Serial.println ("Initializing...");


  // Step 2: Initialize the data rate for the SoftwareSerial port
  mySerial.begin(TFMINI_BAUDRATE);
  mySerial2.begin(TFMINI_BAUDRATE);
Serial.println("baud start");
  // Step 3: Initialize the TF Mini sensor
  tfmini.begin(&mySerial);    
  tfmini2.begin(&mySerial2);
  Serial.println("Serial init");


mySerial.listen();      //Turn on on range finder, only one can be on at a time
  for (i=0; i<10; i++){
    
      dist = tfmini.getDistance();
      strength = tfmini.getRecentSignalStrength();
  // Display the measurement
      Serial.print(dist);
      Serial.print(" cm      sigstr: ");
      Serial.println(strength);
  //buffer range measurements to get an average range
      ranges1[i]=dist;
      temp=temp+ranges1[i];    //sum array
      Serial.print(temp);
      Serial.print("  ");
      Serial.println(i);
  }
  avg1=temp/10;     //average the sum of the array
  Serial.print(avg1);
  Serial.println(" cm Average distance");
  delay(1000);
  


mySerial2.listen();
  
  for (i=0; i<10; i++){
      dist2 = tfmini2.getDistance();
      strength2 = tfmini2.getRecentSignalStrength();
      Serial.print(dist2);
      Serial.print(" cm 2    sigstr 2: ");
      Serial.println(strength2);
      ranges2[i]=dist2;
      temp=temp+ranges2[i];
      Serial.print(temp);
      Serial.print("  ");
      Serial.println(i);
  }
  avg2=temp/10;
  Serial.print(avg2);
  Serial.println(" cm Average distance 2");
  
  delay(1000);
  
}




void loop() {
Serial.println("main loop");   
mySerial.listen();      //Turn on on range finder, only one can be on at a time
      dist = tfmini.getDistance();
      Serial.println(dist);
      Serial.println("distance 1 reading");
      strength = tfmini.getRecentSignalStrength();
      ranges1[count]= dist;
for (i=0; i<10; i++){
  temp=temp+ranges1[i];
}
avg1=temp/10;
Serial.println(avg1);
Serial.println("average 1");
delay(500);
      
mySerial2.listen();
      dist2 = tfmini2.getDistance();
      Serial.println(dist2);
      Serial.println("distance 2 reading");
      strength2 = tfmini2.getRecentSignalStrength();
      ranges2[count]= dist;
for (i=0; i<10; i++){
  temp=temp+ranges2[i];
}
avg2=temp/10;
Serial.println(avg2);
Serial.println("average 2");
delay(500);  


  
  count++;
  if(count==10){
    count=0;
  }
  delay(1000); 
}