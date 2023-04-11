// IOT LOCKER SYSTEM V1.0 (12142022)
// ARDUINO PROGRAM

// REQUIRED PARTS:
// 4x 20x4 i2c LCD
// 4x MFRC522 RFID MODULE
// 4x HX711 WEIGHT SENSOR (LOAD CELL)
// 1x 16-RELAY MODULE

#include <SerialCommand.h>
#include <OneWire.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <HX711_ADC.h>
#include <SPI.h>
#include <MFRC522.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// LCD VARIABLES // CHANGE I2C ADDRESS OF EACH LCD
LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD Module for Locker 1 [CHANGE I2C ADDRESS]
LiquidCrystal_I2C  lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD Module for Locker 2 [CHANGE I2C ADDRESS]
LiquidCrystal_I2C  lcd3(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD Module for Locker 3 [CHANGE I2C ADDRESS]
LiquidCrystal_I2C  lcd4(0x23, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD Module for Locker 4 [CHANGE I2C ADDRESS]

// RFID VARIABLES

String RFID1 = ""; // set the RFID contents here
String RFID2 = "";
String RFID3 = "";
String RFID4 = "";

#define RST_PIN 5
#define SS_PIN1 6
//#define SS_PIN2 7
//#define SS_PIN3 8
//#define SS_PIN4 9

String IDcontent1 = "";
String IDcontent2 = "";
String IDcontent3 = "";
String IDcontent4 = "";

MFRC522 RfidMod1(SS_PIN1, RST_PIN); //RFID Module for Locker 1
//MFRC522 RfidMod2(SS_PIN2, RST_PIN); //RFID Module for Locker 2
//MFRC522 RfidMod3(SS_PIN3, RST_PIN); //RFID Module for Locker 3
//MFRC522 RfidMod4(SS_PIN4, RST_PIN); //RFID Module for Locker 4

// WEIGHT VARIABLES
#define HX711_dout1 42
#define HX711_sck1 43
#define HX711_dout2 44
#define HX711_sck2 45
#define HX711_dout3 46
#define HX711_sck3 47
#define HX711_dout4 48
#define HX711_sck4 49

HX711_ADC LoadCell1(HX711_dout1, HX711_sck1); //load cell for locker 1
HX711_ADC LoadCell2(HX711_dout2, HX711_sck2); //load cell for locker 2
HX711_ADC LoadCell3(HX711_dout3, HX711_sck3); //load cell for locker 3
HX711_ADC LoadCell4(HX711_dout4, HX711_sck4); //load cell for locker 4

float weight1, weight2, weight3, weight4;
const int calVal_eepromAdress = 0;
unsigned long t = 0;

// DELAY/TIMING VARIABLES
//int disinfecttime = 1680; //28 minutes
int disinfecttime = 600; //28 minutes
unsigned long previousOnBoardLedMillis = 0;
const int blinkDuration = 1000;

unsigned long StartTime1 = millis();
unsigned long StartTime2 = millis();
unsigned long StartTime3 = millis();
unsigned long StartTime4 = millis();

unsigned long currentTime;

unsigned int TimeLapsed1;
unsigned int TimeLapsed2;
unsigned int TimeLapsed3;
unsigned int TimeLapsed4;

int MinRemain1;
int MinRemain2;
int MinRemain3;
int MinRemain4;

int s = 60;
int s2 = 60;
int s3 = 60;
int s4 = 60;

// RELAY VARIABLES
#define UVLight1 30
#define UVLight2 31
#define UVLight3 32
#define UVLight4 33

#define PnlLight1 34
#define PnlLight2 35
#define PnlLight3 36
#define PnlLight4 37

#define SolLock1 38
#define SolLock2 39
#define SolLock3 40
#define SolLock4 41

// Limit Switches
#define Locker1Switch 8
#define Locker2Switch 9
#define Locker3Switch 10
#define Locker4Switch 11

int Locker1SwitchStat;
int Locker2SwitchStat;
int Locker3SwitchStat;
int Locker4SwitchStat;

// GENERAL VARIABLES
bool DisinfectLock1 = false;
bool DisinfectLock2 = false;
bool DisinfectLock3 = false;
bool DisinfectLock4 = false;

String ParcelStr1, ParcelStr2, ParcelStr3, ParcelStr4;
bool LockerIsLocked1, LockerIsLocked2, LockerIsLocked3, LockerIsLocked4;

// SERIAL VARIABLE
SerialCommand sCmd;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialization Start");
  SPI.begin();

  RfidMod1.PCD_Init();
  RfidMod1.PCD_DumpVersionToSerial();

  LoadCell1.begin();
  LoadCell2.begin();
  LoadCell3.begin();
  LoadCell4.begin();

  lcd.begin(20, 4);
  lcd2.begin(20, 4);
  lcd3.begin(20, 4);
  lcd4.begin(20, 4);

  lcd.backlight();
  lcd2.backlight();
  lcd3.backlight();
  lcd4.backlight();

  lcd.setCursor(0, 0);
  lcd.print("IOT LOCKER SYSTEM v1");
  lcd.setCursor(0, 1);
  lcd.print("[ PROGRAM STARTING ]");
  lcd.setCursor(0, 2);
  lcd.print("[   PLEASE  WAIT   ]");
  lcd.setCursor(0, 3);
  lcd.print("[   FOR A MOMENT   ]");

  pinMode(Locker1Switch, INPUT);
  pinMode(Locker2Switch, INPUT);
  pinMode(Locker3Switch, INPUT);
  pinMode(Locker4Switch, INPUT);

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

  //INITIALIZE RELAYS (TURN OFF ALL RELAY)

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
  Serial.println("Initialization Complete");
}

void loop()
{
  sCmd.readSerial();
  Locker1SwitchStat = digitalRead(Locker1Switch);
  Locker2SwitchStat = digitalRead(Locker2Switch);
  Locker3SwitchStat = digitalRead(Locker3Switch);
  Locker4SwitchStat = digitalRead(Locker4Switch);

  // TIMER/DELAY
  currentTime = millis(); //TIMER

  // COMMAND WATCHER
  watcher();

  // RFID FUNCTIONS
  ReadRFID1();

  // WEIGHT FUNCTIONS
  ReadWeight1();
  ReadWeight2();
  ReadWeight3();
  ReadWeight4();
}

///////////// INITIALIZE SERIAL COMMAND FUNCTIONS /////////////

void initSerialCommands()
{
  sCmd.addCommand("Set1Parcel", SetParcel1);
  sCmd.addCommand("Set2Parcel", SetParcel2);
  sCmd.addCommand("Set3Parcel", SetParcel3);
  sCmd.addCommand("Set4Parcel", SetParcel4);

  sCmd.addCommand("Lock1Locker", LockLocker1);
  sCmd.addCommand("Lock2Locker", LockLocker2);
  sCmd.addCommand("Lock3Locker", LockLocker3);
  sCmd.addCommand("Lock4Locker", LockLocker4);

  sCmd.addCommand("Unlock1Locker", UnlockLocker1);
  sCmd.addCommand("Unlock2Locker", UnlockLocker2);
  sCmd.addCommand("Unlock3Locker", UnlockLocker3);
  sCmd.addCommand("Unlock4Locker", UnlockLocker4);

  sCmd.addCommand("Disinf1Locker", DisinfectLocker1);
  sCmd.addCommand("Disinf2Locker", DisinfectLocker2);
  sCmd.addCommand("Disinf3Locker", DisinfectLocker3);
  sCmd.addCommand("Disinf4Locker", DisinfectLocker4);
}
///////////// DATA FUNCTIONS /////////////

void SetParcel1()
{
  String Par;
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    Par = String(arg);
    ParcelStr1 = Par;
  }
}

