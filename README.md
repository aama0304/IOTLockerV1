# IOTLockerV1
IOT Locker v1 Arduino Program

Arduino Libraries:
- HX711
- OneWire
- RFID
- SerialCommand

# Installation:

Arduino Libraries
- Extract/Copy to: C:\Users\*user*\Documents\Arduino\libraries

# Arduino Program:

- IOTLockerV1 (04072023) Program
- I2C Address Checker Program [for I2C LCD]

# Installation:

1. Upload I2C Address Checker Program on Arduino first to check all the LCD i2c address one by one.
2. Take note of the addresses of each LCD modules
3. change the address of each LCD modules from line 23-26 (ex. 0x27 > 0x0A) on the main program
4. Upload main program to Arduino board

# Desktop Program (C#):

- IOTLockSystem (04152023-2) Program

# Installation:

Desktop Program (C# Program)
- Download Here: [DOWNLOAD C# PROGRAM HERE](https://drive.google.com/file/d/1G8d_XxXniyg60A8_XvNgZpk6eqcGAF5X/view?usp=sharing)
- Extract and execute na .sln file

# Integration:

1. connect Arduino USB to PC.
2. on your desktop application/serial monitor software select "115200" as your baud rate.
3. connect the software to your Arduino board

*MAKE SURE TO HAVE A SECURE CONNECTION TO AVOID COMMUNICATION PROBLEMS
