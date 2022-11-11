#include "ConfigController.h"
#include "../motor/MotorPins.h"

namespace RestApi
{
	void Config_act()
	{
		deserialize();
		moduleController.get(AvailableModules::config)->act();
		serialize();
	}

	void Config_get()
	{
		deserialize();
		moduleController.get(AvailableModules::config)->get();
		serialize();
	}

	void Config_set()
	{
		deserialize();
		moduleController.get(AvailableModules::config)->set();
		serialize();
	}

}

ConfigController::ConfigController() : Module() { log_i("ctor"); }
ConfigController::~ConfigController() { log_i("~ctor"); }

void ConfigController::act()
{
	log_i("config act");
}

void ConfigController::set()
{
	log_i("config set");
	
	DynamicJsonDocument *doc = WifiController::getJDoc();
	serializeJsonPretty((*doc), Serial);

/*
	// attaching Motor settings
	for (int i = 0; i < (*doc)[key_motor][key_steppers].size(); i++)
	{
		Stepper s = static_cast<Stepper>((*doc)[key_motor][key_steppers][i][key_steppinperid]);
		pins[s]->DIR = (*doc)[key_motor][key_steppers][i][key_dirpin];
		pins[s]->STEP = (*doc)[key_motor][key_steppers][i][key_steppin];
		pins[s]->ENABLE = (*doc)[key_motor][key_steppers][i][key_enablepin];
		pins[s]->direction_inverted = (*doc)[key_motor][key_steppers][i][key_dirpin_inverted];
		pins[s]->step_inverted = (*doc)[key_motor][key_steppers][i][key_steppin_inverted];
		pins[s]->enable_inverted = (*doc)[key_motor][key_steppers][i][key_enablepin_inverted];
		pins[s]->min_position = (*doc)[key_motor][key_steppers][i][key_min_position];
		pins[s]->max_position = (*doc)[key_motor][key_steppers][i][key_max_position];
		pins[s]->current_position = (*doc)[key_motor][key_steppers][i][key_position];
	}
	Config::setMotorPinConfig(pins);
	FocusMotor::setup();
	*/
	// attaching LED settings

	// attaching XX settings 
	
	doc->clear();

}
void ConfigController::get()
{
}
void ConfigController::setup()
{
}
void ConfigController::loop()
{
}

namespace Config
{

	Preferences preferences;

	const char *prefNamespace = "UC2";

	void setWifiConfig(WifiConfig *config)
	{
		preferences.begin(prefNamespace, false);
		preferences.putBytes(keyWifiSSID, config, sizeof(WifiConfig));
		preferences.end();
	}

	WifiConfig *getWifiConfig()
	{
		preferences.begin(prefNamespace, false);
		WifiConfig *conf = new WifiConfig();
		preferences.getBytes(keyWifiSSID, conf, sizeof(WifiConfig));
		preferences.end();
		return conf;
	}

	void resetAllPinPreferences()
	{
		preferences.begin(prefNamespace, false);
		preferences.clear();
		preferences.end();
	}

	void checkifBootWentThrough()
	{
		// indicate if boot went through successfully
		log_i("Boot went through successfully");
		preferences.begin(prefNamespace, false);
		preferences.putBool(keyIsBooting, false);
		preferences.end();
	}

	void setup()
	{
		// check if boot process went through
		preferences.begin(prefNamespace, false);
		if (preferences.getBool(keyIsBooting, false))
		{ // if the boot process stopped for whatever reason, clear settings
			preferences.clear();
			log_i("The boot process stopped for whatever reason, clear settings");
		}
		else
		{
			log_i("Boot process went through..");
		}
		preferences.putBool(keyIsBooting, true);
		preferences.end();

		// if we boot for the first time => reset the preferences! // TODO: Smart? If not, we may have the problem that a wrong pin will block bootup
		if (isFirstRun())
		{
			// TODO: Not working yet, date is not getting set correctly in preferences...
			/*
			log_i("First Run, resetting config");
			preferences.begin(prefNamespace, false);
			preferences.clear();
			preferences.end();
			*/
		}
	}

