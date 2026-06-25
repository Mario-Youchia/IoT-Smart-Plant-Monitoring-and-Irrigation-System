#include "helper_functions.h"
#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"

void setup() {
  Serial.begin(SerialBaudRate);
  initWiFi();
  initFirebase();
  initAnalogSensors();
  initPump();
  initDisplay();
  updateScreen();
}

void loop() {
  Serial.println("********** New Loop **********");
  Decision2 = getDHTData();
  Decision1 = getMoistureData();
  delay(100);
  MasterControl = getWaterData();
  delay(1000);
  controlPump();
  updateFirebase();
  updateScreen();
}