void SetParcel2()
{
  String Par;
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    Par = String(arg);
    ParcelStr2 = Par;
  }
}

void SetParcel3()
{
  String Par;
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    Par = String(arg);
    ParcelStr3 = Par;
  }
}

void SetParcel4()
{
  String Par;
  char *arg;
  arg = sCmd.next();

  if (arg != NULL)
  {
    Par = String(arg);
    ParcelStr4 = Par;
  }
}

///////////// CONTROL FUNCTIONS ////////////

void LockLocker1()
{
  digitalWrite(PnlLight1, HIGH);
  digitalWrite(SolLock1, HIGH);
}

void LockLocker2()
{
  digitalWrite(PnlLight2, HIGH);
  digitalWrite(SolLock2, HIGH);
}

void LockLocker3()
{
  digitalWrite(PnlLight3, HIGH);
  digitalWrite(SolLock3, HIGH);
}

void LockLocker4()
{
  digitalWrite(PnlLight4, HIGH);
  digitalWrite(SolLock4, HIGH);
}

void UnlockLocker1()
{
  digitalWrite(PnlLight1, LOW);
  digitalWrite(SolLock1, LOW);
}

void UnlockLocker2()
{
  digitalWrite(PnlLight2, LOW);
  digitalWrite(SolLock2, LOW);
}

