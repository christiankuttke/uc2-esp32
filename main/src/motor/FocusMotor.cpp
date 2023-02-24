#include "../../config.h"
#include "FocusMotor.h"

namespace RestApi
{
	void FocusMotor_act()
	{
		serialize(moduleController.get(AvailableModules::motor)->act(deserialize()));
	}

	void FocusMotor_get()
	{
		serialize(moduleController.get(AvailableModules::motor)->get(deserialize()));
	}
}

void sendUpdateToClients(void *p)
{
	for (;;)
	{
		int arraypos = 0;
		FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
		for (int i = 0; i < motor->steppers.size(); i++)
		{
			if (!motor->data[i]->stopped)
			{
				motor->sendMotorPos(i, arraypos);
			}
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}


void driveMotorXLoop(void *pvParameter)
{
	FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
	motor->driveMotorLoop(Stepper::X);
}
void driveMotorYLoop(void *pvParameter)
{
	FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
	motor->driveMotorLoop(Stepper::Y);
}
void driveMotorZLoop(void *pvParameter)
{
	FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
	motor->driveMotorLoop(Stepper::Z);
}
void driveMotorALoop(void *pvParameter)
{
	FocusMotor *motor = (FocusMotor *)moduleController.get(AvailableModules::motor);
	motor->driveMotorLoop(Stepper::A);
}

FocusMotor::FocusMotor() : Module() { log_i("ctor"); }
FocusMotor::~FocusMotor() { log_i("~ctor"); }

int FocusMotor::act(DynamicJsonDocument doc)
{
	if (doc.containsKey(key_motor))
	{
		if (doc[key_motor].containsKey(key_steppers))
		{
			for (int i = 0; i < doc[key_motor][key_steppers].size(); i++)
			{
				Stepper s = static_cast<Stepper>(doc[key_motor][key_steppers][i][key_stepperid]);

				if (doc[key_motor][key_steppers][i].containsKey(key_speed))
					data[s]->speed = doc[key_motor][key_steppers][i][key_speed];

				if (doc[key_motor][key_steppers][i].containsKey(key_position))
					data[s]->targetPosition = doc[key_motor][key_steppers][i][key_position];

				if (doc[key_motor][key_steppers][i].containsKey(key_isforever))
					data[s]->isforever = doc[key_motor][key_steppers][i][key_isforever];

				if (doc[key_motor][key_steppers][i].containsKey(key_isabs))
					data[s]->absolutePosition = doc[key_motor][key_steppers][i][key_isabs];

				if (doc[key_motor][key_steppers][i].containsKey(key_isaccel))
					data[s]->isaccelerated = doc[key_motor][key_steppers][i][key_isaccel];

				log_i("data %i speed:%i forever:%i", s, data[s]->speed, data[s]->isforever);
				if (doc[key_motor][key_steppers][i].containsKey(key_isstop))
					stopStepper(s);
				else
				{
					startStepper(s);
				}
			}
		}
	}
	return 1;
}

void FocusMotor::startStepper(int i)
{
	log_i("start stepper:%i isforver:%i", i, data[i]->isforever);
	enableEnablePin(i);
	data[i]->stopped = false;
}

DynamicJsonDocument FocusMotor::set(DynamicJsonDocument doc)
{
	log_i("set motor");

	// only enable/disable motors
	if (doc.containsKey(key_isen))
	{
		for (int i = 0; i < steppers.size(); i++)
		{
			// turn on/off holding current
			if (doc[key_isen]){
				steppers[i]->enableOutputs();
			}
			else{
				steppers[i]->disableOutputs();
			}

		}
	}
	// return all information
	if (doc.containsKey(key_motor))
	{
		if (doc[key_motor].containsKey(key_steppers))
		{

			for (int i = 0; i < doc[key_motor][key_steppers].size(); i++)
			{
				Stepper s = static_cast<Stepper>(doc[key_motor][key_steppers][i][key_stepperid]);
				pins[s]->DIR = doc[key_motor][key_steppers][i][key_dirpin];
				pins[s]->STEP = doc[key_motor][key_steppers][i][key_steppin];
				pins[s]->ENABLE = doc[key_motor][key_steppers][i][key_enablepin];
				pins[s]->direction_inverted = doc[key_motor][key_steppers][i][key_dirpin_inverted];
				pins[s]->step_inverted = doc[key_motor][key_steppers][i][key_steppin_inverted];
				pins[s]->enable_inverted = doc[key_motor][key_steppers][i][key_enablepin_inverted];
				pins[s]->current_position = doc[key_motor][key_steppers][i][key_position];
				pins[s]->max_position = doc[key_motor][key_steppers][i][key_max_position];
				pins[s]->min_position = doc[key_motor][key_steppers][i][key_min_position];
			}
			Config::setMotorPinConfig(pins);
			setup();
		}
	}
	return 1;
}


DynamicJsonDocument FocusMotor::get(DynamicJsonDocument docin)
{
	log_i("get motor");
	DynamicJsonDocument doc(4096); //StaticJsonDocument<1024> doc; // create return doc
	// only return the position of the stepper
	if (docin.containsKey(key_position))
	{
		docin.clear();
		for (int i = 0; i < steppers.size(); i++)
		{
			// update position and push it to the json
			pins[i]->current_position = steppers[i]->currentPosition();
			doc[key_motor][key_steppers][i][key_stepperid] = i;
			doc[key_motor][key_steppers][i][key_position] = pins[i]->current_position;
		}
		return doc;
	}

	// only return if motor is still busy
	if (docin.containsKey(key_stopped))
	{
		docin.clear();
		for (int i = 0; i < steppers.size(); i++)
		{
			// update position and push it to the json
			doc[key_motor][key_steppers][i][key_stopped] = !data[i]->stopped;
		}
		return doc;
	}

	// return the whole config
	docin.clear();
	for (int i = 0; i < steppers.size(); i++)
	{
		ob[key_steppers][i][key_stepperid] = i;
		ob[key_steppers][i][key_position] = data[i]->currentPosition;
	}
	return ob;
}

void FocusMotor::setup()
{

	steppers[Stepper::A] = new AccelStepper(AccelStepper::DRIVER, pinConfig.MOTOR_A_STEP, pinConfig.MOTOR_A_DIR);
	steppers[Stepper::X] = new AccelStepper(AccelStepper::DRIVER, pinConfig.MOTOR_X_STEP, pinConfig.MOTOR_X_DIR);
	steppers[Stepper::Y] = new AccelStepper(AccelStepper::DRIVER, pinConfig.MOTOR_Y_STEP, pinConfig.MOTOR_Y_DIR);
	steppers[Stepper::Z] = new AccelStepper(AccelStepper::DRIVER, pinConfig.MOTOR_Z_STEP, pinConfig.MOTOR_Z_DIR);

	for (int i = 0; i < steppers.size(); i++)
	{
		data[i] = new MotorData();
	}

	/*
	   Motor related settings
	*/
	Serial.println("Setting Up Motors");
	Serial.println("Setting Up Motor A,X,Y,Z");
	enableEnablePin(0);
	for (int i = 0; i < steppers.size(); i++)
	{
		steppers[i]->setMaxSpeed(MAX_VELOCITY_A);
		steppers[i]->setAcceleration(MAX_ACCELERATION_A);
		steppers[i]->enableOutputs();
		steppers[i]->runToNewPosition(-10);
		steppers[i]->runToNewPosition(10);
		steppers[i]->setCurrentPosition(pins[i]->current_position);
		//steppers[i]->enableOutputs();
		// steppers[i]->disableOutputs();
	}
	disableEnablePin(0);
	if(pinConfig.MOTOR_A_DIR > 0)
		xTaskCreate(&driveMotorALoop, "motor_task_A", 1024, NULL, 5, NULL);
	if(pinConfig.MOTOR_X_DIR > 0)
		xTaskCreate(&driveMotorXLoop, "motor_task_X", 1024, NULL, 5, NULL);
	if(pinConfig.MOTOR_Y_DIR > 0)
		xTaskCreate(&driveMotorYLoop, "motor_task_Y", 1024, NULL, 5, NULL);
	if(pinConfig.MOTOR_Z_DIR > 0)
		xTaskCreate(&driveMotorZLoop, "motor_task_Z", 1024, NULL, 5, NULL);
	xTaskCreate(&sendUpdateToClients, "motor_websocket_task", 2048, NULL, 5, NULL);
}

void FocusMotor::driveMotorLoop(int stepperid)
{
	AccelStepper *s = steppers[stepperid];
	MotorData *d = data[stepperid];
	for (;;)
	{
		s->setMaxSpeed(d->maxspeed);
		if (d->isforever)
		{
			s->setSpeed(d->speed);
			s->runSpeed();
		}
		else
		{

			if (d->absolutePosition)
			{
				// absolute position coordinates
				s->moveTo(d->targetPosition);
			}
			else
			{
				// relative position coordinates
				s->move(d->targetPosition);
			}
			// checks if a stepper is still running
			if (s->distanceToGo() == 0 && !d->stopped)
			{
				log_i("stop stepper:%i", stepperid);
				// if not turn it off
				stopStepper(stepperid);
			}
		}
		d->currentPosition = s->currentPosition();
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void FocusMotor::loop()
{

	// Motor logic runs in a background loop => task

	// check if we want to disable motors after timeout
	bool timeoutReached = true;
	for (int i = 0; i < steppers.size(); i++)
	{
		timeoutReached &= ((millis() - data[i]->timeLastActive) > data[i]->timeoutDisable);
		if(timeoutReached)
		{ 	// only if all of the motors have reached their timeout, disable the enable pin
			disableEnablePin(-1);
		}
	}
}

void FocusMotor::sendMotorPos(int i, int arraypos)
{
	if(moduleController.get(AvailableModules::wifi) != nullptr)
	{
		WifiController * w = (WifiController*)moduleController.get(AvailableModules::wifi);
		DynamicJsonDocument doc(4096);
		doc[key_steppers][arraypos][key_stepperid] = i;
		doc[key_steppers][arraypos][key_position] = data[i]->currentPosition;
		arraypos++;
		w->sendJsonWebSocketMsg(doc);
	}
}

void FocusMotor::stopAllDrives()
{
	// Immediately stop the motor
	for (int i = 0; i < 4; i++)
	{
		stopStepper(i);
	}
}

void FocusMotor::stopStepper(int i)
{
	steppers[i]->stop();
	data[i]->isforever = false;
	data[i]->speed = 0;
	data[i]->currentPosition = steppers[i]->currentPosition();
	data[i]->stopped = true;
	disableEnablePin(i);
}

void FocusMotor::startAllDrives()
{
	for (int i = 0; i < steppers.size(); i++)
	{
		startStepper(i);
	}
}

void FocusMotor::disableEnablePin(int i)
{

	if (data[Stepper::A]->stopped &&
		data[Stepper::X]->stopped &&
		data[Stepper::Y]->stopped &&
		data[Stepper::Z]->stopped &&
		power_enable)
	{
		pinMode(pinConfig.MOTOR_ENABLE, OUTPUT);
		digitalWrite(pinConfig.MOTOR_ENABLE, LOW ^ pinConfig.MOTOR_ENABLE_INVERTED);
		power_enable = false;
	}
}

void FocusMotor::enableEnablePin(int i)
{
	if (!power_enable)
	{
		pinMode(pinConfig.MOTOR_ENABLE, OUTPUT);
		digitalWrite(pinConfig.MOTOR_ENABLE, HIGH ^ pinConfig.MOTOR_ENABLE_INVERTED);
		power_enable = true;
	}
}
