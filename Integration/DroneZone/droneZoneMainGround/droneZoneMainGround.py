#Gets the calculations from the Arduino RPI <--
#Display calculations
import serial
import time
import re #To exclude msgs

ser = serial.Serial('/dev/ttyUSB0',9600)
#Define readSerial globally. Decode added to make it a string
readCalcs = ser.readline().decode()

def ArduinoToRPi():
	#Wait for Drone pi to do calculations
	time.sleep(5)
	#Read from the Serial and print the message
	print (readCalcs)
 #May need to clean up the message need to test it

 while 1:
     ArduinoToRPi()
