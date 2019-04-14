import smbus
import math
import csv
import datetime
import time
from ctypes import c_short
import ast
import serial

#gyro/accelerometer definitions
power_mgmt_1 = 0x6b
currentDT=datetime.datetime.now()
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
 
bus = smbus.SMBus(1) # bus = smbus.SMBus(0) fuer Revision 1
address = 0x68       # gyroscope/accelerometer via i2cdetect

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
#
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

  return (temperature/10.0,pressure/100.0)

#Laser definitions
#Laser via arduino, only need to process recieved string values
def laserOutput(rcv):
    rcv=rcv.decode('utf-8')     #decode from Unicode and parse to string
    rcv=rcv.strip('D: ')        #strip hardcoded leading string from distance
    rcv=rcv.strip('m,')         #strip hardcoded trailing string from distance 
    rcv=float(rcv)              #cast string into float
    return rcv

def getGyroscope():
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

def getBaro():
    if DEVICE == 0x77:
      temperature, pressure=readBmp180()
      return temperature, pressure

#def getLaser():
#to be used later-receive data from arduino and pass to laserOutput(rcv string)

#def doMath():
#to be used for combining LRF, GPS, compass, and baro for TGT LOC


#begin main loop
#order: gyro, baro, laser

while 1:
    getGyroscope()
    getBaro()
    getCompass()
    getGPS()
    getLaser()
    doMath()
    sendValues()