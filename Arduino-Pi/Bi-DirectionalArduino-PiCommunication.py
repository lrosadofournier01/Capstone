import serial
import time

#Define the USB port being used and baud rate. Must match the Arduino
ser = serial.Serial('/dev/ttyUSB0',9600)

#Function for sending message to the Arduino
def RPiToArduino():
    #Write your message
    msg = "Hello from Raspberry Pi!\n"

    #Send message over the serial
    while 1:
        print ("Sending message to Arduino Serial...")
        ser.write(msg.encode())
        time.sleep(1)
        #Need to break out of the loop to let the next function operate
        break

#Function for getting message from the Arduino
def ArduinoToRPi():
    #Read from the Serial and print the message
    readSerial = ser.readline()
    print ("This just in...")
    print (readSerial)
    time.sleep(1)

#Call the functions in a loop
while 1:
    RPiToArduino()
    ArduinoToRPi()
