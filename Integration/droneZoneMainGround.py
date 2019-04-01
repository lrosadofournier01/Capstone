#Gets the calculations from the Arduino and displays them
import serial
import time
import re

ser = serial.Serial('/dev/ttyUSB0',9600)
#Define readSerial globally. Decode added to make it a string
readSerial = ser.readline().decode()

def ArduinoToRPi():
	#Wait for Drone pi to do calculations
	time.sleep(1)
	#Read from the Serial and print the message
	print (readSerial)
 #May need to clean up the message need to test it

 while 1:
     ArduinoToRPi()
