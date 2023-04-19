import serial

ser = serial.Serial('COM3', 115200) # Set serial port and baud rate

while True:
    user_input = input('Enter a value: ')
    ser.write(user_input.encode()) # Send user input to Arduino
    
    if ser.in_waiting > 0:
        data = ser.readline().decode('ascii') # Read incoming data
        print(data.strip()) # Print received data from Arduino