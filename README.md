# IoT Smart Plant Monitoring and Irrigation System

An ESP8266-based smart plant monitoring and irrigation system that measures temperature, humidity, soil moisture, and water level, uploads readings to Firebase Realtime Database, displays live values on an OLED screen, and controls a water pump automatically or manually through a mobile application.

![System demonstration with mobile app](public/images/projects/iot-smart-plant-monitoring-irrigation-system/demo-system-and-mobile-app.png)

## Project Links

- [Watch the system demonstration on YouTube](https://youtu.be/aI00_OWCgdE)
- [View and download the CAD assembly on GrabCAD](https://grabcad.com/library/iot-smart-plant-monitoring-and-irrigation-system-1)

## Main Features

- ESP8266 NodeMCU firmware for sensor reading, cloud updates, display output, and pump control.
- DHT11 temperature and humidity reading implemented directly through timing pulses.
- Soil-moisture and water-level readings through the ESP8266 analog input using a 74HC4051 multiplexer.
- Firebase Realtime Database integration for remote monitoring and manual pump control.
- MIT App Inventor mobile application for live sensor values and manual irrigation control.
- OLED display for local monitoring.
- Relay-driven water-pump actuation.
- Fritzing circuit design, SolidWorks enclosure/base design, and laser-cut DXF file.
- Test-case generation and C++ verification for the pump-control decision logic.

## System Overview

The system collects environmental and plant-state readings from multiple sensors. The ESP8266 uses these values to decide whether the plant needs irrigation, while a water-level sensor acts as a master safety condition to prevent the pump from operating when the water level exceeds the threshold.

![Fritzing circuit diagram](public/images/projects/iot-smart-plant-monitoring-irrigation-system/fritzing-breadboard-diagram.png)

The mobile application reads values from Firebase and can also write a manual pump-control signal. The firmware combines this manual command with the automatic control decisions.

A complete demonstration of the working system is available on [YouTube](https://youtu.be/aI00_OWCgdE).

![Mobile app manual control off](public/images/projects/iot-smart-plant-monitoring-irrigation-system/mobile-app-manual-off.jpg)

## Hardware and Mechanical Design

The prototype combines the electronics, pump, reservoir, and plant into a single physical setup. The project includes SolidWorks model files, laser-cut design files, and prototype photographs.

The complete CAD assembly and laser-cut base design are also available on [GrabCAD](https://grabcad.com/library/iot-smart-plant-monitoring-and-irrigation-system-1).

![SolidWorks assembly view](public/images/projects/iot-smart-plant-monitoring-irrigation-system/solidworks-assembly-inclined-view.png)

## Control Logic

The pump-control decision is based on four main conditions:

| Signal | Meaning |
|---|---|
| `MasterControl` | Water-level safety condition. The pump is disabled when water is above the threshold. |
| `Decision1` | Soil is dry enough to require watering. |
| `Decision2` | Temperature is high enough to trigger watering. |
| `ManualControl` | User requested pump operation through Firebase/mobile app. |

The pump is allowed to run only when the master water-level condition is satisfied. Under that condition, it turns on if the user requests manual irrigation, the soil is dry, or the temperature is high.

## Why a Multiplexer Was Used

The ESP8266 has only one analog input. The project uses a 74HC4051 multiplexer to connect multiple analog sensors to the same input. This avoids adding a second microcontroller and keeps the prototype smaller and less complex.

The design comparison considered using an Arduino UNO as an analog-reading companion board, but the direct ESP8266 + multiplexer path was selected because it is faster and more compact for this prototype.

## Firmware Structure

```text
firmware/
└── SmartPlantMonitoring/
    ├── SmartPlantMonitoring.ino
    ├── helper_functions.h
    ├── helper_functions.cpp
    └── credentials.example.h
```

Create your local credentials file before uploading the firmware:

```bash
cp firmware/SmartPlantMonitoring/credentials.example.h firmware/SmartPlantMonitoring/credentials.h
```

Then edit `credentials.h` with your Wi-Fi and Firebase project values.

## Required Arduino Libraries

Install the following libraries in the Arduino IDE:

- ESP8266 board support package
- Firebase ESP Client
- ESP8266WiFi
- SSD1306Wire / ESP8266 OLED driver
- Wire

The firmware uses an internal DHT11 timing implementation, so the final version does not require the earlier DHT11 library test sketch.

## Test-Case Verification

The `tests/` folder contains a Python generator and C++ verification program for the pump-control decision logic.

```text
tests/
├── generate_test_cases.py
├── output.csv
└── verify_test_cases.cpp
```

The generator enumerates combinations of water-level state, Firebase readiness, sign-up state, manual command state, soil-moisture state, and temperature state. The verifier checks the expected master-control, decision, manual-control, slave-control, and pump-status outputs.

## Repository Contents

The repository contains the cleaned firmware, test utilities, mobile APK and screenshots, Fritzing source, SolidWorks and laser-cut files, prototype media, project images, and selected technical references. The report and presentation are stored separately on Google Drive and are linked from the portfolio rather than committed to GitHub.

## Limitations

- The mobile app source `.aia` file in the original archive was corrupted, so the organized package keeps the APK and screenshots instead.
- The prototype is a single-plant demonstrator rather than a scaled agricultural deployment.
- The pump-control thresholds are fixed constants in firmware.
- The system depends on Firebase availability for remote monitoring and manual control.
- The mobile application was built as a project-specific interface rather than a general-purpose commercial app.

## Course Context

Developed for the Electric Machines course at the University of Science and Technology, Zewail City, during Spring 2023.
