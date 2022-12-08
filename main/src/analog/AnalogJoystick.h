#pragma once
#include <Arduino.h>
#include "../../Module.h"
#include "../../ModuleController.h"
#include "../config/ConfigController.h"
#include "../../PinConfig.h"

namespace RestApi
{
	void AnalogJoystick_get();
};



class AnalogJoystick : public Module
{

    private:
    bool joystick_drive_X = false;
    bool joystick_drive_Y = false;

    public:
    AnalogJoystick();
	virtual ~AnalogJoystick();
    void setup() override;
    int act(DynamicJsonDocument  jsonDocument) override;
    DynamicJsonDocument get(DynamicJsonDocument  jsonDocument) override;
    void loop() override;

};