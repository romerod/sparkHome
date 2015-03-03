#define up D1
#define down D0
#define interruptUp A0
#define interruptDown A1
#define maxTemperatur 18
#define minTemperatur 11

int currentTemperature;
int newTemperature;
unsigned long lastPress;

int pressUp(String command);
int pressDown(String command);
int setTemperature(String command);

void setup() {
    currentTemperature = 16;
    newTemperature = 16;
    Spark.variable("currentTemperature", &currentTemperature, INT);
    Spark.function("pressUp", pressUp);
    Spark.function("pressDown", pressDown);
    Spark.function("setTemp", setTemperature);

    pinMode(up, OUTPUT);
    pinMode(down, OUTPUT);

    attachInterrupt(interruptUp, upPressed, RISING);
    attachInterrupt(interruptDown, downPressed, RISING);
}

void loop() {
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

int setTemperature(String command) {
  return setNewTemperature(command.toInt());
}

int pressUp(String command) {
  return setNewTemperature(currentTemperature + 1);
}

int pressDown(String command) {
  return setNewTemperature(currentTemperature - 1);
}

int setNewTemperature(int newTemp)
{
  if(newTemp >= minTemperatur && newTemp <= maxTemperatur) {
    newTemperature = newTemp;
  }
  return newTemperature;
}

//
//  Control watch
//

void upPressed() {
    handlePressed(1);
}

void downPressed() {
    handlePressed(-1);
}

void handlePressed(int tempDelta)
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