	void getMotorPins(MotorPins *pins[])
	{
		preferences.begin(prefNamespace, false);
		pins[Stepper::A] = new MotorPins();
		if (preferences.getBytesLength(keyMotorADirPin) > 0)
		{
			preferences.getBytes(keyMotorADirPin, pins[Stepper::A], sizeof(MotorPins));
			log_i("get stepper a");
		}
		pins[Stepper::X] = new MotorPins();
		if (preferences.getBytesLength(keyMotorXDirPin) > 0)
		{
			preferences.getBytes(keyMotorXDirPin, pins[Stepper::X], sizeof(MotorPins));
			log_i("get stepper x");
		}
		pins[Stepper::Y] = new MotorPins();
		if (preferences.getBytesLength(keyMotorYDirPin) > 0)
		{
			preferences.getBytes(keyMotorYDirPin, pins[Stepper::Y], sizeof(MotorPins));
			log_i("get stepper y");
		}
		pins[Stepper::Z] = new MotorPins();
		if (preferences.getBytesLength(keyMotorZDirPin) > 0)
		{
			preferences.getBytes(keyMotorZDirPin, pins[Stepper::Z], sizeof(MotorPins));
			log_i("get stepper z");
		}
		preferences.end();
	}

	void setMotorPinConfig(MotorPins *pins[])
	{
		preferences.begin(prefNamespace, false);
		preferences.putBytes(keyMotorADirPin, pins[Stepper::A], sizeof(MotorPins));
		preferences.putBytes(keyMotorXDirPin, pins[Stepper::X], sizeof(MotorPins));
		preferences.putBytes(keyMotorYDirPin, pins[Stepper::Y], sizeof(MotorPins));
		preferences.putBytes(keyMotorZDirPin, pins[Stepper::Z], sizeof(MotorPins));
		preferences.end();
	}

	LedConfig *getLedPins()
	{
		preferences.begin(prefNamespace, false);
		LedConfig *config = new LedConfig();
		preferences.getBytes(keyLed, config, sizeof(LedConfig));
		preferences.end();
		return config;
	}

	void setLedPins(LedConfig *config)
	{
		preferences.begin(prefNamespace, false);
		preferences.putBytes(keyLed, config, sizeof(LedConfig));
		preferences.end();
	}

	void setLaserPins(LaserPins pins)
	{
		preferences.begin(prefNamespace, false);
		preferences.putInt(keyLaser1Pin, pins.LASER_PIN_1);
		preferences.putInt(keyLaser2Pin, pins.LASER_PIN_2);
		preferences.putInt(keyLaser3Pin, pins.LASER_PIN_3);
		preferences.end();
	}
	void getLaserPins(LaserPins pins)
	{
		preferences.begin(prefNamespace, false);
		pins.LASER_PIN_1 = preferences.getInt(keyLaser1Pin, pins.LASER_PIN_1);
		pins.LASER_PIN_2 = preferences.getInt(keyLaser2Pin, pins.LASER_PIN_2);
		pins.LASER_PIN_3 = preferences.getInt(keyLaser3Pin, pins.LASER_PIN_3);
		preferences.end();
	}

