#pragma once
#include <map>
#include "Module.h"
#include "ModuleConfig.h"
#include "src/led/LedController.h"
#include "src/motor/FocusMotor.h"
#include "src/analog/AnalogJoystick.h"

namespace RestApi
{
    void getModules();
    void setModules();
};

enum class AvailableModules
{
    led,
    motor,
    analogJoystick,
};

class ModuleController
{
private:
    std::map<AvailableModules, Module *> modules;
   
public:
    ModuleConfig * moduleConfig;
    void setup();
    void loop();
    Module *get(AvailableModules mod);
    DynamicJsonDocument get();
    int set(DynamicJsonDocument j);
};

extern ModuleController moduleController;