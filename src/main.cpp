#include <Servo.h>
#include <Arduino.h>

Servo servo1, servo2, servo3,Brushless1;

//Global Variables
char c;
String dataInput;
int8_t servo1Deg=45;
int8_t servo2Deg=45;
int8_t servo3Deg=45;
int8_t brushlessSPD;
int8_t indexOfA, indexOfB, indexOfC, indexOfD, indexOfE, indexOfF, indexOfG;
bool init=false;
bool start=false;
bool stop=false;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(38400);
    Brushless1.attach(9);
    Brushless1.write(5);

    // servo.attach(pin, min, max)
    // min: the minimum pulse width [ms], which determines the 0-degree angle (default:544)
    // max: the maximum pulse width [ms], which determines the 180-degree angle (default:2400)
    servo1.attach(8); 
    servo1.write(45);
    servo2.attach(4);
    servo3.attach(5);
}

void initialize_motor() {
  Serial.print("Okay, Starting to initailize...\n");
  Serial.print("Setting high speed! and wait 2 sec! ");
  Serial.println("(hearing beep-beep)");
  delay(500);
  Brushless1.write(120);
  delay(2000);
  Serial.print("Setting back to low speed! and wait 2 sec! ");
  Serial.println("(hearing beep-beep-beep)");
  Brushless1.write(60);
  delay(2000);
  Brushless1.write(5);
  delay(2000);
  Serial.print("Testing.... ");
  Brushless1.write(59);
  delay(500);
  for (int i=60;i<=120;i++){
    Brushless1.write(i);
    delay(40);
  }
  delay(500);
  for (int i=120;i>=60;i--){
    Brushless1.write(i);
    delay(40);
  }
  Brushless1.write(5);
  delay(500);
  Serial.print("MOTOR IS READY! ");
  Serial.println("(hearing regular beep---beep---beep--- )");
}

void Receive_Serial_Data()
{
  while(Serial.available())
  {
    c = Serial.read();
    Serial.println(c);
    if(c == '\n') break;
    else dataInput += c;
  }
}

void Parse_Data()
{
  String str_servo1Deg, str_servo2Deg, str_servo3Deg, str_brushlessSPD, str_brushlessInit, str_brushlessStart, str_brushlessStop;
  Serial.println(dataInput);

  indexOfA = dataInput.indexOf("A");
  indexOfB = dataInput.indexOf("B");
  indexOfC = dataInput.indexOf("C");
  indexOfD = dataInput.indexOf("D");
  indexOfE = dataInput.indexOf("E");
  indexOfF = dataInput.indexOf("F");
  indexOfG = dataInput.indexOf("G");

  if(indexOfA > -1) // the index of A not found
  {
    str_servo1Deg = dataInput.substring(0, indexOfA);
    servo1Deg = str_servo1Deg.toInt(); 
  }

  if(indexOfB > -1) // the index of B not found
  {
    str_servo2Deg = dataInput.substring(indexOfA+1, indexOfB);
    servo2Deg = str_servo2Deg.toInt(); 
  }

  if(indexOfC > -1) // the index of C not found
  {
    str_servo3Deg = dataInput.substring(indexOfB+1, indexOfC);
    servo3Deg = str_servo3Deg.toInt(); 
  }

  if(indexOfD > -1) // brushless speed
  {
    str_brushlessSPD = dataInput.substring(indexOfC+1, indexOfD);
    brushlessSPD = str_brushlessSPD.toInt(); 
  }

  if(indexOfE > -1) // init
  {
    str_brushlessInit = dataInput.substring(indexOfD+1, indexOfE);
    init = str_brushlessInit=="1"; 
    if(init){
      servo1.write(45);
      initialize_motor();
      // testfunction();
    }
  }

  if(indexOfF > -1) // start
  {
    str_brushlessStart = dataInput.substring(indexOfE+1, indexOfF);
    start = str_brushlessStart=="1"; 
    stop = false;
  }

  if(indexOfG > -1) // stop
  {
    str_brushlessStop = dataInput.substring(indexOfE+1, indexOfF);
    stop = str_brushlessStop=="1"; 
    start = false;
    brushlessSPD=5;
    Brushless1.write(5);
    delay(2000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    Receive_Serial_Data();
    if(c == '\n')
    {
      Parse_Data();
      c = 0;
      dataInput = "";
    }
    if(init){
      if(start){
        Brushless1.write(brushlessSPD);
      }
    }
    servo1.write(servo1Deg);
    servo2.write(servo2Deg);
    servo3.write(servo3Deg);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

