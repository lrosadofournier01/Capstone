import smbus #Gyroscope, barometer
import math #Gyroscope, Math
import time #Barometer, LRF, Arduino-Pi
from ctypes import c_short #Barometer
import ast #LRF
import serial #LRF, Arduino-Pi

#Define serial port and baud rate
ser=serial.Serial("/dev/ttyAMA0",9600, timeout=3.0)
#Define the bus
bus = smbus.SMBus(1) # bus = smbus.SMBus(0) fuer Revision 1 for Gyro, Acc, and Baro

#-----------------------------------
#Check if sensors are working
#-----------------------------------


#-----------------------------------
#Update status of the sensors
#-----------------------------------
def updateStatus():
    getGyroAcc()
    getBaro()
    getCompass()
#-----------------------------------
#Arduino values (GPS and Joystick)
#-----------------------------------
#Define readSerial globally. Decode added to make it a string
readSerial = ser.readline().decode()
#Function for getting message from the Arduino
def getArduino():
	#Wait for Arduino to send message
	time.sleep(1)
	#Read from the Serial. This is the on value to turn on the laser
	return readSerial

#Might need to clean up the message, need to test and see

#-----------------------------------
#Gyroscope and accelerometer
#-----------------------------------
power_mgmt_1 = 0x6b

def read_byte(reg):
    return bus.read_byte_data(address, reg)

def read_word(reg):
    h = bus.read_byte_data(address, reg)
    l = bus.read_byte_data(address, reg+1)
    value = (h << 8) + l
    return value

def read_word_2c(reg):
    val = read_word(reg)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def dist(a,b):
    return math.sqrt((a*a)+(b*b))

def get_y_rotation(x,y,z):
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def get_x_rotation(x,y,z):
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

address = 0x68       # gyroscope/accelerometer via i2cdetect

def getGyroAcc():
    if address == 0x68:
        bus.write_byte_data(address, power_mgmt_1, 0)
        gyroscope_xout = read_word_2c(0x43)
        gyroscope_yout = read_word_2c(0x45)
        gyroscope_zout = read_word_2c(0x47)

        gyroscope_x_scaled=gyroscope_xout / 131
        gyroscope_y_scaled=gyroscope_yout / 131
        gyroscope_z_scaled=gyroscope_zout / 131

        accelerometer_xout = read_word_2c(0x3b)
        accelerometer_yout = read_word_2c(0x3d)
        accelerometer_zout = read_word_2c(0x3f)

        accelerometer_xout_scaled = accelerometer_xout / 16384.0
        accelerometer_yout_scaled = accelerometer_yout / 16384.0
        accelerometer_zout_scaled = accelerometer_zout / 16384.0

        get_x_rotation(accelerometer_xout_scaled, accelerometer_yout_scaled, accelerometer_zout_scaled)
        get_y_rotation(accelerometer_xout_scaled, accelerometer_yout_scaled, accelerometer_zout_scaled)

        #shrink/simplify output values
        gx=gyroscope_x_scaled
        gy=gyroscope_y_scaled
        gz=gyroscope_z_scaled
        ax=accelerometer_xout_scaled
        ay=accelerometer_yout_scaled
        az=accelerometer_zout_scaled
        return gx,gy,gz,ax,ay,az

#barometer definitions
#           bmp180.py
#  Read data from a digital pressure sensor.
#
# Author : Matt Hawkins
# Date   : 17/02/2017
# Edited : Jonathan Hoogerhyde
# Date   : 02/14/2019
#
# http://www.raspberrypi-spy.co.uk/

#-----------------------------------
# Barometer
#-----------------------------------
DEVICE = 0x77 # Default barometer I2C address

def convertToString(data):
  # Simple function to convert binary data into
  # a string
  return str((data[1] + (256 * data[0])) / 1.2)

def getShort(data, index):
  # return two bytes from data as a signed 16-bit value
  return c_short((data[index] << 8) + data[index + 1]).value

def getUshort(data, index):
  # return two bytes from data as an unsigned 16-bit value
  return (data[index] << 8) + data[index + 1]

def readBmp180Id(addr=DEVICE):
  # Chip ID Register Address
  REG_ID     = 0xD0
  (chip_id, chip_version) = bus.read_i2c_block_data(addr, REG_ID, 2)
  return (chip_id, chip_version)

def readBmp180(addr=DEVICE):
  # Register Addresses
  REG_CALIB  = 0xAA
  REG_MEAS   = 0xF4
  REG_MSB    = 0xF6
  REG_LSB    = 0xF7
  # Control Register Address
  CRV_TEMP   = 0x2E
  CRV_PRES   = 0x34
  # Oversample setting
  OVERSAMPLE = 3    # 0 - 3

  # Read calibration data
  # Read calibration data from EEPROM
  cal = bus.read_i2c_block_data(addr, REG_CALIB, 22)

  # Convert byte data to word values
  AC1 = getShort(cal, 0)
  AC2 = getShort(cal, 2)
  AC3 = getShort(cal, 4)
  AC4 = getUshort(cal, 6)
  AC5 = getUshort(cal, 8)
  AC6 = getUshort(cal, 10)
  B1  = getShort(cal, 12)
  B2  = getShort(cal, 14)
  MB  = getShort(cal, 16)
  MC  = getShort(cal, 18)
  MD  = getShort(cal, 20)

  # Read temperature
  bus.write_byte_data(addr, REG_MEAS, CRV_TEMP)
  time.sleep(0.005)
  (msb, lsb) = bus.read_i2c_block_data(addr, REG_MSB, 2)
  UT = (msb << 8) + lsb

  # Read pressure
  bus.write_byte_data(addr, REG_MEAS, CRV_PRES + (OVERSAMPLE << 6))
  time.sleep(0.04)
  (msb, lsb, xsb) = bus.read_i2c_block_data(addr, REG_MSB, 3)
  UP = ((msb << 16) + (lsb << 8) + xsb) >> (8 - OVERSAMPLE)

  # Refine temperature
  X1 = ((UT - AC6) * AC5) >> 15
  X2 = (MC << 11) / (X1 + MD)
  B5 = X1 + X2
  temperature = int(B5 + 8) >> 4

  # Refine pressure
  B6  = B5 - 4000
  B62 = int(B6 * B6) >> 12
  X1  = (B2 * B62) >> 11
  X2  = int(AC2 * B6) >> 11
  X3  = X1 + X2
  B3  = (((AC1 * 4 + X3) << OVERSAMPLE) + 2) >> 2

  X1 = int(AC3 * B6) >> 13
  X2 = (B1 * B62) >> 16
  X3 = ((X1 + X2) + 2) >> 2
  B4 = (AC4 * (X3 + 32768)) >> 15
  B7 = (UP - B3) * (50000 >> OVERSAMPLE)

  P = (B7 * 2) / B4

  X1 = (int(P) >> 8) * (int(P) >> 8)
  X1 = (X1 * 3038) >> 16
  X2 = int(-7357 * P) >> 16
  pressure = int(P + ((X1 + X2 + 3791) >> 4))
  #C and mbar respectivly
  return (temperature/10.0,pressure/100.0)

