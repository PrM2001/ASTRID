import serial
import time
import os
import sys 
from tetra3_master.tetra3 import Tetra3
from PIL import Image
from pathlib import Path
from skyfield.api import load
sys.path.append('../tetra3_master')

class ASTRID:
    def __init__(self):
        self.t3 = Tetra3('13_FOV_Database')
        self.path = Path('../test_data/')
        self.planets = load('../Skyfield/de421.bsp')
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

    def isUnsafe(self, ra, dec):
        return (ra < self.raBounds[0] or ra > self.raBounds[1] or dec < self.decBounds[0] or dec > self.decBounds[1])

    def getCurrPos(self):
        for impath in self.path.glob('test/*.jpg'): 
            print('Solving for image at: ' + str(impath))
            with Image.open(str(impath)) as img:
                solved = self.t3.solve_from_image(img)  # Adding e.g. fov_estimate=11.4, fov_max_error=.1 improves performance
            os.remove(impath)
            return(solved.get('RA'), solved.get('Dec'))

    def getDesiredPos(self, argv):
        if argv[1] in self.DSOs:
            t = self.ts.now()
            astrometric = self.earth.at(t).observe(self.DSOs.get(argv[1])).apparent()
            ra, dec, distance = astrometric.radec()
            ra = ra._degrees
            dec = dec.degrees

            if self.isUnsafe(ra, dec):
                raise Exception('The object is out of safe RA and Dec bounds')

            return(ra, dec)

        elif len(argv) == 3 and all(int_arg.isnumeric() for int_arg in argv[1:]):
            if self.isUnsafe(ra, dec):
                raise Exception('The object is out of safe RA and Dec bounds')
            return float(argv[2]), float(argv[3])
        
        else:
            raise Exception("Inputs should be a planet or an integer for RA and Dec YOU FOOL")
    
    def observe(self, DSO):
        # Get the current RA and Dec from the telescope
        current_ra, current_dec = self.getCurrPos()
        
        # Get the desired RA and Dec from the user
        desired_ra, desired_dec = self.getDesiredPos(DSO) 
        
        return current_ra, current_dec, desired_ra, desired_dec






def main():
    # Open a serial connection to the controller.ino
    ser = serial.Serial('COM3', 115200) # Change 'COM3' to the actual port number
    # Create instance and load default_database (built with max_fov=12 and the rest as default)
    t3 = Tetra3('13_FOV_Database')

    astrid = ASTRID() 
    
    while True:
        # Get the current and desired RA and Dec from the sensor
        current_ra, current_dec, desired_ra, desired_dec = astrid.observe(sys.argv)
        
        # Calculate the number of microsteps for each axis
        ra_error = (desired_ra - current_ra) ((1.8/32)/810)
        dec_error = (desired_dec - current_dec)  ((1.8/32)/810)

        ra_steps = ra_error / ((1.8/32)/810)
        dec_steps = dec_error / ((1.8/32)/810)

        # Check if the error is less than 10 arcseconds
        if abs(ra_error) < 10 and abs(dec_error) < 10:
            # Send an instruction of 0 steps
            ser.write(b"0 0\n")
        else:                  
            # Send the number of microsteps to the controller.ino
            ser.write(f"{int(ra_error)} {int(dec_error)}\n".encode())
        
        # Wait for the controller.ino to finish the previous command
        while ser.in_waiting == 0:
            time.sleep(0.1)
        

        
        # Wait for the controller.ino to finish the command
        while ser.in_waiting == 0:
            time.sleep(0.1)


if __name__ == "__main__":
    main()