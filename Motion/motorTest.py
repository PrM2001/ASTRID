import serial
import time

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='COM3',
    baudrate=115200,
)

ser.isOpen()

while True:
    # get user input for number of microsteps
    num_steps = input("Enter number of microsteps: ")
    
    # send number of microsteps to ESP32
    ser.write(num_steps.encode())
    
    # wait for ESP32 to finish turning the stepper motor
    while ser.inWaiting() < 1:
        pass
    print("ESP32: " + ser.readline().decode().strip())
    
    # wait 1 second for next user input during idle
    time.sleep(0.5)