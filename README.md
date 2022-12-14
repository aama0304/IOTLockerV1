# IOTLockerV1
IOT Locker v1 Arduino Program

Arduino Libraries:
\n HX711
OneWire
RFID
SerialCommand

# Installation:

Extract/Copy to: C:\Users\*user*\Documents\Arduino\libraries

# Arduino Program:

IOTLockerV1 (12142022) Program
I2C Address Checker Program [for I2C LCD]

# Installation:

1. Upload I2C Address Checker Program on Arduino first to check all the LCD i2c address one by one.
2. Take note of the addresses of each LCD modules
3. change the address of each LCD modules from line 23-26 (ex. 0x27 > 0x0A) on the main program
4. Upload main program to Arduino board

# Integration:

1. connect Arduino USB to PC.
2. on your desktop application/serial monitor software select "115200" as your baud rate.
3. connect the software to your Arduino board

*MAKE SURE TO HAVE A SECURE CONNECTION TO AVOID COMMUNICATION PROBLEMS
