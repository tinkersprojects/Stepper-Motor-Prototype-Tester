#include <AccelStepper.h>
#include <EEPROM.h>

//Pins
#define Activepin 2
#define Enablepin 3
#define M1pin 4
#define M2pin 5
#define M3pin 6


boolean last_Activepin_state;

boolean M1 = false;
boolean M2 = false;
boolean M3 = false;

byte   ActiveType = 2;
long ActiveValue = 200;
double ActiveSpeed = 200;

byte   NonActiveType = 2;
long NonActiveValue = 200;
double NonActiveSpeed = 200;



AccelStepper stepper(1,7,8);

void setup() 
{
  read();
  Serial.begin(115200);
  Serial.println("Welcome v1.0 Stepper motor controller");
  Help();
  pinMode(Activepin, OUTPUT);
  pinMode(Enablepin, OUTPUT);
  digitalWrite(Enablepin,LOW);
  setSetting();
  
  stepper.setEnablePin(13);
  stepper.setMaxSpeed(200.0);
  stepper.setAcceleration(100000.0);
  stepper.enableOutputs();

  last_Activepin_state = !digitalRead(Activepin);
}

void loop() 
{
  serialRead();
  runmotor();
}








/****************** MOTOR CONTROL ******************/

void runmotor()
{
  if(last_Activepin_state != digitalRead(Activepin))
  {
    if(digitalRead(Activepin) == HIGH)
    {
      SetMotor(ActiveType, ActiveValue, ActiveSpeed);
    }
    else
    {
      SetMotor(NonActiveType, NonActiveValue, NonActiveSpeed);
    }
    last_Activepin_state = digitalRead(Activepin);
  }
  runsteps();
}

void SetMotor(byte command, long Value, double Speed)
{
  switch(command)
  {
    case 0:  // Stop
      stepper.setSpeed(0);
      stepper.move(0);
    break;
    case 1:  // Move to position
      stepper.setSpeed(Speed);
      stepper.moveTo(Value);
      while(!hasFinshedStepping()){runsteps();}
    break;
    case 2:  // Move Amount from current loction
      stepper.setSpeed(Speed);
      stepper.move(Value);
      while(!hasFinshedStepping()){runsteps();}
    break;
    case 3:  // Move constly 
      stepper.setSpeed(Speed);
      stepper.move(Value);
      last_Activepin_state = digitalRead(Activepin); // needs to be set again
    break;
    case 4:  // Move to position and can be interupeted
      stepper.setSpeed(Speed);
      stepper.moveTo(Value);
    break;
    case 5:  // Move Amount from current loction and can be interupeted
      stepper.setSpeed(Speed);
      stepper.move(Value);
    break;
  }
}

void setSetting()
{
  if(M1)
  digitalWrite(M1pin,HIGH);
  else
  digitalWrite(M1pin,LOW);
  
  if(M2)
  digitalWrite(M2pin,HIGH);
  else
  digitalWrite(M2pin,LOW);
  
  if(M3)
  digitalWrite(M3pin,HIGH);
  else
  digitalWrite(M3pin,LOW);
}

bool hasFinshedStepping()
{
    return stepper.distanceToGo()!=0;
}

void runsteps()
{
  stepper.run();
}







/****************** COMPUTER INTERFACE ******************/

String inputString = "";
char menu = 0;
char menuStep = 0;

void serialRead() 
{
  while (Serial.available()) 
  {
    char inChar = (char)Serial.read();
    if(menu == 0)
    {
      if(inChar>='a')inChar = inChar - ('a'-'A');
      menu = inChar;
      menuStep = 0;
      while((char)Serial.read() != '\n'){runmotor();}
      ReceiveCommand(0);
      inputString = "";
      return;
    }
    else
    {
      if (inChar == '\n') 
      {
        menuStep++;
        ReceiveCommand(inputString.toDouble());
        inputString = "";
        return;
      }
      else
      {
        inputString += inChar;
      }
    }
  }
}

void ReceiveCommand(double Value)
{
   switch(menu)
   {
     case 'H': 
      Help();
      break;
     case 'P':
      Program(Value); 
      break;
     case 'A': 
      MoveAmount(Value);
      break;
     case 'T': 
      MoveTo(Value);
      break;
     case 'S': 
      Setting(Value);
      break;
     case 'D': 
      Display();
      break;
   }
}


