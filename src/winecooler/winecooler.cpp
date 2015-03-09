#include "application.h"
#include "winecooler.h"

#define up D1
#define down D0
#define interruptUp A0
#define interruptDown A1
#define maxTemperatur 18
#define minTemperatur 11

winecooler* winecooler::instance;

void winecooler::setup() {
    instance = this;
    currentTemperature = 16;
    newTemperature = 16;
    Spark.variable("currentTemperature", &currentTemperature, INT);
    Spark.function("pressUp", winecooler::pressUp);
    Spark.function("pressDown", winecooler::pressDown);
    Spark.function("setTemp", winecooler::setTemperature);

    pinMode(up, OUTPUT);
    pinMode(down, OUTPUT);

    attachInterrupt(interruptUp, winecooler::upPressed, RISING);
    attachInterrupt(interruptDown, winecooler::downPressed, RISING);
}

void winecooler::loop() {
  int pin = -1;

  // 1 degree per loop
  if(currentTemperature > newTemperature) {
    pin = down;
  } else if (currentTemperature < newTemperature) {
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
//  Temperature control
//
int winecooler::setTemperature(String command) {
  return instance->setNewTemperature(command.toInt());
}

int winecooler::pressUp(String command) {
  return instance->setNewTemperature(instance->currentTemperature + 1);
}

int winecooler::pressDown(String command) {
  return instance->setNewTemperature(instance->currentTemperature - 1);
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
    instance->handlePressed(1);
}

void winecooler::downPressed() {
    instance->handlePressed(-1);
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
          newTemperature = currentTemperature;
        }
      }
  }
  lastPress = millis();
}