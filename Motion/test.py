# import serial

# ser = serial.Serial('COM3', 115200) # Set serial port and baud rate

# while True:
#     user_input = input('Enter a value: ')
#     ser.write(user_input.encode()) # Send user input to Arduino
    
#     if ser.in_waiting > 0:
#         data = ser.readline().decode('ascii') # Read incoming data
#         print(data.strip()) # Print received data from Arduino

import serial
import time
ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM3'
ser.open()

values = bytearray([4, 9, 62, 144, 56, 30, 147, 3, 210, 89, 111, 78, 184, 151, 17, 129])

while True:

    ser.write(values)
    try:
        print(f'Characters in ser buffer = {ser.in_waiting}')
        for i in range(ser.in_waiting):
            print (ord(ser.read()))
    except:
        print("pass")
    time.sleep(5)
 
print("done")
 
ser.close()