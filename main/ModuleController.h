#pragma once
#include <map>
#include "Module.h"
#include "src/led/LedController.h"
#include "src/motor/FocusMotor.h"
#include "src/analog/AnalogJoystick.h"
#include "PinConfig.h"

namespace RestApi
{
    void getModules();
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
    void setup();
    void loop();
    Module *get(AvailableModules mod);
    DynamicJsonDocument get();
};

extern ModuleController moduleController;