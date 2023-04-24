import serial
import time
import os
import sys 
from PIL import Image
from pathlib import Path
from skyfield.api import load
from tetra3_master.tetra3 import Tetra3
sys.path.append('/tetra3_master')

class ASTRID:
    def __init__(self):
        self.t3 = Tetra3('13_FOV_Database')
        self.path = Path('./tetra3_master/test_data/')
        #print(os.path.abspath(self.path))
        self.planets = load('de421.bsp')
        self.ts = load.timescale()

        self.DSOs = {'mercury':self.planets['mercury barycenter'],
                     'venus': self.planets['venus barycenter'],
                     'earth': self.planets['earth barycenter'],
                     'mars': self.planets['mars barycenter'],
                     'jupiter': self.planets['jupiter barycenter'],
                     'saturn': self.planets['saturn barycenter'],
                     'uranus': self.planets['uranus barycenter'],
                     'neptune': self.planets['neptune barycenter'],
                     'pluto':self.planets['pluto barycenter'],
                     'moon': self.planets['moon']
                     }
        
        self.raBounds = [0,360]
        self.decBounds = [-90, 90]
        self.raWindow = 40/3600
        self.decWindow = 40/3600

        self.reduction = 26.85 * 30
        self.microsteps = 8

    def isUnsafe(self, ra, dec):
        return (ra < self.raBounds[0] or ra > self.raBounds[1] or dec < self.decBounds[0] or dec > self.decBounds[1])

    def getCurrPos(self):
        #print(os.path.abspath(self.path))

        paths = sorted(self.path.glob('test/*.jpg')) #change this folder

        if paths:
            impath = paths[-1]
            print('Solving for image at: ' + str(impath), flush=True)
            with Image.open(str(impath)) as img:
                solved = self.t3.solve_from_image(img)  # Adding e.g. fov_estimate=11.4, fov_max_error=.1 improves performance
            ra = solved.get('RA')
            dec = solved.get('Dec')

            for oldPath in paths:
                #os.remove(oldPath)
                pass

            return (ra, dec)
        else:
            return (None, None)

        # for impath in sorted(self.path.glob('test/*.jpg')): 
        #     print('Solving for image at: ' + str(impath))
        #     with Image.open(str(impath)) as img:
        #         solved = self.t3.solve_from_image(img)  # Adding e.g. fov_estimate=11.4, fov_max_error=.1 improves performance
        #     os.remove(impath)
        #     ra = solved.get('RA')
        #     dec = solved.get('Dec')
        # return(ra, dec)

    def getDesiredPos(self, argv):
        if argv[1] in self.DSOs:
            t = self.ts.now()
            astrometric = self.earth.at(t).observe(self.DSOs.get(argv[1])).apparent()
            ra, dec, distance = astrometric.radec()
            ra = ra._degrees
            dec = dec._degrees

            if self.isUnsafe(ra, dec):
                raise SystemExit('\nThe object is out of safe RA and Dec bounds!\n')

            return(ra, dec)

        elif len(argv) == 3:
            ra = float(argv[1])
            dec = float(argv[2])
            if self.isUnsafe(ra, dec):
                raise SystemExit('\nThe object is out of safe RA and Dec bounds!\n')
            return ra, dec
        
        else:
            raise SystemExit("\nInputs should be a planet or an integer for RA and Dec YOU FOOL\n")
    
    def observe(self, argv):
        # Get the current RA and Dec from the telescope
        current_ra, current_dec = self.getCurrPos()
        
        # Get the desired RA and Dec from the user
        desired_ra, desired_dec = self.getDesiredPos(argv) 
        
        return current_ra, current_dec, desired_ra, desired_dec





def main(): 
    # Open a serial connection to the controller.ino
    esp32 = serial.Serial(port='COM3', baudrate=115200, timeout=.1) # Change 'COM3' to the actual port number
    
    #Instantiate and ASTRID object
    astrid = ASTRID()

    #remove all previous images
    for impath in astrid.path.glob('test/*.jpg'):
        #os.remove(impath)
        pass

    while True:
        print("new main loop beginning", flush=True)
        #set all the coordinates to None for the check below
        current_ra, current_dec, desired_ra, desired_dec = None, None, None, None

        # Get the current and desired RA and Dec from the sensor
        while None in (current_ra, current_dec, desired_ra, desired_dec):
            current_ra, current_dec, desired_ra, desired_dec = astrid.observe(sys.argv)
            if (None in (current_ra, current_dec, desired_ra, desired_dec)):
                print('Could not find lock', flush=True)
            time.sleep(0.5)
        
        # Calculate the number of microsteps for each axis

        ra_error = (desired_ra - current_ra)
        #make smol angle rather than big here
        while ra_error > 180:
            ra_error = ra_error - 360
        while ra_error < -180:
            ra_error = ra_error + 360

        dec_error = (desired_dec - current_dec)



        if abs(ra_error) < astrid.raWindow and abs(dec_error) < astrid.decWindow:
            ra_steps, dec_steps = '0','0' 
        else:
            ra_steps = str(int(ra_error / ((1.8/astrid.microsteps)/astrid.reduction)))
            dec_steps = str(int(dec_error / ((1.8/astrid.microsteps)/astrid.reduction)))

        msg = ra_steps+','+dec_steps+',' 


        #A (have sent esp some code)
        esp32.write(msg.encode())

        while esp32.inWaiting() < 1: #wait for acknowledgement from esp32
            time.sleep(0.05)

        #when we get here, pythn will have received some acknowledgment

        while esp32.inWaiting() > 0: #while there are still acknowledgement messages, read them all out and print them
            print("From ESP32: " + esp32.readline().decode(), flush=True)
        
        
        while esp32.inWaiting() < 1 : #wait until esp32 asks for a new command
            print("Zero items in input buffer. waiting for esp32 to say smth")
            #pass
        esp32.flushInput()


        # # Check if the error is less than 10 arcseconds
        # if abs(ra_error) < astrid.raWindow and abs(dec_error) < astrid.decWindow:
        #     # Send an instruction of 0 steps
        #     esp32.write(f"{0} {0}\n".encode())
        # else:                  
        #     # Send the number of microsteps to the controller.ino
        #     esp32.write(f"{int(ra_error)} {int(dec_error)}\n".encode())
        
        # # Wait for the controller.ino to finish the previous command
        # while esp32.in_waiting == 0:
        #     time.sleep(0.1)

        # #need to read and remove everything in buffer
        # esp32.reset_input_buffer()
        


if __name__ == "__main__":
    main()