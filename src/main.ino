#include "application.h"
#include "winecooler.h"

winecooler _winecooler;

void setup() {
  _winecooler.setup();
}

void loop() {
  _winecooler.loop();
}
