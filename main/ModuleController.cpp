#include "ModuleController.h"

namespace RestApi
{
    void getModules()
    {
        serialize(moduleController.get());
    }
};

void ModuleController::setup()
{
    for (auto &x : modules)
    {
        if (x.second != nullptr)
        {
            delete x.second;
        }
    }
    modules.clear();
    if (pinConfig.LED_PIN > 0)
    {
        modules.insert(std::make_pair(AvailableModules::led, dynamic_cast<Module *>(new LedController())));
        log_i("add led");
    }
    if (pinConfig.MOTOR_ENABLE > 0)
    {
        modules.insert(std::make_pair(AvailableModules::motor, dynamic_cast<Module *>(new FocusMotor())));
        log_i("add motor");
    }
    if (pinConfig.ANLOG_JOYSTICK_X > 0 || pinConfig.ANLOG_JOYSTICK_Y > 0)
    {
        modules.insert(std::make_pair(AvailableModules::analogJoystick, dynamic_cast<Module *>(new AnalogJoystick())));
        log_i("add scanner");
    }
    
    for (auto &x : modules)
    {
        if (x.second != nullptr)
            x.second->setup();
    }
}

void ModuleController::loop()
{
    for (auto &x : modules)
    {
        if (x.second != nullptr)
        {
             x.second->loop();
        }
    }
}

Module *ModuleController::get(AvailableModules mod)
{
    return modules[mod];
}

DynamicJsonDocument ModuleController::get()
{
    DynamicJsonDocument doc(4096);
    doc[key_modules][keyLed] = pinConfig.LED_PIN > 0;
    doc[key_modules][key_motor] = pinConfig.MOTOR_ENABLE > 0;
    doc[key_modules][key_joy] = pinConfig.ANLOG_JOYSTICK_X > 0 || pinConfig.ANLOG_JOYSTICK_Y > 0;
    return doc;
}

ModuleController moduleController;