def getBaro():
    if DEVICE == 0x77:
      temperature, pressure=readBmp180()
      return temperature, pressure

#-----------------------------------
#Compass
#-----------------------------------


#-----------------------------------
#GPS
#-----------------------------------
#Get the data from the Arduino. Need to parse the data coming in from the Arduino and return the GPS data

#-----------------------------------
#Laser
#-----------------------------------
def getLaser():
    myChar=chr(79) #On instruction coming from the Arduino
    myChar=myChar.encode('utf-8')
    ser.write(myChar)
    rcv=ser.read(10) #Get on confirmation

    myChar=chr(68) #Measure instruction
    myChar=myChar.encode('utf-8')
    ser.write(myChar)
    rcv=ser.read(10) Get measure confirmation
    time.sleep(3)

    rcv=rcv.decode('utf-8') #Clean up the message
    rcv=rcv.strip('D: ')
    rcv=rcv.strip('m,')
    rcv=float(rcv)
    return rcv

#-----------------------------------
#Math
#-----------------------------------
def doMath():
#to be used for combining LRF, GPS, compass, and baro for TGT LOC
#Returns 'distance' and 'location'
    molMass = 0.02896968
    grav = 9.80665
    gasConst = 8.31582991
    seaLvlP = 1013.25
    tempLapse = 0.0065
    let1, let2, numA, numB = getGPS()
    laserDist = getLaser()
    headingAngle = getCompass()
    #Convert to radians for calculations
    headingAngle = math.radians(headingAngle)
    temp, press = getBaro()
    #Convert to kelvin for calculations
    tempKelv = temp + 273.15

    #Find the altitude
    constExp = (grav*molMass)/(gasConst*tempLapse)
    altitude = (1 - math.pow(press/seaLvlP, 1/constExp))*(tempKelv/tempLapse)

    #distance is in meters
    distance = math.sqrt(math.pow(altitude, 2) + math.pow(laserDist, 2))

    #Find the x and y components
    distX = distance * math.sin(headingAngle)
    distY = distance * math.cos(headingAngle)

    #Location is in MGRS
    xNumA = numA + distX #horizontal measurement
    yNumB = numB + distY #vertical measurement

    #See if the number for letter 1 moves to a different grid
    if xNumA > 99999:
        incLet1 = chr(ord(let1) + 1) #Goes to the next letter
        let1 = incLet1
        incRem1 = xNumA - 99999 #Find how much it goes into the next grid
        xNumA = incRem1 + 10000
    elif xNumA < 10000:
        decLet1 = chr(ord(let1) - 1) #Goes to the previous letter
        let1 = decLet1
        decRem1 = 10000 - xNumA #Find how much it goes into the previous grid
        xNumA = 99999 - decRem1
    else:
        pass #Keep going
    #See if the number for letter 2 moves to a different grid
    if yNumB > 99999:
        incLet2 = chr(ord(let2) + 1) #Goes to the next letter
        let2 = incLet2
        incRem2 = yNumB - 99999 #Find how much it goes into the next grid
        yNumB = incRem2 + 10000
    elif yNumB < 10000:
        decLet2 = chr(ord(let2) - 1) #Goes to the previous letter
        let2 = decLet2
        decRem2 = 10000 - yNumB #Find how much it goes into the previous grid
        yNumB = 99999 - decRem2
    else:
        pass #Keep going

    location = let1, let2, int(xNumA), int(yNumB)
    return distance, location

#-----------------------------------
#Get joystick instruction
#-----------------------------------
#Get the data from the Arduino. Need to parse the data coming in from the Arduino and return the
#joystick instruction to turn on the laser
def userInput():
    getArduino()
    #Strip the Arduino message to only have the joystick input
    #If click
    getGPS()
    getLaser()
    doMath()
    #else
    updateStatus()

#-----------------------------------
#Send calculations to Arduino
#-----------------------------------
def sendValues():
    dist, loc = doMath()
    valuesArray[] = ""
#Send message over the serial
	while 1:
        valuesArray[0] = dist
        valuesArray[1] = loc
		ser.write(valuesArray[].encode())
		#Wait for Arduino to print the message
		time.sleep(1)
		#Need to break out of the loop to let the next function operate
		break

#-----------------------------------
#Main loop
#-----------------------------------
while 1:
    errorChecking()
    updateStatus()
    userInput()
    sendValues()
