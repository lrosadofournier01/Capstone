#!/usr/bin/python
import smbus
import math
import csv
import datetime

# Register
power_mgmt_1 = 0x6b
power_mgmt_2 = 0x6c
currentDT=datetime.datetime.now() 
filename='data.csv'
newline='\r\n'
timestamp=""

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
address = 0x68       # via i2cdetect
 
# Activate to be able to address the module
with open(filename, 'a') as sensor_data:
    sensor_logger=csv.writer(sensor_data, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)    
    sensor_logger.writerow(['Timestamp', 'Gyro X Data', 'Gyro Y Data', 'Gyro Z Data', 'Accel X Data', 'Accel Y Data', 'Accel Z Data'])
while address == 0x68:
    bus.write_byte_data(address, power_mgmt_1, 0)
    with open(filename, 'a') as sensor_data:
        sensor_logger=csv.writer(sensor_data, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)    
            
        print ("Gyroscope")
        print ("--------")
 
        gyroscope_xout = read_word_2c(0x43)
        gyroscope_yout = read_word_2c(0x45)
        gyroscope_zout = read_word_2c(0x47)
    
        print ("gyroscope_xout: ", ("%5d" % gyroscope_xout), " scaled: ", (gyroscope_xout / 131))
        print ("gyroscope_yout: ", ("%5d" % gyroscope_yout), " scaled: ", (gyroscope_yout / 131))
        print ("gyroscope_zout: ", ("%5d" % gyroscope_zout), " scaled: ", (gyroscope_zout / 131))


        print ("\n")
        print ("Accelerometer")
        print ("---------------------")
        accelerometer_xout = read_word_2c(0x3b)
        accelerometer_yout = read_word_2c(0x3d)
        accelerometer_zout = read_word_2c(0x3f)
    
        accelerometer_xout_scaled = accelerometer_xout / 16384.0
        accelerometer_yout_scaled = accelerometer_yout / 16384.0
        accelerometer_zout_scaled = accelerometer_zout / 16384.0
    
        print ("accelerometer_xout: ", ("%6d" % accelerometer_xout), " scaled: ", accelerometer_xout_scaled)
        print ("accelerometer_yout: ", ("%6d" % accelerometer_yout), " scaled: ", accelerometer_yout_scaled)
        print ("accelerometer_zout: ", ("%6d" % accelerometer_zout), " scaled: ", accelerometer_zout_scaled)
        
        print ("X Rotation: " , get_x_rotation(accelerometer_xout_scaled, accelerometer_yout_scaled, accelerometer_zout_scaled))
        print ("Y Rotation: " , get_y_rotation(accelerometer_xout_scaled, accelerometer_yout_scaled, accelerometer_zout_scaled))
        timestamp=currentDT.strftime("%H:%M:%S")
        gx=str(gyroscope_xout)
        gy=str(gyroscope_yout)
        gz=str(gyroscope_zout)
        ax=str(accelerometer_xout_scaled)
        ay=str(accelerometer_yout_scaled)
        az=str(accelerometer_zout_scaled)
        sensor_logger.writerow([timestamp,gx,gy,gz,ax,ay,az])
        
