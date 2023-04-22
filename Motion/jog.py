import serial
import time

microsteps = 1
reduction = 26.85 * 30


def jog():
    # configure the serial connections (the parameters differs on the device you are connecting to)
    ser = serial.Serial(
        port='COM5',
        baudrate=115200,
    )

    # ser.isOpen()

    while True:
        while ser.inWaiting() < 1:
            time.sleep(0.2)
        ser.flushInput()

        raAngle = input("Enter RA angle to move in degrees:\n")
        raSteps =  str(int(float(raAngle) /(1.8/(30 * 26.85))))

        ser.write(raSteps.encode())
        while ser.inWaiting() < 1:
            pass
        while ser.inWaiting() > 0:
            print("ESP32: " + ser.readline().decode())


        decAngle = input("Enter Dec angle to move in degrees:\n")
        decSteps = str(int(float(decAngle) /(1.8/(30 * 26.85))))

        ser.write(decSteps.encode())
        while ser.inWaiting() < 1:
            pass
        while ser.inWaiting() > 0:
            print("ESP32: " + ser.readline().decode())
        
        # # wait for ESP32 to finish turning the stepper motor
        # while ser.inWaiting() < 1:
        #     pass
        # print("ESP32: " + ser.readline())
        
        # wait 1 second for next user input during idle
        time.sleep(1)

if __name__ == "__main__":
    jog()