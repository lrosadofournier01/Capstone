import serial
import time

#Define the USB port being used and baud rate. Must match the Arduino
ser = serial.Serial('/dev/ttyUSB0',9600)
#Define readSerial globally
readSerial = ser.readline()
#Write your message
msg = "Hello from Raspberry Pi!\n"

#Function for sending message to the Arduino
def RPiToArduino():
    #Send message over the serial
    while 1:
        print ("Sending message to Arduino Serial...")
        ser.write(msg.encode())
        #Wait for Arduino to print the message
        time.sleep(3)
        #Need to break out of the loop to let the next function operate
        break

#Function for getting message from the Arduino
def ArduinoToRPi():
    #Wait for Arduino to send message
    time.sleep(3)
    #Read from the Serial and print the message
    print ("This just in...")
    Exclude()

#Function to exclude redundant serial messages
def Exclude():
    #Define Regex statements
    re_AtoR = r'Sending message to Raspberry Pi...'
    re_NM = r'New Message...'

    #Want to only print the message from the Arduino Nano
    check = re.match(re_AtoR + re_NM + msg, readSerial)
    if check:
        #Do nothing
    else:
        #Read from the serial
        print (readSerial)

#Call the functions in a loop
while 1:
    RPiToArduino()
    ArduinoToRPi()
    #Wait before executing the loop again
    time.sleep(5)
