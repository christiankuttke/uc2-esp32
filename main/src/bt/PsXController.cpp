#include "PsXController.h"

void PsXController::setup(String mac, int type)
{
	// select the type of wifi controller - either PS3 or PS4
	if (type == 1)
		psx = new PSController(nullptr, PSController::kPS3);
	else if (type == 2)
		psx = new PSController(nullptr, PSController::kPS4);
	psx->startListening(mac);
}

void PsXController::loop()
{
	// This is called in every MCU cycle

	// This ensures the mapping of physical inputs (e.g. buttons) to logical outputs (e.g. motor)

	// log_i("psx connected:%i",psx.isConnected());
	if (psx != nullptr && psx->isConnected())
	{
		if (moduleController.get(AvailableModules::led) != nullptr)
		{
			// switch LED on/off on cross/circle button press
			LedController *led = (LedController *)moduleController.get(AvailableModules::led);
			if (psx->event.button_down.cross)
			{
				log_i("Turn on LED ");
				IS_PS_CONTROLER_LEDARRAY = !led->TurnedOn();
				led->set_all(255, 255, 255);
			}
			if (psx->event.button_down.circle)
			{
				log_i("Turn off LED ");
				IS_PS_CONTROLER_LEDARRAY = !led->TurnedOn();
				led->set_all(0, 0, 0);
			}
		}

		// MOTORS
		if (moduleController.get(AvailableModules::motor) != nullptr)
		{
			/* code */
			FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
			// Z-Direction
			if (abs(psx->state.analog.stick.ly) > offset_val)
			{
				stick_ly = psx->state.analog.stick.ly;
				stick_ly = stick_ly - sgn(stick_ly) * offset_val;
				if (abs(stick_ly) > 50)
					stick_ly = 2 * stick_ly; // add more speed above threshold
				motor->data[Stepper::Z]->speed = stick_ly * global_speed;
				Serial.println(motor->data[Stepper::Z]->speed);
				motor->data[Stepper::Z]->isforever = true;
				joystick_drive_Z = true;
				if (motor->data[Stepper::Z]->stopped)
				{
					motor->startStepper(Stepper::Z);
				}
			}
			else if (motor->data[Stepper::Z]->speed != 0 && joystick_drive_Z)
			{
				motor->stopStepper(Stepper::Z);
				joystick_drive_Z = false;
				log_i("stop stepper z");
			}

			// X-Direction
			if ((abs(psx->state.analog.stick.rx) > offset_val))
			{
				// move_x
				stick_rx = psx->state.analog.stick.rx;
				stick_rx = stick_rx - sgn(stick_rx) * offset_val;
				motor->data[Stepper::X]->speed = stick_rx * global_speed;
				if (abs(stick_rx) > 50)
					stick_rx = 2 * stick_rx; // add more speed above threshold
				motor->data[Stepper::X]->isforever = true;
				joystick_drive_X = true;
				if (motor->data[Stepper::X]->stopped)
				{
					motor->startStepper(Stepper::X);
				}
			}
			else if (motor->data[Stepper::X]->speed != 0 && joystick_drive_X)
			{
				motor->stopStepper(Stepper::X);
				joystick_drive_X = false;
				log_i("stop stepper x");
			}

			// Y-direction
			if ((abs(psx->state.analog.stick.ry) > offset_val))
			{
				stick_ry = psx->state.analog.stick.ry;
				stick_ry = stick_ry - sgn(stick_ry) * offset_val;
				motor->data[Stepper::Y]->speed = stick_ry * global_speed;
				if (abs(stick_ry) > 50)
					stick_ry = 2 * stick_ry; // add more speed above threshold
				motor->data[Stepper::Y]->isforever = true;
				joystick_drive_Y = true;
				if (motor->data[Stepper::Y]->stopped)
				{
					motor->startStepper(Stepper::Y);
				}
			}
			else if (motor->data[Stepper::Y]->speed != 0 && joystick_drive_Y)
			{
				motor->stopStepper(Stepper::Y);
				joystick_drive_Y = false;
				log_i("stop stepper y");
			}
		}
	}
}