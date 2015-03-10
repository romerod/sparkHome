#include "application.h"
#include "winecooler.h"

#define up D1
#define down D0
#define pwr D2
#define interruptUp A2
#define interruptDown A1
#define maxTemperatur 18
#define minTemperatur 11

int winecooler::currentTemperature;
int winecooler::newTemperature;
unsigned long winecooler::lastPress;

void winecooler::setup() {
    currentTemperature = 12;
    newTemperature = 16;

    Serial.begin(9600);

    Spark.variable("currentTemp", &currentTemperature, INT);
    Spark.function("pressUp", winecooler::pressUp);
    Spark.function("pressDown", winecooler::pressDown);
    Spark.function("setTemp", winecooler::setTemperature);
    Spark.function("wc-power", winecooler::power);

    pinMode(up, OUTPUT);
    pinMode(down, OUTPUT);
    pinMode(pwr, OUTPUT);

    Serial.println("winecooler: on");

    digitalWrite(pwr, HIGH);
    delay(500);

    attachInterrupt(interruptUp, winecooler::upPressed, RISING);
    attachInterrupt(interruptDown, winecooler::downPressed, RISING);
}

void winecooler::loop() {
  int pin = -1;

  // 1 degree per loop
  if(currentTemperature > newTemperature) {
    Serial.println("down");
    pin = down;
  } else if (currentTemperature < newTemperature) {
    Serial.println("up");
    pin = up;
  }

  if(pin > -1)
  {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(250);
  }
}

//
//  Power control
//
int winecooler::power(String command) {
  if(command.equalsIgnoreCase("on")) {
    Serial.println("winecooler: on");
    digitalWrite(pwr, HIGH);
    int lastTemp = currentTemperature;
    delay(1000);
    currentTemperature = 12;
    newTemperature = lastTemp;
  } else if (command.equalsIgnoreCase("off")) {
    Serial.println("winecooler: off");
    digitalWrite(pwr, LOW);
  }
}

//
//  Temperature control
//
int winecooler::setTemperature(String command) {
  return setNewTemperature(command.toInt());
}

int winecooler::pressUp(String command) {
  return setNewTemperature(currentTemperature + 1);
}

int winecooler::pressDown(String command) {
  return setNewTemperature(currentTemperature - 1);
}

int winecooler::setNewTemperature(int newTemp)
{
  if(newTemp >= minTemperatur && newTemp <= maxTemperatur) {
    newTemperature = newTemp;
  }
  return newTemperature;
}

//
//  Control watch
//
void winecooler::upPressed() {
  Serial.println("winecooler: user pressed up");
  handlePressed(1);
}

void winecooler::downPressed() {
  Serial.println("winecooler: user pressed down");
  handlePressed(-1);
}

void winecooler::handlePressed(int tempDelta)
{
  if(millis()-lastPress < 1000) {
      bool isManualPress = currentTemperature == newTemperature;
      int currentTemp = currentTemperature + tempDelta;
      if(currentTemp >= minTemperatur && currentTemp <= maxTemperatur) {
        currentTemperature = currentTemp;
        Spark.publish("winecooler-settings-temp", String(currentTemperature, DEC));
        if(isManualPress)
        {
          Serial.print("winecooler: user selected ");
          Serial.println(currentTemperature);
          newTemperature = currentTemperature;
        } else {
          Serial.print("winecooler: currentTemperature");
          Serial.println(currentTemperature);
        }
      }
  }
  lastPress = millis();
}
