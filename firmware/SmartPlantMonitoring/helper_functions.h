#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "SSD1306Wire.h"
#include "Wire.h"
#include "credentials.h"

#define PUMP_URL "/Smart_Plant_Monitoring/Pump"
#define TEMPERATURE_URL "/Smart_Plant_Monitoring/Temperature"
#define WATER_URL "/Smart_Plant_Monitoring/Water"
#define MOISTURE_URL "/Smart_Plant_Monitoring/Moisture"
#define HUMIDITY_URL "/Smart_Plant_Monitoring/Humidity"
#define MASTERCONTROL_URL "/Smart_Plant_Monitoring/MasterControl"

#define PumpPin D3
#define DHTPIN D4
#define MuxSelectPin D5
#define AnalogPin A0

#define SerialBaudRate 115200
#define TempHigh 30
#define Moisture_Wet 500
#define Moisture_Dry 750
#define Water_Low 450
#define Water_High 600
#define MaxCount 1000000

extern bool signupOk;
extern bool Decision1;
extern bool Decision2;
extern bool MasterControl;
extern bool ManualControl;
extern uint16_t Water;
extern uint16_t Moisture;
extern float Temperature;
extern float Humidity;
extern String PumpStatus;

extern FirebaseData cloud;
extern FirebaseConfig config;
extern FirebaseAuth auth;
extern SSD1306Wire display;

void initWiFi();
void onWiFiConnect();
void initFirebase();
void onFirebaseSignUp();
void onFirebaseSignUpError();
void initAnalogSensors();
void initPump();
void initDisplay();
bool getDHTData();
bool getMoistureData();
bool getWaterData();
void controlPump();
void updateFirebase();
void updateScreen();

#endif
