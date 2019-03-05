#include <Servo.h>
#include <SoftwareSerial.h>
Servo servo1;
Servo servo2;
int x_key = A1;                                               
int y_key = A0;                                               
int x_pos;
int y_pos;
int servo1_pin = 8;
int servo2_pin = 9;  
int initial_position = 90;
int initial_position1 = 90;

void setup ( ) {
Serial.begin (9600) ;
servo1.attach (servo1_pin ) ; 
servo2.attach (servo2_pin ) ; 
servo1.write (initial_position);
servo2.write (initial_position1);
pinMode (x_key, INPUT) ;                     
pinMode (y_key, INPUT) ;                      
}

void loop ( ) {
x_pos = analogRead (x_key) ;  
Serial.print(x_pos);
y_pos = analogRead (y_key) ;          
Serial.print(y_pos);            

// Make servo go to 0 degrees 
   servo1.write(0); 
   servo2.write(0); 
   delay(1000); 
   // Make servo go to 90 degrees 
   servo1.write(90); 
   servo2.write(90); 
   delay(1000); 
   // Make servo go to 180 degrees 
   servo1.write(180); 
   servo2.write(180); 
   delay(1000); 
}   
/*
if (x_pos < 300){
if (initial_position < 10) { } 
  else{ 
    initial_position = initial_position - 20; 
    servo1.write ( initial_position );
    delay (100) ;
    } 
   } 
if (x_pos > 700){
if (initial_position > 180)
{  
}  
else{
initial_position = initial_position + 20;
servo1.write ( initial_position ) ;
delay (100) ;
}
}

if (y_pos < 300){
if (initial_position1 < 10) { } else{ initial_position1 = initial_position1 - 20; servo2.write ( initial_position1 ) ; delay (100) ; } } if (y_pos > 700){
if (initial_position1 > 180)
{  
}        
else{
initial_position1 = initial_position1 + 20;
servo2.write ( initial_position1 ) ;
delay (100) ;
}
}
}

*/
