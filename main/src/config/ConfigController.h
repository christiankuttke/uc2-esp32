#pragma once
#include "Preferences.h"
#include "ArduinoJson.h"
#include "JsonKeys.h"
#include "../wifi/WifiConfig.h"
#include "esp_log.h"
#include "../motor/MotorPins.h"
#include "../motor/FocusMotor.h"
#include "../led/LedConfig.h"
#include "../analog/JoystickPins.h"
#include "../../ModuleConfig.h"

namespace Config
{
    void setup();
    bool resetPreferences();
    bool isFirstRun();
    void setWifiConfig(WifiConfig * config);
    WifiConfig * getWifiConfig();
    void setMotorPinConfig(MotorPins * pins[]);
    void getMotorPins(MotorPins * pins[]);
    void setLedPins(LedConfig * config);
    LedConfig * getLedPins();
    void setModuleConfig(ModuleConfig * pins);
    ModuleConfig * getModuleConfig();
    void setAnalogJoyStickPins(JoystickPins * pins);
    void getAnalogJoyStickPins(JoystickPins * pins);
    void setPsxMac(String mac);
    String getPsxMac();
    void setPsxControllerType(int type);
    int getPsxControllerType();
}
