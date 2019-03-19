import serial
import time
import re

ser = serial.Serial('/dev/ttyUSB0',9600)
time.sleep(5)
readSerial = ser.readline().decode()

#Function to exclude redundant serial messages
def Exclude():
	#Define Regex statements
	re_AtoR = 'Sending message to Raspberry Pi...'

	#Want to only print the message from the Arduino Nano
	replaceSerial = readSerial.replace(re_AtoR, 'This Just in: ')
	print (replaceSerial)

while 1:
    #print (readSerial)
    Exclude()
