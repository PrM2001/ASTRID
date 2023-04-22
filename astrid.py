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
        self.raWindow = 40
        self.decWindow = 40

        self.gearReduction = 26.85 * 30
        self.microSteps = 1

    def isUnsafe(self, ra, dec):
        return (ra < self.raBounds[0] or ra > self.raBounds[1] or dec < self.decBounds[0] or dec > self.decBounds[1])

    def getCurrPos(self):
        #print(os.path.abspath(self.path))

        paths = sorted(self.path.glob('test/*.jpg')) #change this folder

        impath = paths[-1]
        print('Solving for image at: ' + str(impath))
        with Image.open(str(impath)) as img:
            solved = self.t3.solve_from_image(img)  # Adding e.g. fov_estimate=11.4, fov_max_error=.1 improves performance
        ra = solved.get('RA')
        dec = solved.get('Dec')

        for oldPath in paths:
            os.remove(oldPath)

        return(ra, dec)

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

        elif len(argv) == 3 and all(int_arg.strip('-').isnumeric() for int_arg in argv[1:]):
            ra = float(argv[1])
            dec = float(argv[2])
            if self.isUnsafe(ra, dec):
                raise SystemExit('\nThe object is out of safe RA and Dec bounds!\n')
            return float(argv[2]), float(argv[3])
        
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
    esp32 = serial.Serial(port='COM4', baudrate=115200, timeout=.1) # Change 'COM3' to the actual port number
    
    #Instantiate and ASTRID object
    astrid = ASTRID()

    #remove all previous images
    for impath in astrid.path.glob('test/*.jpg'):
        os.remove(impath)
    


    while True:
        #set all the coordinates to None for the check below
        current_ra, current_dec, desired_ra, desired_dec = None, None, None, None

        # Get the current and desired RA and Dec from the sensor
        while None in (current_ra, current_dec, desired_ra, desired_dec):
            current_ra, current_dec, desired_ra, desired_dec = astrid.observe(sys.argv)
            if (None in (current_ra, current_dec, desired_ra, desired_dec)):
                print('Could not find lock')
            time.sleep(0.5)
        
        # Calculate the number of microsteps for each axis

        ra_error = (desired_ra - current_ra)
        #make smol angle rather than big here

        dec_error = (desired_dec - current_dec)

        ra_steps = ra_error / ((1.8/astrid.microsteps)/astrid.reduction)
        dec_steps = dec_error / ((1.8/astrid.microsteps)/astrid.reduction)

        # Check if the error is less than 10 arcseconds
        if abs(ra_error) < astrid.raWindow and abs(dec_error) < astrid.decWindow:
            # Send an instruction of 0 steps
            esp32.write(f"{0} {0}\n".encode())
        else:                  
            # Send the number of microsteps to the controller.ino
            esp32.write(f"{int(ra_error)} {int(dec_error)}\n".encode())
        
        # Wait for the controller.ino to finish the previous command
        while esp32.in_waiting == 0:
            time.sleep(0.1)

        #need to read and remove everything in buffer
        esp32.reset_input_buffer()
        


if __name__ == "__main__":
    main()