import serial
import time

ser = serial.Serial('/dev/ttyACM0',9600)
time.sleep(5)

string = "Hello"
string_encode = string.encode()

while 1:
    ser.write(string_encode)