void UnlockLocker3()
{
  digitalWrite(PnlLight3, LOW);
  digitalWrite(SolLock3, LOW);
}

void UnlockLocker4()
{
  digitalWrite(PnlLight4, LOW);
  digitalWrite(SolLock4, LOW);
}

void DisinfectLocker1()
{
  String Par;
  char *arg;
  arg = sCmd.next();

  if (arg != NULL)
  {
    if (DisinfectLock1 == false)
    {
      Par = String(arg);
      s = Par.toInt();
      DisinfectLock1 = true;
    }
  }
}

void DisinfectLocker2()
{
  String Par;
  char *arg;
  arg = sCmd.next();

  if (arg != NULL)
  {
    if (DisinfectLock2 == false)
    {
      Par = String(arg);
      s2 = Par.toInt();
      DisinfectLock2 = true;
    }
  }
}

void DisinfectLocker3()
{
  String Par;
  char *arg;
  arg = sCmd.next();

  if (arg != NULL)
  {
    if (DisinfectLock3 == false)
    {
      Par = String(arg);
      s3 = Par.toInt();
      DisinfectLock3 = true;
    }
  }
}

void DisinfectLocker4()
{
  String Par;
  char *arg;
  arg = sCmd.next();

  if (arg != NULL)
  {
    if (DisinfectLock4 == false)
    {
      Par = String(arg);
      s4 = Par.toInt();
      DisinfectLock4 = true;
    }
  }
}

///////////// MAJOR FUNCTIONS //////////////

void watcher()
{
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
}

//////////////// WEIGHT READING FUNCTION ////////////////

void ReadWeight1()
{
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
}

void ReadWeight2()
{
  static boolean newDataReady = 0;

  if (LoadCell2.update())
  {
    newDataReady = true;
  }

  if (newDataReady)
  {
    weight2 = LoadCell2.getData();
    weight2 = weight2 / 1000.00;
    newDataReady = 0;
    weight2 = abs(weight2);
  }
}

void ReadWeight3()
{
  static boolean newDataReady = 0;

  if (LoadCell3.update())
  {
    newDataReady = true;
  }

  if (newDataReady)
  {
    weight3 = LoadCell3.getData();
    weight3 = weight3 / 1000.00;
    newDataReady = 0;
    weight3 = abs(weight3);
  }
}

void ReadWeight4()
{
  static boolean newDataReady = 0;

  if (LoadCell4.update())
  {
    newDataReady = true;
  }

  if (newDataReady)
  {
    weight4 = LoadCell4.getData();
    weight4 = weight4 / 1000.00;
    newDataReady = 0;
    weight4 = abs(weight4);
  }
}

