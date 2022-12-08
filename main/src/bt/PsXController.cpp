#include "PsXController.h"

void PsXController::setup(String mac, int type)
{
	if(type == 1)
		psx = new PSController(nullptr, PSController::kPS3);
	else if(type == 2)
		psx = new PSController(nullptr, PSController::kPS4);
    psx->startListening(mac);
}

void PsXController::loop()
{
	//log_i("psx connected:%i",psx.isConnected());
    if (psx != nullptr && psx->isConnected())
    {
        if (moduleController.get(AvailableModules::led) != nullptr)
        {
            LedController *led = (LedController *)moduleController.get(AvailableModules::led);
            if (psx->event.button_down.cross)
            {
                IS_PS_CONTROLER_LEDARRAY = !led->TurnedOn();
                led->set_all(255 * IS_PS_CONTROLER_LEDARRAY, 255 * IS_PS_CONTROLER_LEDARRAY, 255 * IS_PS_CONTROLER_LEDARRAY);
                delay(1000); // Debounce?
            }
            if (psx->event.button_down.circle)
            {
                IS_PS_CONTROLER_LEDARRAY = !led->TurnedOn();
                led->set_center(255 * IS_PS_CONTROLER_LEDARRAY, 255 * IS_PS_CONTROLER_LEDARRAY, 255 * IS_PS_CONTROLER_LEDARRAY);
                delay(1000); // Debounce?
            }
        }

        if (moduleController.get(AvailableModules::motor) != nullptr)
		{
			/* code */
			FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
			// Y-Direction
			if (abs(psx->state.analog.stick.ly) > offset_val)
			{
				// move_z
				stick_ly = psx->state.analog.stick.ly;
				stick_ly = stick_ly - sgn(stick_ly) * offset_val;
				motor->data[Stepper::Y]->speed = stick_ly * 5 * global_speed;
				motor->data[Stepper::Y]->isforever = true;
				if (motor->data[Stepper::Y]->stopped)
                {
                    motor->startStepper(Stepper::Y);
                }
			}
			else if (motor->data[Stepper::Y]->speed != 0)
			{
				motor->stopStepper(Stepper::Y);
			}

			// Z-Direction
			if ((abs(psx->state.analog.stick.rx) > offset_val))
			{
				// move_x
				stick_rx = psx->state.analog.stick.rx;
				stick_rx = stick_rx - sgn(stick_rx) * offset_val;
				motor->data[Stepper::Z]->speed = stick_rx * 5 * global_speed;
				motor->data[Stepper::Z]->isforever = true;
				if (motor->data[Stepper::Z]->stopped)
                {
                    motor->startStepper(Stepper::Z);
                }
			}
			else if (motor->data[Stepper::Z]->speed != 0)
			{
				motor->stopStepper(Stepper::Z);
			}
			// X-direction
			if ((abs(psx->state.analog.stick.ry) > offset_val))
			{
				// move_y
				stick_ry = psx->state.analog.stick.ry;
				stick_ry = stick_ry - sgn(stick_ry) * offset_val;
				motor->data[Stepper::X]->speed = stick_ry * 5 * global_speed;
				motor->data[Stepper::X]->isforever = true;
				if (motor->data[Stepper::X]->stopped)
                {
                    motor->startStepper(Stepper::X);
                }
			}
			else if (motor->data[Stepper::X]->speed != 0)
			{
				motor->stopStepper(Stepper::X);
			}
		}
    }
}