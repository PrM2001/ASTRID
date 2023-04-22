import serial
import time

microsteps = 1
reduction = 26.85 * 30


def jog():
    # configure the serial connections (the parameters differs on the device you are connecting to)
    ser = serial.Serial(
        port='COM3',
        baudrate=115200,
    )

    ser.isOpen()

    while True:
        # get user input for number of microsteps
        
        whichMotor = None
        while (whichMotor == None):
            whichMotor = input("RA or Dec? ")
            if whichMotor in ['RA', 'r', 'ra', 'right ascension', 'R']:
                num_motor = 0
            elif  whichMotor in ['Dec', 'd', 'dec', 'declination', 'D']:
                num_motor = 1
            else:
                print('Not a valid axis')

        ser.write(str(num_motor).encode())

        angle = float(input("Enter angle in degrees to turn"))

        num_steps = int(angle * (1.8/microsteps)/reduction)
        
        # send number of microsteps to ESP32
        ser.write(str(num_steps).encode())
        
        # wait for ESP32 to finish turning the stepper motor
        while ser.inWaiting() < 1:
            pass
        print("ESP32: " + ser.readline().decode().strip())
        
        # wait 1 second for next user input during idle
        time.sleep(1)

if __name__ == "__main__":
    jog()