//////////////// RFID READING FUNCTION ////////////////

void ReadRFID1()
{
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
        if (Locker1SwitchStat == 1)
        {
          lcd.setCursor(0, 1);
          lcd.print("[ ID READ SUCCESS! ]");
          lcd.setCursor(0, 2);
          lcd.print("[  LOCKER LOCKED.  ]");
          digitalWrite(SolLock1, HIGH);
          digitalWrite(PnlLight1, HIGH);
          IDcontent1 = "";
          LockerIsLocked1 = true;
          //DisinfectLock1 = true;
          delay(500);
        }
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("[ ID READ SUCCESS! ]");
          lcd.setCursor(0, 2);
          lcd.print("[  LOCKER IS OPEN  ]");
          IDcontent1 = "";
          LockerIsLocked1 = false;
          delay(500);
        }
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
  else if (IDcontent1.substring(1) == RFID2) // Make sure you change this with your own UID number
  {
    if (DisinfectLock2 == false)
    {
      if (LockerIsLocked2 == true)
      {
        lcd2.setCursor(0, 1);
        lcd2.print("[ ID READ SUCCESS! ]");
        lcd2.setCursor(0, 2);
        lcd2.print("[ LOCKER UNLOCKED. ]");
        digitalWrite(SolLock2, LOW);
        digitalWrite(PnlLight2, LOW);
        IDcontent2 = "";
        LockerIsLocked2 = false;
        delay(500);
      }
      else if (LockerIsLocked2 == false)
      {
        if (Locker2SwitchStat == 1)
        {
          lcd2.setCursor(0, 1);
          lcd2.print("[ ID READ SUCCESS! ]");
          lcd2.setCursor(0, 2);
          lcd2.print("[  LOCKER LOCKED.  ]");
          digitalWrite(SolLock2, HIGH);
          digitalWrite(PnlLight2, HIGH);
          IDcontent2 = "";
          LockerIsLocked2 = true;
          //DisinfectLock2 = true;
          delay(500);
        }
        else
        {
          lcd2.setCursor(0, 1);
          lcd2.print("[ ID READ SUCCESS! ]");
          lcd2.setCursor(0, 2);
          lcd2.print("[  LOCKER IS OPEN  ]");
          IDcontent2 = "";
          LockerIsLocked2 = false;
          delay(500);
        }
      }
    }
    else
    {
      lcd2.setCursor(0, 1);
      lcd2.print("[DISINFECTION MODE.]");
      lcd2.setCursor(0, 2);
      lcd2.print("[  LOCKER LOCKED.  ]");
      IDcontent2 = "";
      LockerIsLocked2 = true;
      delay(500);
    }
  }
  else if (IDcontent1.substring(1) == RFID3) // Make sure you change this with your own UID number
  {
    if (DisinfectLock3 == false)
    {
      if (LockerIsLocked3 == true)
      {
        lcd3.setCursor(0, 1);
        lcd3.print("[ ID READ SUCCESS! ]");
        lcd3.setCursor(0, 2);
        lcd3.print("[ LOCKER UNLOCKED. ]");
        digitalWrite(SolLock3, LOW);
        digitalWrite(PnlLight3, LOW);
        IDcontent3 = "";
        LockerIsLocked3 = false;
        delay(500);
      }
      else if (LockerIsLocked3 == false)
      {
        if (Locker3SwitchStat == 1)
        {
          lcd3.setCursor(0, 1);
          lcd3.print("[ ID READ SUCCESS! ]");
          lcd3.setCursor(0, 2);
          lcd3.print("[  LOCKER LOCKED.  ]");
          digitalWrite(SolLock3, HIGH);
          digitalWrite(PnlLight3, HIGH);
          IDcontent3 = "";
          LockerIsLocked3 = true;
          //DisinfectLock3 = true;
          delay(500);
        }
        else
        {
          lcd3.setCursor(0, 1);
          lcd3.print("[ ID READ SUCCESS! ]");
          lcd3.setCursor(0, 2);
          lcd3.print("[  LOCKER IS OPEN  ]");
          IDcontent3 = "";
          LockerIsLocked3 = false;
          delay(500);
        }
      }
    }
    else
    {
      lcd3.setCursor(0, 1);
      lcd3.print("[DISINFECTION MODE.]");
      lcd3.setCursor(0, 2);
      lcd3.print("[  LOCKER LOCKED.  ]");
      IDcontent3 = "";
      LockerIsLocked3 = true;
      delay(500);
    }
  }
  else if (IDcontent1.substring(1) == RFID3) // Make sure you change this with your own UID number
  {
    if (DisinfectLock4 == false)
    {
      if (LockerIsLocked4 == true)
      {
        lcd4.setCursor(0, 1);
        lcd4.print("[ ID READ SUCCESS! ]");
        lcd4.setCursor(0, 2);
        lcd4.print("[ LOCKER UNLOCKED. ]");
        digitalWrite(SolLock4, LOW);
        digitalWrite(PnlLight4, LOW);
        IDcontent4 = "";
        LockerIsLocked4 = false;
        delay(500);
      }
      else if (LockerIsLocked4 == false)
      {
        if (Locker4SwitchStat == 1)
        {
          lcd4.setCursor(0, 1);
          lcd4.print("[ ID READ SUCCESS! ]");
          lcd4.setCursor(0, 2);
          lcd4.print("[  LOCKER LOCKED.  ]");
          digitalWrite(SolLock4, HIGH);
          digitalWrite(PnlLight4, HIGH);
          IDcontent4 = "";
          LockerIsLocked4 = true;
          //DisinfectLock4 = true;
          delay(500);
        }
        else
        {
          lcd4.setCursor(0, 1);
          lcd4.print("[ ID READ SUCCESS! ]");
          lcd4.setCursor(0, 2);
          lcd4.print("[  LOCKER IS OPEN  ]");
          IDcontent4 = "";
          LockerIsLocked4 = false;
          delay(500);
        }
      }
    }
    else
    {
      lcd4.setCursor(0, 1);
      lcd4.print("[DISINFECTION MODE.]");
      lcd4.setCursor(0, 2);
      lcd4.print("[  LOCKER LOCKED.  ]");
      IDcontent4 = "";
      LockerIsLocked4 = true;
      delay(500);
    }
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("[   ID READ FAIL   ]");
    lcd.setCursor(0, 2);
    lcd.print("[INVALID CREDENTIAL]");

    lcd2.setCursor(0, 1);
    lcd2.print("[   ID READ FAIL   ]");
    lcd2.setCursor(0, 2);
    lcd2.print("[INVALID CREDENTIAL]");

    lcd3.setCursor(0, 1);
    lcd3.print("[   ID READ FAIL   ]");
    lcd3.setCursor(0, 2);
    lcd3.print("[INVALID CREDENTIAL]");

    lcd4.setCursor(0, 1);
    lcd4.print("[   ID READ FAIL   ]");
    lcd4.setCursor(0, 2);
    lcd4.print("[INVALID CREDENTIAL]");

    IDcontent1 = "";
    delay(500);
  }
}

