#Get the joystick click from the Arduino: RPI <--
#Get values from all of the sensors
#Do the calculations
#Send the calculations to the Arduino:    RPI -->
import math #calculations
import time #Arduino-Pi
import re #Exclude msgs


#Define readSerial globally. Decode added to make it a string
readJoy = ser.readline().decode()

#Get Joystick values from the Arduino
def getArduino():
	#Read from the Serial. This is the on value to turn on the laser
	return readJoy

#Dummy barometer values
def getBaro():
    temperature = 15
    pressure = 1011
    return temperature, pressure

#Dummy compass values
def getCompass():
    heading_angle = 300 #In Degrees
    return heading_angle

#Dummy laser values
def getLaser():
    meas = 15
    return meas

#Dummy GPS values
def getGPS():
    letter1 = 'P'
    letter2 = 'U'
    numberA = 12345
    numberB = 99999
    return letter1, letter2, numberA, numberB

#'Update' status of the sensors to see if there are new values
def updateStatus():
    getBaro()
    getCompass()
    getGPS()
    getLaser()

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

    #distance is in meters. Check math on this
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

#Function to send calculations to the Arduino
def RPiToArduino(msg):
#Send message over the serial
	while 1:
		ser.write(msg.encode())
		#Need to break out of the loop to let the next function operate
		break

#Wait for Arduino to send the click
time.sleep(2)
#Assign joystick click to a variable
joyClick = getArduino()
while 1:
    #If the joy stick is clicked, update the status, do the math, and send the calulations to the Arduino
    if joyClick = 0:
        updateStatus()
        dist, loc = doMath()
        calc = dist, loc
        RPiToArduino(calc)
    else:
        updateStatus()
