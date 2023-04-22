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

    # ser.isOpen()

    while True:
        # get user input for number of microsteps
        
        # whichMotor = None
        # while (whichMotor == None):
        #     whichMotor = input("RA or Dec? ")
        #     if whichMotor in ['RA', 'r', 'ra', 'right ascension']:
        #         num_motor = 0
        #     elif  whichMotor in ['Dec', 'd', 'dec', 'declination']:
        #         num_motor = 1
        #     else:
        #         print('Not a valid axis')

        # ser.write(num_motor.encode())

        # angle = input("Enter angle in degrees to turn")

        # num_steps = angle * ((1.8/microsteps)/reduction) 
        
        # # send number of microsteps to ESP32
        # ser.write(num_steps.encode())


        raAngle = input("Enter RA angle to move in degrees:\n")
        decAngle = input("Enter Dec angle to move in degrees:\n")

        raSteps = raAngle /(1.8/(30 * 26.85))
        decSteps = decAngle /(1.8/(30 * 26.85))

        ser.write(raSteps.encode())
        ser.write(decSteps.encode())
        
        # wait for ESP32 to finish turning the stepper motor
        while ser.inWaiting() < 1:
            pass
        print("ESP32: " + ser.readline().decode().strip())
        
        # wait 1 second for next user input during idle
        time.sleep(1)

if __name__ == "__main__":
    jog()