#This is a script to get a message sent to Arduino Nano B from
#Arduino Nano A and then send a reply to Arduino Nano B

import serial
import time
import re

#Define the USB port being used and baud rate. Must match the Arduino
ser = serial.Serial('/dev/ttyUSB0',9600)
#Define readSerial globally. Decode added to make it a string
readSerial = ser.readline().decode()
#Write your message
reply = "Hello from Raspberry Pi B! I'm doing well.\n"

#Function for sending message to the Arduino
def RPiToArduino():
#Send message over the serial
	while 1:
		print ("Sending reply to Arduino B Serial...")
		ser.write(reply.encode())
		#Wait for Arduino to print the message
		time.sleep(1)
		#Need to break out of the loop to let the next function operate
		break

#Function for getting message from the Arduino
def ArduinoToRPi():
	#Wait for Arduino to send message
	time.sleep(1)
	#Read from the Serial and print the message
	Exclude()

#Replace multiple phrases in any given string
def replaceMultiple(mainString, toBeReplaces, newString):
    # Iterate over the strings to be replaced
    for elem in toBeReplaces :
        # Check if string is in the main string
        if elem in mainString :
            # Replace the string
            mainString = mainString.replace(elem, newString)

    return  mainString

#Function to exclude redundant serial messages
def Exclude():
	#Want to only print the message from the Arduino Nano
	replaceSerial = replaceMultiple(readSerial, [reply], "This Just in: ")
	print (replaceSerial)

#Call the functions in a loop
while 1:
        #Get message delivered by Arduino Nano B
        ArduinoToRPi()
        #Send reply to be delivered by Arduino Nano B
        RPiToArduino()
        #Wait before executing the loop again
        time.sleep(5)
