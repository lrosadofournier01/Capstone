import mgrs
from RPLCD.i2c import CharLCD
import time
import gps

# Listen on port 2947 (gpsd) of localhost
session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)
#lcd definitions
lcd=CharLCD(i2c_expander='PCF8574', address=0x27)
sample='$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47'

def charposition(string, char):
    pos = [] #list to store positions for each 'char' in 'string'
    for n in range(len(string)):
        if string[n] == char:
            pos.append(n)
    print(pos)    
    return pos


while True:
    try:
        report = session.next()
        if report['class'] == 'TPV':
            latVal=report.lat;
            lonVal=report.lon;
            #print("Lat: ")
            #print(latVal)
            #print("Lon: ")
            #print(lonVal)
            m=mgrs.MGRS()
            d=m.toMGRS(latVal, lonVal)
            print(d)
            #print(d.decode("ascii"))
            lcd.write_string('Tgt loc: ')
            lcd.cursor_pos=(1,0)
            lcd.write_string(d.decode("ascii"))

            #e=m.toLatLon(d)
            #print(e)

            #y='321942.29N'
            #yd=m.dmstodd(y)
            #print(y)

            #d,m,s=m.ddtodms(yd)
            #print(d)
            #print(m)
            #print(s)
    except KeyError:
        pass
    except KeyboardInterrupt:
        quit()
    except StopIteration:
        session = None
        print("GPSD has terminated")
#mypos=[]
#mypos=charposition(sample, ',')      #find all instances of commas and index them
#sampleLat=float(sample[mypos[1]+1:mypos[2]])     #.replace(",","") #find substring in GGA sentence for lat and remove commas 
#sampleLong=float(sample[mypos[3]+1:mypos[4]])     #.replace(",","") #find substring in GGA for long and remove commas
#sampleLat=sampleLat/100
#sampleLong=sampleLong/100


#print(sampleLat)
#print(sampleLong)

#latitude=42.0
#longitude=-93.0