void Program(double Value)
{
  if((menuStep == 2 && Value == 3) || (menuStep == 5 && Value == 3)) menuStep++;

  switch(menuStep)
  {
    case 1:
      Serial.print("Enter ON Type:");
    break;
    case 2:
      ActiveType = Value;
      Serial.println(Value);
      Serial.print("Enter ON Distance:");
    break;
    case 3:
      ActiveValue = Value;
      Serial.println(Value);
      Serial.print("Enter ON Speed:");
    break;
    case 4:
      ActiveSpeed = Value;
      Serial.println(Value);
      Serial.print("Enter OFF Type:");
    break;
    case 5:
      NonActiveType = Value;
      Serial.println(Value);
      Serial.print("Enter OFF Distance:");
    break;
    case 6:
      NonActiveValue = Value;
      Serial.println(Value);
      Serial.print("Enter OFF Speed:");
    break;
    case 7:
      NonActiveSpeed = Value;
      menu = 0;
      Serial.println(Value);
      Serial.println("Save");
      save();
    break;
  }
}

void MoveAmount(double Value)
{
  stepper.move(Value);
  menu = 0;
}

void MoveTo(double Value)
{
  stepper.moveTo(Value);
  menu = 0;
}

void Setting(double Value)
{
  switch(menuStep)
  {
    case 1:
      Serial.print("Enter M1:");
    break;
    case 2:
      if(Value >0)
        M1 = true;
      else
        M1 = false;
      Serial.println(M1);
      Serial.print("Enter M2:");
    break;
    case 3:
      if(Value >0)
        M2 = true;
      else
        M2 = false;
      Serial.println(M2);
      Serial.print("Enter M3:");
    break;
    case 4:
      if(Value >0)
        M3 = true;
      else
        M3 = false;
      menu = 0;
      Serial.println(M3);
      Serial.println("Save");
      save();
      setSetting();
    break;
  }
}


void Help()
{
  menu = 0;
  Serial.println("Commads:");
  Serial.println("\tP - Program (ON Type, ON Distance, ON Speed, OFF Type, OFF Distance, OFF Speed)");
  Serial.println("\tA - Move amount (Distance)");
  Serial.println("\tT - Move To positon (Distance)");
  Serial.println("\tS - Set setting (m1, m2, m3)");
  Serial.println("\tD - Display details");
  Serial.println("\tH - Help");
  Serial.println();
  Serial.println("Type:");
  Serial.println("\t0 - Stop");
  Serial.println("\t1 - Move to position");
  Serial.println("\t2 - Move amount");
  Serial.println("\t3 - Continuously moving");
}



void Display()
{
  menu = 0;
  Serial.println("ON:");
  Serial.print("\tType: ");
  Serial.println(ActiveType);
  Serial.print("\tDistance: ");
  Serial.println(ActiveValue);
  Serial.print("\tSpeed:");
  Serial.println(ActiveSpeed);
  Serial.println();
  Serial.println("OFF:");
  Serial.print("\tType: ");
  Serial.println(NonActiveType);
  Serial.print("\tDistance: ");
  Serial.println(NonActiveValue);
  Serial.print("\tSpeed: ");
  Serial.println(NonActiveSpeed);
  Serial.println();
  Serial.println("Settings:");
  Serial.print("\tM1: ");
  Serial.println(M1);
  Serial.print("\tM2: ");
  Serial.println(M2);
  Serial.print("\tM3: ");
  Serial.println(M3);
  Serial.println();
}







/****************** STORAGE ******************/

void read()
{
  if(EEPROM.read(0) == 1)
  {
    M1 = EEPROM.read(1);
    M2 = EEPROM.read(2);
    M3 = EEPROM.read(3);

    String readString = "";
    char letter = 0;
    for(int i = 20;i<2000;i++)
    {
      char CHAR = EEPROM.read(i);
      if(CHAR >= 'A' && CHAR < 'Z' && letter != 0)
      {
        switch(letter)
        {
          case 'A':
            ActiveType = readString.toInt();
          break;
          case 'B':
            ActiveValue = readString.toInt();
          break;
          case 'C':
            ActiveSpeed = readString.toDouble();
          break;
          case 'D':
            NonActiveType = readString.toInt();
          break;
          case 'E':
            NonActiveValue = readString.toInt();
          break;
          case 'F':
            NonActiveSpeed = readString.toDouble();
          break;
          case 'G':
            return;
          break;
        }
      }
      if(CHAR >= 'A' && CHAR < 'Z')
      {
        letter = CHAR;
        readString = "";
      }
      else
      {
        readString += CHAR;
      }
    }
  }
  else
  {
    save();
  }
}

void save()
{
  EEPROM.write(0, 1);
  EEPROM.write(1, M1);
  EEPROM.write(2, M2);
  EEPROM.write(3, M3);

  String saveString = "A"+String(ActiveType);
  saveString += "B"+String(ActiveValue);
  saveString += "C"+String(ActiveSpeed);
  saveString += "D"+String(NonActiveType);
  saveString += "E"+String(NonActiveValue);
  saveString += "F"+String(NonActiveSpeed);
  saveString += "G";

  for(int i = 0;i<saveString.length();i++)
  {
    EEPROM.write(20+i, saveString.charAt(i));
  }
}
