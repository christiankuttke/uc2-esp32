#include "ModuleController.h"

namespace RestApi
{
    void getModules()
    {
        serialize(moduleController.get());
    }
    void setModules()
    {
        serialize(moduleController.set(deserialize()));
    }
};

void ModuleController::setup()
{

    // moduleConfig.led = true;
    // moduleConfig.motor = true;
    for (auto &x : modules)
    {
        if (x.second != nullptr)
        {
            delete x.second;
            //x.second = nullptr;
        }
    }
    modules.clear();
    moduleConfig = Config::getModuleConfig();
    if (moduleConfig->led)
    {
        modules.insert(std::make_pair(AvailableModules::led, dynamic_cast<Module *>(new LedController())));
        log_i("add led");
    }
    if (moduleConfig->motor)
    {
        modules.insert(std::make_pair(AvailableModules::motor, dynamic_cast<Module *>(new FocusMotor())));
        log_i("add motor");
    }
    if (moduleConfig->analogJoystick)
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
    doc[key_modules][keyLed] = moduleConfig->led;
    doc[key_modules][key_motor] = moduleConfig->motor;
    doc[key_modules][key_joy] = moduleConfig->analogJoystick;
    return doc;
}

int ModuleController::set(DynamicJsonDocument j)
{
    if (j.containsKey(key_modules))
    {
        if (j[key_modules].containsKey(keyLed))
            moduleConfig->led = j[key_modules][keyLed];
        if (j[key_modules].containsKey(key_motor))
            moduleConfig->motor = j[key_modules][key_motor];
        if (j[key_modules].containsKey(key_joy))
            moduleConfig->analogJoystick = j[key_modules][key_joy];
        Config::setModuleConfig(moduleConfig);
        setup();
        WifiController::restartWebServer();
    }
    return 1;
}

ModuleController moduleController;