	void setDacPins(DacPins pins)
	{
		preferences.begin(prefNamespace, false);
		preferences.putInt(keyDACfake1Pin, pins.dac_fake_1);
		preferences.putInt(keyDACfake2Pin, pins.dac_fake_2);
		preferences.end();
	}
	void getDacPins(DacPins pins)
	{
		preferences.begin(prefNamespace, false);
		pins.dac_fake_1 = preferences.getInt(keyDACfake1Pin, pins.dac_fake_1);
		pins.dac_fake_2 = preferences.getInt(keyDACfake2Pin, pins.dac_fake_2);
		preferences.end();
	}
	void setAnalogInPins(AnalogInPins pins)
	{
		preferences.begin(prefNamespace, false);
		preferences.putInt(keyAnalogIn1Pin, pins.analogin_PIN_1);
		preferences.putInt(keyAnalogIn2Pin, pins.analogin_PIN_2);
		preferences.putInt(keyAnalogIn3Pin, pins.analogin_PIN_3);
		preferences.end();
	}
	void getAnalogInPins(AnalogInPins pins)
	{
		preferences.begin(prefNamespace, false);
		pins.analogin_PIN_1 = preferences.getInt(keyAnalogIn1Pin, pins.analogin_PIN_1);
		pins.analogin_PIN_2 = preferences.getInt(keyAnalogIn2Pin, pins.analogin_PIN_2);
		pins.analogin_PIN_3 = preferences.getInt(keyAnalogIn3Pin, pins.analogin_PIN_3);
		preferences.end();
	}
	void setAnalogOutPins(AnalogOutPins pins)
	{
		preferences.begin(prefNamespace, false);
		preferences.putInt(keyAnalogOut2Pin, pins.analogout_PIN_1);
		preferences.putInt(keyAnalogOut2Pin, pins.analogout_PIN_2);
		preferences.putInt(keyAnalogOut3Pin, pins.analogout_PIN_3);
		preferences.end();
	}
	void getAnalogOutPins(AnalogOutPins pins)
	{
		preferences.begin(prefNamespace, false);
		pins.analogout_PIN_1 = preferences.getInt(keyAnalogOut1Pin, pins.analogout_PIN_1);
		pins.analogout_PIN_2 = preferences.getInt(keyAnalogOut2Pin, pins.analogout_PIN_2);
		pins.analogout_PIN_3 = preferences.getInt(keyAnalogOut3Pin, pins.analogout_PIN_3);
		preferences.end();
	}
	void setDigitalOutPins(DigitalOutPins pins)
	{

		preferences.begin(prefNamespace, false);
		preferences.putInt(keyDigitalOut1Pin, pins.digitalout_PIN_1);
		preferences.putInt(keyDigitalOut2Pin, pins.digitalout_PIN_2);
		preferences.putInt(keyDigitalOut3Pin, pins.digitalout_PIN_3);
		preferences.end();
	}
	void getDigitalOutPins(DigitalOutPins pins)
	{
		preferences.begin(prefNamespace, false);
		pins.digitalout_PIN_1 = preferences.getInt(keyDigitalOut1Pin, pins.digitalout_PIN_1);
		pins.digitalout_PIN_2 = preferences.getInt(keyDigitalOut2Pin, pins.digitalout_PIN_2);
		pins.digitalout_PIN_3 = preferences.getInt(keyDigitalOut3Pin, pins.digitalout_PIN_3);
		preferences.end();
	}
	void setDigitalInPins(DigitalInPins pins)
	{

		preferences.begin(prefNamespace, false);
		preferences.putInt(keyDigitalIn1Pin, pins.digitalin_PIN_1);
		preferences.putInt(keyDigitalIn2Pin, pins.digitalin_PIN_2);
		preferences.putInt(keyDigitalIn3Pin, pins.digitalin_PIN_3);
		preferences.end();
	}
	void getDigitalInPins(DigitalInPins pins)
	{
		preferences.begin(prefNamespace, false);
		pins.digitalin_PIN_1 = preferences.getInt(keyDigitalIn1Pin, pins.digitalin_PIN_1);
		pins.digitalin_PIN_2 = preferences.getInt(keyDigitalIn2Pin, pins.digitalin_PIN_2);
		pins.digitalin_PIN_3 = preferences.getInt(keyDigitalIn3Pin, pins.digitalin_PIN_3);
		preferences.end();
	}

	bool isFirstRun()
	{
		bool rdystate = preferences.begin(prefNamespace, false);
		log_i("isFirstRun Start preferences rdy %s", rdystate ? "true" : "false");
		// define preference name
		const char *compiled_date = __DATE__ " " __TIME__;
		String stored_date = preferences.getString(dateKey, ""); // FIXME

		log_i("Stored date: %s", stored_date.c_str());
		log_i("Compiled date: %s", compiled_date);

		log_i("First run? ");
		if (!stored_date.equals(compiled_date))
		{
			log_i("yes, resetSettings");
			resetPreferences();
			preferences.putString(dateKey, compiled_date); // FIXME?
		}
		else
		{
			log_i("no, loadSettings");
		}
		preferences.end();

		rdystate = preferences.begin(prefNamespace, false);
		log_i("datatest pref rdy %s", rdystate ? "true" : "false");
		String datetest = preferences.getString(dateKey, "");
		preferences.end();
		log_i("isFirstRun End datetest:%s", datetest.c_str());
		return !stored_date.equals(compiled_date);
	}

	bool resetPreferences()
	{
		log_i("resetPreferences");
		preferences.clear();
		return true;
	}

	void setModuleConfig(ModuleConfig *pins)
	{
		preferences.begin(prefNamespace, false);
		size_t s = preferences.putBytes("module", pins, sizeof(ModuleConfig));
		log_i("setModuleConfig size:%i", s);
		preferences.end();
	}
	ModuleConfig *getModuleConfig()
	{
		preferences.begin(prefNamespace, true);
		ModuleConfig *pin = new ModuleConfig();
		size_t s = preferences.getBytes("module", pin, sizeof(ModuleConfig));
		log_i("getModuleConfig size:%i", s);
		preferences.end();
		return pin;
	}
}
