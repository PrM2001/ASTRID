import serial
import time

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
            if whichMotor in ['RA', 'r', 'ra', 'right ascension']:
                num_motor = 0
            elif  whichMotor in ['Dec', 'd', 'dec', 'declination']:
                num_motor = 1
            else:
                print('Not a valid axis')

        ser.write(num_motor.encode())

        num_steps = input("Enter angle in degrees to turn")
        
        # send number of microsteps to ESP32
        ser.write(num_steps.encode())
        
        # wait for ESP32 to finish turning the stepper motor
        while ser.inWaiting() < 1:
            pass
        print("ESP32: " + ser.readline().decode().strip())
        
        # wait 1 second for next user input during idle
        time.sleep(1)

if __name__ == "__main__":
    jog()