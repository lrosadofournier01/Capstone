import serial
import time

#Define the USB port being used
ser = serial.Serial('/dev/ttyUSB0',9600)
time.sleep(10)

#Write your message
msg = "Hello from Raspberry Pi!\n"
#msg_encode = msg.encode()

#Send message over the serial
while 1:
    print("Sending message to Arduino Serial...")
    ser.write(msg.encode())
    print ("Message Sent!")
