#include "helper_functions.h"

bool signupOk = false;
bool Decision1 = false;
bool Decision2 = false;
bool MasterControl = false;  // true means the pump is allowed to respond to control signals.
bool ManualControl = false;
uint16_t Water = 0;
uint16_t Moisture = 0;
float Temperature = 0;
float Humidity = 0;
String PumpStatus = "";
uint8_t data[5];
uint32_t cycles[80];
uint32_t count = 0;

FirebaseData cloud;
FirebaseConfig config;
FirebaseAuth auth;
SSD1306Wire display(0x3c, D2, D1);

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  onWiFiConnect();
}

void onWiFiConnect() {
  Serial.println("");
  Serial.print("Connected: ");
  Serial.print(WIFI_SSID);
  Serial.println("");
  Serial.print(WiFi.localIP());
  Serial.println("");
}

void initFirebase() {
  config.api_key = FIREBASE_API_KEY;
  config.database_url = FIREBASE_DATABASE_URL;
  Firebase.signUp(&config, &auth, "", "") ? onFirebaseSignUp() : onFirebaseSignUpError();
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void onFirebaseSignUp() {
  signupOk = true;
  Serial.println("OK");
}

void onFirebaseSignUpError() {
  Serial.printf("%s\n", config.signer.signupError.message.c_str());
  Serial.println("Error");
}

void initAnalogSensors() {
  pinMode(MuxSelectPin, OUTPUT);
}

void initPump() {
  digitalWrite(PumpPin, LOW);
  pinMode(PumpPin, OUTPUT);
}

void initDisplay() {
  display.init();
  display.clear();
}

bool getDHTData() {
  delay(1000);
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(20);

  pinMode(DHTPIN, INPUT_PULLUP);
  delayMicroseconds(50);

  count = 0;
  while (digitalRead(DHTPIN) == LOW && count++ < MaxCount) {}
  if (count >= MaxCount) return false;

  count = 0;
  while (digitalRead(DHTPIN) == HIGH && count++ < MaxCount) {}
  if (count >= MaxCount) return false;

  for (int i = 0; i < 80; i += 2) {
    count = 0;
    while (digitalRead(DHTPIN) == LOW && ++count) {}
    cycles[i] = count;

    count = 0;
    while (digitalRead(DHTPIN) == HIGH && ++count) {}
    cycles[i + 1] = count;
  }

  for (int i = 0; i < 40; ++i) {
    uint32_t lowCycles = cycles[2 * i];
    uint32_t highCycles = cycles[2 * i + 1];
    data[i / 8] <<= 1;
    if (highCycles > lowCycles) data[i / 8] |= 1;
  }

  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    Humidity = data[0] + data[1] * 0.1;
    Temperature = data[2] + (data[3] & 0x0f) * 0.1;
  }

  Serial.printf("Humidity = %.1f%%\nTemperature = %.1f°C\n", Humidity, Temperature);
  return (Temperature >= TempHigh);
}

bool getMoistureData() {
  digitalWrite(MuxSelectPin, 0);
  Moisture = analogRead(AnalogPin);
  Serial.printf("Moisture Level: %.2f%%\n%s\n",
                Moisture * 100.0 / 1024.0,
                (Moisture < Moisture_Wet) ? "Status: Soil is too wet" :
                (Moisture < Moisture_Dry) ? "Status: Soil moisture is perfect" :
                                           "Status: Soil is too dry - time to water!");
  return (Moisture >= Moisture_Dry);
}

bool getWaterData() {
  digitalWrite(MuxSelectPin, 1);
  Water = analogRead(AnalogPin);
  Serial.printf("Water Level: %.2f%%\nStatus: Water level is %s\n",
                Water * 100.0 / 1024.0,
                (Water < Water_Low) ? "LOW" : (Water < Water_High) ? "MEDIUM" : "HIGH");
  return (Water < Water_High);
}

void controlPump() {
  if (MasterControl) {
    if (Firebase.ready() && signupOk) {
      Firebase.RTDB.setString(&cloud, MASTERCONTROL_URL, "1");
      PumpStatus = Firebase.RTDB.getString(&cloud, PUMP_URL) ? cloud.stringData() : "";
    }

    ManualControl = (PumpStatus == "1");
    digitalWrite(PumpPin, (PumpStatus == "1" || Decision1 || Decision2) ? HIGH : LOW);
  } else {
    Firebase.RTDB.setString(&cloud, MASTERCONTROL_URL, "0");
    Firebase.RTDB.setString(&cloud, PUMP_URL, "0");
    digitalWrite(PumpPin, LOW);
    ManualControl = false;
    PumpStatus = "0";
  }
}

void updateFirebase() {
  Firebase.RTDB.setFloat(&cloud, TEMPERATURE_URL, Temperature);
  Firebase.RTDB.setFloat(&cloud, HUMIDITY_URL, Humidity);
  Firebase.RTDB.setInt(&cloud, WATER_URL, Water * 100 / 1024);
  Firebase.RTDB.setInt(&cloud, MOISTURE_URL, Moisture * 100 / 1024);
}

void updateScreen() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Temperature = " + String(Temperature, 1) + String("°C"));
  display.drawString(0, 12, "Humidity = " + String(Humidity, 1) + "%");
  display.drawString(0, 24, "Moisture = " + String(Moisture * 100 / 1024) + "%");
  display.drawString(0, 36, "Water = " + String(Water * 100 / 1024) + "%");
  ManualControl ? display.drawString(0, 48, "Manual Control: ON") : display.drawString(0, 48, "Manual Control: OFF");
  display.display();
}
