import ast
import serial
import time
ser=serial.Serial("/dev/ttyAMA0",19200, timeout=3.0)
myChar=chr(79)
myChar=myChar.encode('utf-8')
ser.write(myChar)

rcv=ser.read(10)
print(rcv)

myChar=chr(68)
myChar=myChar.encode('utf-8')
ser.write(myChar)

rcv=ser.read(10)
time.sleep(3)

print(rcv)

rcv=rcv.decode('utf-8')
rcv=rcv.strip('D: ')
rcv=rcv.strip('m,')
print(rcv)
rcv=float(rcv)
print("number")
print(rcv)
