import serial
import time

#Define the USB port being used
ser = serial.Serial('/dev/ttyACM0',9600)
time.sleep(5)

#Write your message
msg = "Hello"
#msg_encode = msg.encode()

#Send message over the serial
while 1:
    ser.write(msg)
