# IOTLockerV1 Arduino Program
IOT Locker Arduino Program Explanation

## **Variables**

**Directives**
> - **OneWire/Wire** is a Master/Slave protocol, and all communication cabling required is a single wire. 
> - **LCD/Liquidcrystal_I2C** is for character LCDs based on the HD44780 controller connected via I2C bus using the cheap I2C backpack modules based on the PCF8574(T/A)
> - **HX711_ADC** is for the HX711 24-bit ADC for weight scales
> - **SPI** allows you to communicate with SPI devices, with the Arduino as the controller device.
> - **MFRC522** is a highly integrated reader or writer IC for contactless communication at 13.56 MHz.
> - **EEPROM** is a memory library whose values are kept when the board is powered off.
```Java
  #include <OneWire.h>
  #include <Wire.h>
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>
  #include <HX711_ADC.h>
  #include <SPI.h>
  #include <MFRC522.h>
  #include <EEPROM.h>
```
**LCD VARIABLES**
> - **lcd[X]** I2C LCD Declaration variable. use the I2C scanner to change the I2C address on each LCD connected in the system.
```Java
  LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD Module for Locker 1 [CHANGE I2C ADDRESS]
  ....
```
**RFID VARIABLES**
> - **RST_PIN / SS_PIN[X]** IO Declaration on where the RFID Module's Data/Reset Pin to be connected.
```Java
  #define RST_PIN 5
  #define SS_PIN1 6
  ....
```
> - **IDcontent[X]** String variable to store the RFID Content.
```Java
  String IDcontent1 = "";
  ....
```
> - **RfidMod[X]** RFID module variable declaring the Data and RST pin of the module to be read by the arduino properly.
```Java
  MFRC522 RfidMod1(SS_PIN1, RST_PIN);
  ....  
```
**LOAD CELL VARIABLES**
> - **HX711_dout[X] / HX711_sck[X]** HX711 Load Cell Weight Sensor Data pin Declaration.
```Java
  #define HX711_dout1 42
  #define HX711_sck1 43
  ....
```
> - **weight[X]** Float Variable to hold the weight reading of the load cell.
```Java
  float weight1
  ....
```
**TIMING/DELAY VARIABLES**
> - **disinfecttime** = the total time needed for the disinfection process (Default Value: **1680** [28 Minutes]).
> - **previousOnBoardLedMillis** = previous time count in milliseconds.
> - **blinkDuration** = duration of the delay (Default Value: **1000** [1 Second]).
> - **currentTime** = current count of the the timer in milliseconds.
> - **StartTime[X]** = used for starting the timer delay function (**millis()**).
> - **TimeLapsed[X]** = variable to hold the current value of the time that has lapsed
> - **MinRemain[X]** = variable holding the remaining time from the total "disinfecttime" set.
```Java
  int disinfecttime = 1680; //28 minutes
  unsigned long previousOnBoardLedMillis = 0;
  const int blinkDuration = 1000;
  unsigned long currentTime; 

  unsigned long StartTime1 = millis();
  ....

  unsigned int TimeLapsed1;
  ....

  int MinRemain1;
  ....
  
  int s = 60;
  ....
```
**GENERAL VARIABLES**
> - **UVLight[X] / PnlLight[X] / SolLock[X]** = relay variables.
```Java
  #define UVLight1 30;
  ....
  #define PnlLight1 34;
  ....
  #define SolLock1 38;
  ....
  
```
> - **DisinfectLock[X]** = Boolean to confirm that the corresponding locker number[X] is for disinfection.
> - **LockerIsLocked[X]** = Boolean to confirm that the corresponding locker number[X] is currently LOCKED.
> - **ParcelStr[X]** = String variable to store parcel QR Code content to be displayed on the LCD.
```Java
  bool DisinfectLock1 = false;
  ....
  bool LockerIsLocked1;
  ....
  String ParcelStr1;
  ....
```
**SERIAL VARIABLE**
> - **sCmd** = SerialCommand Variable for executing serial commands from the C# program.
```Java
  SerialCommand sCmd;
```

