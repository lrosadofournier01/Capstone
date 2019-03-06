import serial
import time

ser = serial.Serial('/dev/ttyACM0',9600)
time.sleep(5)

while 1:
    readSerial = ser.readline()
    print readSerial
