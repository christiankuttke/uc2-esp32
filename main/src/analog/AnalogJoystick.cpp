#include "AnalogJoystick.h"

namespace RestApi
{
    void AnalogJoystick_get()
    {
        serialize(moduleController.get(AvailableModules::analogJoystick)->get(deserialize()));
    }
};

AnalogJoystick::AnalogJoystick(/* args */){};
AnalogJoystick::~AnalogJoystick(){};
void AnalogJoystick::setup()
{
    pinMode(pinConfig.ANLOG_JOYSTICK_X, INPUT);
    pinMode(pinConfig.ANLOG_JOYSTICK_Y, INPUT);
}
int AnalogJoystick::act(DynamicJsonDocument jsonDocument) { return 1;}

DynamicJsonDocument AnalogJoystick::get(DynamicJsonDocument  doc) {
    doc.clear();
    doc[key_joy][key_joiypinX] = pinConfig.ANLOG_JOYSTICK_X;
    doc[key_joy][key_joiypinY] = pinConfig.ANLOG_JOYSTICK_Y;
    return doc;
}

void AnalogJoystick::loop()
{

    // log_i("X: %i Y: %i", x, y);
    if (moduleController.get(AvailableModules::motor) != nullptr)
    {
        FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
        if (pinConfig.ANLOG_JOYSTICK_X > 0)
        {
            int x = analogRead(pinConfig.ANLOG_JOYSTICK_X) - 4096 / 2;
            if (x >= 200 || x <= -200)
            {
                motor->data[Stepper::X]->speed = x;
                motor->data[Stepper::X]->isforever = true;
                joystick_drive_X = true;
                if (motor->data[Stepper::X]->stopped)
                {
                    motor->startStepper(Stepper::X);
                }
            }
            else if ((x <= 200 || x >= -200) && !motor->data[Stepper::X]->stopped && joystick_drive_X)
            {
                log_i("stop motor X");
                motor->stopStepper(Stepper::X);
                joystick_drive_X = false;
            }
        }
        if (pinConfig.ANLOG_JOYSTICK_Y > 0)
        {
            int y = analogRead(pinConfig.ANLOG_JOYSTICK_Y) - 4096 / 2;
            if (y >= 200 || y <= -200)
            {
                motor->data[Stepper::Y]->speed = y;
                motor->data[Stepper::Y]->isforever = true;
                joystick_drive_Y = true;
                if (motor->data[Stepper::Y]->stopped)
                    motor->startStepper(Stepper::Y);
            }
            else if ((y <= 200 || y >= -200) && !motor->data[Stepper::Y]->stopped && joystick_drive_Y)
            {
                log_i("stop motor Y");
                motor->stopStepper(Stepper::Y);
                joystick_drive_Y = false;
            }
        }
    }
}