//////////////// DISINFECTION FUNCTION ////////////////

void StartDisinfection1()
{
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
}

void StartDisinfection2()
{
  if (currentTime - StartTime2 >= blinkDuration)
  {
    TimeLapsed2++;
    s2--;
    MinRemain2 = (disinfecttime - TimeLapsed2) / 60;
    StartTime2 = currentTime;

    Serial.print(MinRemain2);
    Serial.print(":");
    Serial.println(s2);
    //Serial.println(TimeLapsed1);

    if (s2 <= 0)
    {
      s2 = 60;
    }
  }

  lcd2.setCursor(0, 1);
  lcd2.print("[DISINFECTION MODE.]");
  lcd2.setCursor(0, 2);
  lcd2.print("[PLEASE WAIT: ");

  if (MinRemain2 <= 9)
  {
    lcd2.print("0");
  }

  lcd2.print(MinRemain2);
  lcd2.print(":");
  lcd2.print(String(s2));
  //lcd.setCursor(0, 19);
  //lcd.print("]");
  digitalWrite(UVLight2, LOW); // turn on UV Light

  if (TimeLapsed2 >= disinfecttime)
  {
    lcd2.setCursor(0, 1);
    lcd2.print("[   DISINFECTION   ]");
    lcd2.setCursor(0, 2);
    lcd2.print("[       DONE!      ]");
    digitalWrite(UVLight2, HIGH); // turn on UV Light
    TimeLapsed2 = 0;
    StartTime2 = 0;
    TimeLapsed2 = 0;
    DisinfectLock2 = false;
    delay(500);
  }
}