## **VOID SETUP()**
> - **Serial.begin()** begin serial monitor (Default Baud Rate: **115200**).
> - **SPI.begin()** Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high. for the RFID Modules
```Java
  Serial.begin(115200);
  SPI.begin();
```
> - **RfidMod[X].PCD_Init()** Initialize RFID Module No.[X].
> - **RfidMod1.PCD_DumpVersionToSerial()** Receive RFID Module version for confirmation if the Module is working properly.
```Java
  RfidMod1.PCD_Init();
  ....
  RfidMod1.PCD_DumpVersionToSerial();
  ....
```
> - **LoadCell[X].begin()** Initialize Load Cell Module No.[X].
```Java
  LoadCell1.begin();
  ....
```
> - **lcd[X]begin(20, 4)** Initialize LCD Module (Default Format: **20x4**).
> - **lcd[X].backlight()** Enable LCD Backlight for LCD no. [X].
```Java
  lcd.begin(20, 4);
  ....
  lcd.backlight();
  ....
```
> - Display program version on LCD to confirm that the LCD are initialized properly.
```Java
  lcd.setCursor(0, 0);
  lcd.print("IOT LOCKER SYSTEM v1");
  lcd.setCursor(0, 1);
  lcd.print("[ PROGRAM STARTING ]");
  lcd.setCursor(0, 2);
  lcd.print("[   PLEASE  WAIT   ]");
  lcd.setCursor(0, 3);
  lcd.print("[   FOR A MOMENT   ]");
```
> - Configure the relay pins to act as an OUTPUT device.
```Java
  pinMode(UVLight1, OUTPUT);
  pinMode(UVLight2, OUTPUT);
  pinMode(UVLight3, OUTPUT);
  pinMode(UVLight4, OUTPUT);

  pinMode(PnlLight1, OUTPUT);
  pinMode(PnlLight2, OUTPUT);
  pinMode(PnlLight3, OUTPUT);
  pinMode(PnlLight4, OUTPUT);

  pinMode(SolLock1, OUTPUT);
  pinMode(SolLock2, OUTPUT);
  pinMode(SolLock3, OUTPUT);
  pinMode(SolLock4, OUTPUT);
```
> - Initialize and calibrate all of the Load Cell module to display the correct value.
> - **calibrationValue** value is constant, must always have the value of **-465.30** for data accuracy.
```Java
  float calibrationValue;
  calibrationValue = -465.30;

  unsigned long stabilizingtime = 2000;
  //boolean _tare = true;
  LoadCell1.start(stabilizingtime, true);
  LoadCell2.start(stabilizingtime, true);
  LoadCell3.start(stabilizingtime, true);
  LoadCell4.start(stabilizingtime, true);

  if (LoadCell1.getTareTimeoutFlag() || LoadCell2.getTareTimeoutFlag() || LoadCell3.getTareTimeoutFlag() || LoadCell4.getTareTimeoutFlag())
  {
    lcd.setCursor(0, 0);
    lcd.print("FAIL: check MCU>HX711");
    lcd.setCursor(0, 1);
    lcd.println("wiring and pin designations");
    lcd.println(" ");
    while (1);
  }
  else
  {
    LoadCell1.setCalFactor(calibrationValue);
    LoadCell2.setCalFactor(calibrationValue);
    LoadCell3.setCalFactor(calibrationValue);
    LoadCell4.setCalFactor(calibrationValue);
  }
```
> - Initialize the Relays by turning every relay to HIGH (relay module configuration is ACTIVE LOW).
```Java
  digitalWrite(UVLight1, HIGH);
  digitalWrite(UVLight2, HIGH);
  digitalWrite(UVLight3, HIGH);
  digitalWrite(UVLight4, HIGH);

  digitalWrite(PnlLight1, HIGH);
  digitalWrite(PnlLight2, HIGH);
  digitalWrite(PnlLight3, HIGH);
  digitalWrite(PnlLight4, HIGH);

  digitalWrite(SolLock1, HIGH);
  digitalWrite(SolLock2, HIGH);
  digitalWrite(SolLock3, HIGH);
  digitalWrite(SolLock4, HIGH);
```
> - **LockerIsLocked[X]** setting the booleans to true confirming the state of the locker (Locked State).
> - **delay()** take a break for 1 Second.
> - **lcd[X].clear()** clear all the LCD contents to avoid unwanted data display.
> - **initSerialCommands()** Function initializing the SerialCommand Library to start listening commands from the serial monitor.
```Java
  LockerIsLocked1 = true;
  LockerIsLocked2 = true;
  LockerIsLocked3 = true;
  LockerIsLocked4 = true;

  delay(1000);
 
  lcd.clear();
  lcd2.clear();
  lcd3.clear();
  lcd4.clear();

  initSerialCommands();
```
## **VOID LOOP()**
> - **currentTime = millis()** Is used to monitor individual timers and task from each locker
> - **watcher()** Function to monitor incoming serial commands to change the LCD display from each locker
> - **ReadRFID[X]** Function to monitor the incoming RFID tag readings.
> - **ReadWeight[X]** Function to monitor the weight readings from the load cell.
```Java
  currentTime = millis(); //TIMER

  watcher();

  // RFID FUNCTIONS
  ReadRFID1();
  ReadRFID2();
  ReadRFID3();
  ReadRFID4();

  // WEIGHT FUNCTIONS
  ReadWeight1();
  ReadWeight2();
  ReadWeight3();
  ReadWeight4();
```

