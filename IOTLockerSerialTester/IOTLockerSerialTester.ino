#include <SerialCommand.h>

SerialCommand sCmd;

#define Locker1Switch 8
#define Locker2Switch 9
#define Locker3Switch 10
#define Locker4Switch 11

int Locker1SwitchStat = 0;
int Locker2SwitchStat = 0;
int Locker3SwitchStat = 0;
int Locker4SwitchStat = 0;

bool DisinfectLock1 = false;
bool DisinfectLock2 = false;
bool DisinfectLock3 = false;
bool DisinfectLock4 = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialization Start");
  
  pinMode(Locker1Switch, INPUT_PULLUP);
  pinMode(Locker2Switch, INPUT_PULLUP);
  pinMode(Locker3Switch, INPUT_PULLUP);
  pinMode(Locker4Switch, INPUT_PULLUP);
  
  initSerialCommands();
  Serial.println("Initialization Complete");
}


void initSerialCommands()
{
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
}

void SetParcel1()
{
  String Par;
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    Par = String(arg);
      Serial.print("SetParcel1 ");
      Serial.println(Par);
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
      Serial.print("SetParcel2 ");
      Serial.println(Par);
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
      Serial.print("SetParcel3 ");
      Serial.println(Par);
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
      Serial.print("SetParcel4 ");
      Serial.println(Par);
  }
}

void LockLocker1()
{
  Serial.println("Locker 1 Locked");
}

void LockLocker2()
{
  Serial.println("Locker 2 Locked");
}

void LockLocker3()
{
  Serial.println("Locker 3 Locked");
}

void LockLocker4()
{
  Serial.println("Locker 4 Locked");
}

void UnlockLocker1()
{
  Serial.println("Locker 1 Unlocked");
}

void UnlockLocker2()
{
  Serial.println("Locker 2 Unlocked");
}

void UnlockLocker3()
{
  Serial.println("Locker 3 Unlocked");
}

void UnlockLocker4()
{
  Serial.println("Locker 4 Unlocked");
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
      Serial.print("DisinfectLocker1 ");
      Serial.println(Par);
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
      Serial.print("DisinfectLocker2 ");
      Serial.println(Par);
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
      Serial.print("DisinfectLocker3 ");
      Serial.println(Par);
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
      Serial.print("DisinfectLocker4 ");
      Serial.println(Par);
    }
  }
}

void loop()
{
  sCmd.readSerial();
  Locker1SwitchStat = digitalRead(Locker1Switch);
  Locker2SwitchStat = digitalRead(Locker2Switch);
  Locker3SwitchStat = digitalRead(Locker3Switch);
  Locker4SwitchStat = digitalRead(Locker4Switch);

  Serial.println(Locker1SwitchStat);
  Serial.println(Locker2SwitchStat);
  Serial.println(Locker3SwitchStat);
  Serial.println(Locker4SwitchStat);
  Serial.println("");
  delay(1000);
  
}
