#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>


#define TempHigh 30
#define Moisture_Dry 750
#define Water_High 600

using namespace std;


float random_number(const string& range) {
    int lowerBoundary = stoi(range.substr(0, range.find('-')));
    int upperBoundary = stoi(range.substr(range.find('-') + 1));
    return lowerBoundary + (upperBoundary - lowerBoundary) * static_cast<float>(rand()) / RAND_MAX;
}


int main() {
    bool* passedTest = new bool[128] {};
    ifstream file("output.csv");

    if (!file) {
        std::cout << "Error opening file." << endl;
        return 1;
    }

    string line;
    int TestCase = 0;
    getline(file, line);
    string WaterLevel, Firebase_ready, signupok, getString, SoilMoistureSensor,
        TemperatureSensor, userInput, MasterControlSignal, Decision1, Decision2, ManualControl,
        SlaveControlSignal, PumpStatus;
    int PassedTestCases = 0;
    srand(time(nullptr));
    while (getline(file, line)) {
        std::cout << "Test Case " << ++TestCase << endl;
        stringstream ss(line);
        string reading;
        int col = 0;
        while (getline(ss, reading, ',')) {
            col++;
            switch (col) {
            case 1: WaterLevel = reading; break;
            case 2: Firebase_ready = reading; break;
            case 3: signupok = reading; break;
            case 4: getString = reading; break;
            case 5: SoilMoistureSensor = reading; break;
            case 6: TemperatureSensor = reading; break;
            case 7: userInput = reading; break;
            case 8: MasterControlSignal = reading; break;
            case 9: Decision1 = reading; break;
            case 10: Decision2 = reading; break;
            case 11: ManualControl = reading; break;
            case 12: SlaveControlSignal = reading; break;
            case 13: PumpStatus = reading; break;

            }
        }
        std::cout << "WaterLevel: " + WaterLevel + "\n";                            // Input
        std::cout << "Firebase_ready: " + Firebase_ready + "\n";                    // Input
        std::cout << "signupok: " + signupok + "\n";                                // Input
        std::cout << "getString: " + getString + "\n";                              // Input
        std::cout << "SoilMoistureSensor: " + SoilMoistureSensor + "\n";            // Input
        std::cout << "TemperatureSensor: " + TemperatureSensor + "\n";              // Input
        std::cout << "userInput: " + userInput + "\n";                              // Input
        std::cout << "MasterControlSignal: " + MasterControlSignal + "\n";          // Depends on WaterLevel                                DONE
        std::cout << "Decision1: " + Decision1 + "\n";                              // Depends on Moisture Level                            DONE
        std::cout << "Decision2: " + Decision2 + "\n";                              // Depends on Temperature                               DONE
        std::cout << "ManualControl: " + ManualControl + "\n";                      // Depends on Firebase_ready, signupok,  getString      DONE
        std::cout << "SlaveControlSignal: " + SlaveControlSignal + "\n";            // Depends on Decision1, Decision2, ManualControl       DONE
        cout << "PumpStatus: " + PumpStatus + "\n";                                 // Depend on SlaveControlSignal and MasterControlSignal DONE


        float _Temperature = random_number(TemperatureSensor);
        bool _Decision2 = (_Temperature >= TempHigh);
        int _Moisture = (int)random_number(SoilMoistureSensor);
        bool _Decision1 = (_Moisture >= Moisture_Dry);
        int _Water = (int)random_number(WaterLevel);
        bool _MasterControl = (_Water < Water_High);
        string _Pump, _PumpStatus, _SlaveControlSignal;
        bool _ManualControl;

        if (_MasterControl) { // if water level is not high
            if (stoi(Firebase_ready) && stoi(signupok)) {
                _Pump = stoi(getString) ? userInput : "";
            }
            _ManualControl = (_Pump == "1");
            _PumpStatus = (_Pump == "1" || _Decision1 || _Decision2) ? "1" : "0"; // Manual Control
        }
        else { // if water level is high, Close the pump if it is open
            _ManualControl = false;
            _PumpStatus = "0";
        }
        _SlaveControlSignal = (_Pump == "1" || _Decision1 || _Decision2) ? "1" : "0"; // Manual Control


        if (stoi(MasterControlSignal) == _MasterControl && stoi(Decision1) == _Decision1 && stoi(Decision2) == _Decision2
            && stoi(ManualControl) == _ManualControl && _PumpStatus == PumpStatus && _SlaveControlSignal == SlaveControlSignal) {
            passedTest[PassedTestCases++] = 1;
        }
        else cout << "<><><><><><><><><><><><><><><><><><><><><><><><>\n";
        cout << "*************************************************\n";

        std::cout << "_MasterControlSignal: " + to_string(_MasterControl) + "\n";          // Depends on WaterLevel                                DONE
        std::cout << "_Decision1: " + to_string(_Decision1) + "\n";                              // Depends on Moisture Level                            DONE
        std::cout << "_Decision2: " + to_string(_Decision2) + "\n";                              // Depends on Temperature                               DONE
        std::cout << "_ManualControl: " + to_string(_ManualControl) + "\n";                      // Depends on Firebase_ready, signupok,  getString
        std::cout << "SlaveControlSignal: " + _SlaveControlSignal + "\n";            // Depends on Decision1, Decision2, ManualControl
        cout << "_PumpStatus: " + _PumpStatus + "\n";                                 // Depend on SlaveControlSignal and MasterControlSignal
        cout << "Passed Test Cases = " + to_string(PassedTestCases) + "\n";
        std::cout << endl;
    }
    cout << "Passed Test Cases = " + to_string(PassedTestCases) + "\n\n\n";
    int x = 1;
    for (int i = 0; i < 128; i++) {
        if (passedTest[i]) cout << "Test Case " + to_string(i + 1) + ": " << "\033[32m" << "Passed" "\033[0m" << "\t";
        else cout << "Test Case " + to_string(i + 1) + ": " << "\033[31m" << "Failed" "\033[0m" << "\t";
        if (x++ % 4 == 0) cout << endl;
    }
    cout << "\n\t\t\t\tPassed Test Cases = " << "\033[33m" + to_string(PassedTestCases) + "\033[0m" + "\n";
    file.close();
    cout << "\n\nThe end";
    return 0;
}