void StartDisinfection3()
{
  if (currentTime - StartTime3 >= blinkDuration)
  {
    TimeLapsed3++;
    s3--;
    MinRemain3 = (disinfecttime - TimeLapsed3) / 60;
    StartTime3 = currentTime;

    Serial.print(MinRemain3);
    Serial.print(":");
    Serial.println(s3);
    //Serial.println(TimeLapsed1);

    if (s3 <= 0)
    {
      s3 = 60;
    }
  }

  lcd3.setCursor(0, 1);
  lcd3.print("[DISINFECTION MODE.]");
  lcd3.setCursor(0, 2);
  lcd3.print("[PLEASE WAIT: ");

  if (MinRemain3 <= 9)
  {
    lcd3.print("0");
  }

  lcd3.print(MinRemain3);
  lcd3.print(":");
  lcd3.print(String(s3));
  //lcd.setCursor(0, 19);
  //lcd.print("]");
  digitalWrite(UVLight3, LOW); // turn on UV Light

  if (TimeLapsed3 >= disinfecttime)
  {
    lcd3.setCursor(0, 1);
    lcd3.print("[   DISINFECTION   ]");
    lcd3.setCursor(0, 2);
    lcd3.print("[       DONE!      ]");
    digitalWrite(UVLight3, HIGH); // turn on UV Light
    TimeLapsed3 = 0;
    StartTime3 = 0;
    TimeLapsed3 = 0;
    DisinfectLock3 = false;
    delay(500);
  }
}


void StartDisinfection4()
{
  if (currentTime - StartTime4 >= blinkDuration)
  {
    TimeLapsed4++;
    s4--;
    MinRemain4 = (disinfecttime - TimeLapsed3) / 60;
    StartTime4 = currentTime;

    Serial.print(MinRemain4);
    Serial.print(":");
    Serial.println(s4);
    //Serial.println(TimeLapsed1);

    if (s4 <= 0)
    {
      s4 = 60;
    }
  }

  lcd4.setCursor(0, 1);
  lcd4.print("[DISINFECTION MODE.]");
  lcd4.setCursor(0, 2);
  lcd4.print("[PLEASE WAIT: ");

  if (MinRemain4 <= 9)
  {
    lcd4.print("0");
  }

  lcd4.print(MinRemain4);
  lcd4.print(":");
  lcd4.print(String(s4));
  //lcd.setCursor(0, 19);
  //lcd.print("]");
  digitalWrite(UVLight4, LOW); // turn on UV Light

  if (TimeLapsed4 >= disinfecttime)
  {
    lcd4.setCursor(0, 1);
    lcd4.print("[   DISINFECTION   ]");
    lcd4.setCursor(0, 2);
    lcd4.print("[       DONE!      ]");
    digitalWrite(UVLight4, HIGH); // turn on UV Light
    TimeLapsed4 = 0;
    StartTime4 = 0;
    TimeLapsed4 = 0;
    DisinfectLock4 = false;
    delay(500);
  }
}