## **FUNCTIONS**
- initSerialCommands()
> list down and declare the following serial commands you want the system to receive and execute a specific task.
```Java
  sCmd.addCommand("SetParcel1", SetParcel1);
  sCmd.addCommand("SetParcel2", SetParcel2);
  sCmd.addCommand("SetParcel3", SetParcel3);
  sCmd.addCommand("SetParcel4", SetParcel4);

  sCmd.addCommand("LockLocker1", LockLocker1);
  sCmd.addCommand("LockLocker2", LockLocker2);
  sCmd.addCommand("LockLocker3", LockLocker3);
  sCmd.addCommand("LockLocker4", LockLocker4);

  sCmd.addCommand("UnlockLocker1", UnlockLocker1);
  sCmd.addCommand("UnlockLocker2", UnlockLocker1);
  sCmd.addCommand("UnlockLocker3", UnlockLocker1);
  sCmd.addCommand("UnlockLocker4", UnlockLocker1);

  sCmd.addCommand("DisinfectLocker1", DisinfectLocker1);
  sCmd.addCommand("DisinfectLocker2", DisinfectLocker2);
  sCmd.addCommand("DisinfectLocker3", DisinfectLocker3);
  sCmd.addCommand("DisinfectLocker4", DisinfectLocker4);
```
- watcher()
> the function responsible for the displays of the system. this handles all the changes on the data being display on all of the locker's lcd screens
```Java
  lcd.setCursor(0, 0);
  lcd.print("PARCEL LOCKER NO.:01");
  lcd.setCursor(0, 3);
  lcd.print("PARCEL WEIGHT KG:");
  lcd.print(String(weight1, 1));

  lcd2.setCursor(0, 0);
  lcd2.print("PARCEL LOCKER NO.:02");
  lcd2.setCursor(0, 3);
  lcd2.print("PARCEL WEIGHT KG:");
  lcd2.print(weight2, 1);

  lcd3.setCursor(0, 0);
  lcd3.print("PARCEL LOCKER NO.:03");
  lcd3.setCursor(0, 3);
  lcd3.print("PARCEL WEIGHT KG:");
  lcd3.print(weight3, 1);

  lcd4.setCursor(0, 0);
  lcd4.print("PARCEL LOCKER NO.:04");
  lcd4.setCursor(0, 3);
  lcd4.print("PARCEL WEIGHT KG:");
  lcd4.print(weight4, 1);

  if ( DisinfectLock1 == true)
  {
    StartDisinfection1();
  }
  else
  {
    if (weight1 > 0.1)
    {

      lcd.setCursor(0, 1);
      lcd.print("[  PARCEL INSIDE   ]");
      lcd.setCursor(0, 2);
      lcd.print("[");
      lcd.setCursor(2, 2);
      lcd.print(ParcelStr1);
      lcd.setCursor(19, 2);
      lcd.print("]");
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print("[ NO PARCEL INSIDE ]");
      lcd.setCursor(0, 2);
      lcd.print("[                  ]");
    }
  }

  if ( DisinfectLock2 == true)
  {
    StartDisinfection2();
  }
  else
  {
    if (weight2 > 0.1)
    {

      lcd2.setCursor(0, 1);
      lcd2.print("[  PARCEL INSIDE   ]");
      lcd2.setCursor(0, 2);
      lcd2.print("[");
      lcd2.setCursor(2, 2);
      lcd2.print(ParcelStr2);
      lcd2.setCursor(19, 2);
      lcd2.print("]");
    }
    else
    {
      lcd2.setCursor(0, 1);
      lcd2.print("[ NO PARCEL INSIDE ]");
      lcd2.setCursor(0, 2);
      lcd2.print("[                  ]");
    }
  }

  if ( DisinfectLock3 == true)
  {
    StartDisinfection3();
  }
  else
  {
    if (weight3 > 0.1)
    {

      lcd3.setCursor(0, 1);
      lcd3.print("[  PARCEL INSIDE   ]");
      lcd3.setCursor(0, 2);
      lcd3.print("[");
      lcd3.setCursor(2, 2);
      lcd3.print(ParcelStr3);
      lcd3.setCursor(19, 2);
      lcd3.print("]");
    }
    else
    {
      lcd3.setCursor(0, 1);
      lcd3.print("[ NO PARCEL INSIDE ]");
      lcd3.setCursor(0, 2);
      lcd3.print("[                  ]");
    }
  }

  if ( DisinfectLock4 == true)
  {
    StartDisinfection4();
  }
  else
  {
    if (weight4 > 0.1)
    {

      lcd4.setCursor(0, 1);
      lcd4.print("[  PARCEL INSIDE   ]");
      lcd4.setCursor(0, 2);
      lcd4.print("[");
      lcd4.setCursor(2, 2);
      lcd4.print(ParcelStr4);
      lcd4.setCursor(19, 2);
      lcd4.print("]");
    }
    else
    {
      lcd4.setCursor(0, 1);
      lcd4.print("[ NO PARCEL INSIDE ]");
      lcd4.setCursor(0, 2);
      lcd4.print("[                  ]");
    }
  }
```
- ReadWeight(X)
> the function responsible for returning the weight value from load cell no. [X].
```java
static boolean newDataReady = 0;

  if (LoadCell1.update())
  {
    newDataReady = true;
  }

  if (newDataReady)
  {
    weight1 = LoadCell1.getData();
    weight1 = weight1 / 1000.00;
    newDataReady = 0;
    weight1 = abs(weight1);
  }
```
- ReadRFID(X)
> the function responsible for returning the RFID Content from RFID Reader Module.
```java
 if ( ! RfidMod1.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! RfidMod1.PICC_ReadCardSerial())
  {
    return;
  }

  byte letter;
  for (byte i = 0; i < RfidMod1.uid.size; i++)
  {
    //lcd.setCursor(0, 0);
    //Serial.print(RfidMod1.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(RfidMod1.uid.uidByte[i], HEX);
    IDcontent1.concat(String(RfidMod1.uid.uidByte[i] < 0x10 ? " 0" : " "));
    IDcontent1.concat(String(RfidMod1.uid.uidByte[i], HEX));
  }

  IDcontent1.toUpperCase();
  if (IDcontent1.substring(1) == RFID1) // Make sure you change this with your own UID number
  {
    if (DisinfectLock1 == false)
    {
      if (LockerIsLocked1 == true)
      {
        lcd.setCursor(0, 1);
        lcd.print("[ ID READ SUCCESS! ]");
        lcd.setCursor(0, 2);
        lcd.print("[ LOCKER UNLOCKED. ]");
        digitalWrite(SolLock1, LOW);
        digitalWrite(PnlLight1, LOW);
        IDcontent1 = "";
        LockerIsLocked1 = false;
        delay(500);
      }
      else if (LockerIsLocked1 == false)
      {
        lcd.setCursor(0, 1);
        lcd.print("[ ID READ SUCCESS! ]");
        lcd.setCursor(0, 2);
        lcd.print("[  LOCKER LOCKED.  ]");
        digitalWrite(SolLock1, HIGH);
        digitalWrite(PnlLight1, HIGH);
        IDcontent1 = "";
        LockerIsLocked1 = true;
        DisinfectLock1 = true;
        delay(500);
      }
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print("[DISINFECTION MODE.]");
      lcd.setCursor(0, 2);
      lcd.print("[  LOCKER LOCKED.  ]");
      IDcontent1 = "";
      LockerIsLocked1 = true;
      delay(500);
    }
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("[   ID READ FAIL   ]");
    lcd.setCursor(0, 2);
    lcd.print("[INVALID CREDENTIAL]");
    IDcontent1 = "";
    delay(500);
  }
```
- StartDisinfection(X)
> the function responsible for executing the disinfection process on locker no. [X].
```java
if (currentTime - StartTime1 >= blinkDuration)
  {
    TimeLapsed1++;
    s--;
    MinRemain1 = (disinfecttime - TimeLapsed1) / 60;
    StartTime1 = currentTime;

    Serial.print(MinRemain1);
    Serial.print(":");
    Serial.println(s);
    //Serial.println(TimeLapsed1);

    if (s <= 0)
    {
      s = 60;
    }
  }

  lcd.setCursor(0, 1);
  lcd.print("[DISINFECTION MODE.]");
  lcd.setCursor(0, 2);
  lcd.print("[PLEASE WAIT: ");

  if (MinRemain1 <= 9)
  {
    lcd.print("0");
  }

  lcd.print(MinRemain1);
  lcd.print(":");
  lcd.print(String(s));
  //lcd.setCursor(0, 19);
  //lcd.print("]");
  digitalWrite(UVLight1, LOW); // turn on UV Light

  if (TimeLapsed1 >= disinfecttime)
  {
    lcd.setCursor(0, 1);
    lcd.print("[   DISINFECTION   ]");
    lcd.setCursor(0, 2);
    lcd.print("[       DONE!      ]");
    digitalWrite(UVLight1, HIGH); // turn on UV Light
    TimeLapsed1 = 0;
    StartTime1 = 0;
    TimeLapsed1 = 0;
    DisinfectLock1 = false;
    delay(500);
  }
```
