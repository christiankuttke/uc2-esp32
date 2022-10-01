#include "ConfigController.h"

namespace Config
{

	Preferences preferences;
	PINDEF *pins;

	const char *prefNamespace = "UC2";

	/*
	  {
	  "motXstp": 1,
	  "motXdir": 2,
	  "motYstp": 3,
	  "motYdir": 4,
	  "motZstp": 5,
	  "motZdir": 6,
	  "motAstp": 7,
	  "motAdir": 8,
	  "motEnable": 9,
	  "ledArrPin": 0,
	  "ledArrNum": 64,
	  "digitalPin1":10,
	  "digitalPin2":11,
	  "analogPin1":12,
	  "analogPin2":13,
	  "analogPin3":14,
	  "laserPin1":15,
	  "laserPin2":16,
	  "laserPin3":17,
	  "dacFake1":18,
	  "dacFake2":19,
	  "identifier": "TEST",
	  "ssid": "ssid",
	  "PW": "PW"
	  }
	*/

	void setWifiConfig(String ssid, String pw, bool ap, bool prefopen)
	{
		bool open = prefopen;
		if (!prefopen)
			open = preferences.begin(prefNamespace, false);
		log_i("setWifiConfig ssid: %s, pw: %s, ap:%s prefopen:%s", ssid, pw, boolToChar(ap), boolToChar(open));
		preferences.putString(keyWifiSSID, ssid);
		preferences.putString(keyWifiPW, pw);
		preferences.putInt(keyWifiAP, ap);
		log_i("setWifiConfig pref ssid: %s pw:%s", preferences.getString(keyWifiSSID), preferences.getString(keyWifiPW));
		if (!prefopen)
			preferences.end();
	}

	void setJsonToPref(const char *key)
	{
		if (WifiController::getJDoc()->containsKey(key))
			preferences.putInt(key, (*WifiController::getJDoc())[key]);
	}

	void setPinsToJson(const char *key, int val)
	{
		(*WifiController::getJDoc())[key] = val;
	}

	void setup(PINDEF *pin)
	{
		pins = pin;
		// if we boot for the first time => reset the preferences! // TODO: Smart? If not, we may have the problem that a wrong pin will block bootup
		if (isFirstRun())
		{
			log_i("First Run, resetting config");
		}
		// check if setup went through after new config - avoid endless boot-loop
		// checkSetupCompleted();
	}

	void getMotorPins()
	{

		preferences.begin(prefNamespace, false);
		motor.pins[Stepper::X].STEP = preferences.getInt(keyMotorXStepPin);
		motor.pins[Stepper::X].DIR = preferences.getInt(keyMotorXDirPin);
		motor.pins[Stepper::X].ENABLE = preferences.getInt(keyMotorEnableX);
		motor.pins[Stepper::X].enable_inverted = preferences.getInt(keyMotorEnableXinverted);
		motor.pins[Stepper::X].step_inverted = preferences.getInt(keyMotorXStepPinInverted);
		motor.pins[Stepper::X].direction_inverted = preferences.getInt(keyMotorXDirPinInverted);

		motor.pins[Stepper::Y].STEP = preferences.getInt(keyMotorYStepPin);
		motor.pins[Stepper::Y].DIR = preferences.getInt(keyMotorYDirPin);
		motor.pins[Stepper::Y].ENABLE = preferences.getInt(keyMotorEnableY);
		motor.pins[Stepper::Y].enable_inverted = preferences.getInt(keyMotorEnableYinverted);
		motor.pins[Stepper::Y].step_inverted = preferences.getInt(keyMotorYStepPinInverted);
		motor.pins[Stepper::Y].direction_inverted = preferences.getInt(keyMotorYDirPinInverted);

		motor.pins[Stepper::Z].STEP = preferences.getInt(keyMotorZStepPin);
		motor.pins[Stepper::Z].DIR = preferences.getInt(keyMotorZDirPin);
		motor.pins[Stepper::Z].ENABLE = preferences.getInt(keyMotorEnableZ);
		motor.pins[Stepper::Z].enable_inverted = preferences.getInt(keyMotorEnableZinverted);
		motor.pins[Stepper::Z].step_inverted = preferences.getInt(keyMotorZStepPinInverted);
		motor.pins[Stepper::Z].direction_inverted = preferences.getInt(keyMotorZDirPinInverted);

		motor.pins[Stepper::A].STEP = preferences.getInt(keyMotorAStepPin);
		motor.pins[Stepper::A].DIR = preferences.getInt(keyMotorADirPin);
		motor.pins[Stepper::A].ENABLE = preferences.getInt(keyMotorEnableA);
		motor.pins[Stepper::A].enable_inverted = preferences.getInt(keyMotorEnableAinverted);
		motor.pins[Stepper::A].step_inverted = preferences.getInt(keyMotorAStepPinInverted);
		motor.pins[Stepper::A].direction_inverted = preferences.getInt(keyMotorADirPinInverted);
		preferences.end();
	}

	void setMotorPinConfig(bool prefsOpen)
	{
		if (!prefsOpen)
			preferences.begin(prefNamespace, false);
		preferences.putInt(keyMotorAStepPin, motor.pins[Stepper::A].STEP);
		preferences.putInt(keyMotorXStepPin, motor.pins[Stepper::X].STEP);
		preferences.putInt(keyMotorYStepPin, motor.pins[Stepper::Y].STEP);
		preferences.putInt(keyMotorZStepPin, motor.pins[Stepper::Z].STEP);

		preferences.putInt(keyMotorADirPin, motor.pins[Stepper::A].DIR);
		preferences.putInt(keyMotorXDirPin, motor.pins[Stepper::X].DIR);
		preferences.putInt(keyMotorYDirPin, motor.pins[Stepper::Y].DIR);
		preferences.putInt(keyMotorZDirPin, motor.pins[Stepper::Z].DIR);

		preferences.putInt(keyMotorEnableA, motor.pins[Stepper::A].ENABLE);
		preferences.putInt(keyMotorEnableX, motor.pins[Stepper::X].ENABLE);
		preferences.putInt(keyMotorEnableY, motor.pins[Stepper::Y].ENABLE);
		preferences.putInt(keyMotorEnableY, motor.pins[Stepper::Z].ENABLE);

		preferences.putInt(keyMotorADirPinInverted, motor.pins[Stepper::A].direction_inverted);
		preferences.putInt(keyMotorAStepPinInverted, motor.pins[Stepper::A].step_inverted);
		preferences.putInt(keyMotorEnableAinverted, motor.pins[Stepper::A].enable_inverted);

		preferences.putInt(keyMotorXDirPinInverted, motor.pins[Stepper::X].direction_inverted);
		preferences.putInt(keyMotorXStepPinInverted, motor.pins[Stepper::X].step_inverted);
		preferences.putInt(keyMotorEnableXinverted, motor.pins[Stepper::X].enable_inverted);

		preferences.putInt(keyMotorYDirPinInverted, motor.pins[Stepper::Y].direction_inverted);
		preferences.putInt(keyMotorYStepPinInverted, motor.pins[Stepper::Y].step_inverted);
		preferences.putInt(keyMotorEnableYinverted, motor.pins[Stepper::Y].enable_inverted);

		preferences.putInt(keyMotorZDirPinInverted, motor.pins[Stepper::Z].direction_inverted);
		preferences.putInt(keyMotorZStepPinInverted, motor.pins[Stepper::Z].step_inverted);
		preferences.putInt(keyMotorEnableZinverted, motor.pins[Stepper::Z].enable_inverted);

		log_i("Stepper A step:%i dir:%i enablepin:%i", preferences.getInt(keyMotorAStepPin), preferences.getInt(keyMotorADirPin), preferences.getInt(keyMotorEnableA));
		log_i("Stepper X step:%i dir:%i enablepin:%i", preferences.getInt(keyMotorXStepPin), preferences.getInt(keyMotorXDirPin), preferences.getInt(keyMotorEnableX));
		log_i("Stepper Y step:%i dir:%i enablepin:%i", preferences.getInt(keyMotorYStepPin), preferences.getInt(keyMotorYDirPin), preferences.getInt(keyMotorEnableY));
		log_i("Stepper Z step:%i dir:%i enablepin:%i", preferences.getInt(keyMotorZStepPin), preferences.getInt(keyMotorZDirPin), preferences.getInt(keyMotorEnableZ));
		if (!prefsOpen)
			preferences.end();
	}

	void getLedPins()
	{
		preferences.begin(prefNamespace, false);
		led.ledconfig.ledCount = preferences.getInt(keyLEDCount,0);
		led.ledconfig.ledPin = preferences.getInt(keyLEDPin,0);
		preferences.end();
	}

	void setLedPins(bool openPrefs)
	{
		if (openPrefs)
			preferences.begin(prefNamespace, false);
		preferences.putInt(keyLEDCount, led.ledconfig.ledCount);
		preferences.putInt(keyLEDPin, led.ledconfig.ledPin);
		if(openPrefs)
			preferences.end();
	}

	bool isFirstRun()
	{
		bool rdystate = preferences.begin(prefNamespace, false);
		log_i("isFirstRun Start preferences rdy %s", rdystate ? "true" : "false");
		// define preference name
		const char *dateKey = "date";
		const char *compiled_date = __DATE__ " " __TIME__;
		String stored_date = preferences.getString(dateKey, ""); // FIXME

		log_i("Stored date: %s", stored_date.c_str());
		log_i("Compiled date: %s", compiled_date);

		log_i("First run? ");
		if (!stored_date.equals(compiled_date))
		{
			log_i("yes, resetSettings");
			resetPreferences();
			initempty();
			savePreferencesFromPins(false);
			applyPreferencesToPins();
			preferences.putString(dateKey, compiled_date); // FIXME?
		}
		else
		{
			log_i("no, loadSettings");
			applyPreferencesToPins();
		}
		preferences.end();

		rdystate = preferences.begin(prefNamespace, false);
		log_i("datatest pref rdy %s", rdystate ? "true" : "false");
		String datetest = preferences.getString(dateKey, "");
		preferences.end();
		log_i("isFirstRun End datetest:%s", datetest);
		return !stored_date.equals(compiled_date);
	}

	bool resetPreferences()
	{
		log_i("resetPreferences");
		preferences.clear();
		return true;
	}

	void savePreferencesFromPins(bool openPrefs)
	{
		if (openPrefs)
			preferences.begin(prefNamespace, false);
		preferences.putInt(keyAnalog1Pin, (*pins).analog_PIN_1);
		preferences.putInt(keyAnalog1Pin, pins->analog_PIN_1);
		preferences.putInt(keyAnalog2Pin, pins->analog_PIN_2);
		preferences.putInt(keyAnalog3Pin, pins->analog_PIN_3);

		setMotorPinConfig(openPrefs);
		setLedPins(openPrefs);
		
		preferences.putInt(keyDigital1Pin, pins->digital_PIN_1);
		preferences.putInt(keyDigital2Pin, pins->digital_PIN_2);
		preferences.putInt(keyDigital3Pin, pins->digital_PIN_3);

		preferences.putInt(keyAnalog1Pin, pins->analog_PIN_1);
		preferences.putInt(keyAnalog2Pin, pins->analog_PIN_2);
		preferences.putInt(keyAnalog3Pin, pins->analog_PIN_3);

		preferences.putInt(keyLaser1Pin, pins->LASER_PIN_1);
		preferences.putInt(keyLaser2Pin, pins->LASER_PIN_2);
		preferences.putInt(keyLaser3Pin, pins->LASER_PIN_3);

		preferences.putInt(keyDACfake1Pin, pins->dac_fake_1);
		preferences.putInt(keyDACfake2Pin, pins->dac_fake_2);
		preferences.putString(keyIdentifier, pins->identifier_setup);
		preferences.putString(keyWifiSSID, WifiController::getSsid());
		preferences.putString(keyWifiPW, WifiController::getPw());
		preferences.putInt(keyWifiAP, WifiController::getAp());
		if (openPrefs)
			preferences.end();
	}

	void initempty()
	{
		preferences.putInt(keyAnalog1Pin, 0);
		preferences.putInt(keyAnalog1Pin, 0);
		preferences.putInt(keyAnalog2Pin, 0);
		preferences.putInt(keyAnalog3Pin, 0);

		preferences.putInt(keyMotorAStepPin, 0);
		preferences.putInt(keyMotorXStepPin, 0);
		preferences.putInt(keyMotorYStepPin, 0);
		preferences.putInt(keyMotorZStepPin, 0);

		preferences.putInt(keyMotorADirPin, 0);
		preferences.putInt(keyMotorXDirPin, 0);
		preferences.putInt(keyMotorYDirPin, 0);
		preferences.putInt(keyMotorZDirPin, 0);

		preferences.putInt(keyMotorEnableA, 0);
		preferences.putInt(keyMotorEnableX, 0);
		preferences.putInt(keyMotorEnableY, 0);
		preferences.putInt(keyMotorEnableZ, 0);

		preferences.putInt(keyMotorEnableAinverted, 0);
		preferences.putInt(keyMotorEnableXinverted, 0);
		preferences.putInt(keyMotorEnableYinverted, 0);
		preferences.putInt(keyMotorEnableZinverted, 0);

		preferences.putInt(keyLEDPin, 0);
		preferences.putInt(keyLEDCount, 0);

		preferences.putInt(keyDigital1Pin, 0);
		preferences.putInt(keyDigital2Pin, 0);
		preferences.putInt(keyDigital3Pin, 0);

		preferences.putInt(keyAnalog1Pin, 0);
		preferences.putInt(keyAnalog2Pin, 0);
		preferences.putInt(keyAnalog3Pin, 0);

		preferences.putInt(keyLaser1Pin, 0);
		preferences.putInt(keyLaser2Pin, 0);
		preferences.putInt(keyLaser3Pin, 0);

		preferences.putInt(keyDACfake1Pin, 0);
		preferences.putInt(keyDACfake2Pin, 0);
		preferences.putString(keyIdentifier, "");
		preferences.putString(keyWifiSSID, "Uc2");
		preferences.putString(keyWifiPW, "");
		preferences.putInt(keyWifiAP, true);
	}

	bool setPreferences()
	{
		preferences.begin(prefNamespace, false);

		setJsonToPref(keyDigital1Pin);
		setJsonToPref(keyDigital2Pin);
		setJsonToPref(keyDigital3Pin);

		setJsonToPref(keyAnalog1Pin);
		setJsonToPref(keyAnalog2Pin);
		setJsonToPref(keyAnalog3Pin);

		setJsonToPref(keyLaser1Pin);
		setJsonToPref(keyLaser2Pin);
		setJsonToPref(keyLaser3Pin);

		setJsonToPref(keyDACfake1Pin);
		setJsonToPref(keyDACfake2Pin);

		preferences.putString(keyIdentifier, (const char *)(*WifiController::getJDoc())[keyIdentifier]);
		setWifiConfig((*WifiController::getJDoc())[keyWifiSSID], (*WifiController::getJDoc())[keyWifiPW], (*WifiController::getJDoc())[keyWifiAP], true);
		preferences.end();
		return true;
	}

	void applyPreferencesToPins()
	{
		// motor loads pins on setup

		led.ledconfig.ledPin = preferences.getInt(keyLEDPin, 0);
		led.ledconfig.ledCount = preferences.getInt(keyLEDCount, 0);

		pins->digital_PIN_1 = preferences.getInt(keyDigital1Pin, pins->digital_PIN_1);
		pins->digital_PIN_2 = preferences.getInt(keyDigital2Pin, pins->digital_PIN_2);
		pins->digital_PIN_3 = preferences.getInt(keyDigital3Pin, pins->digital_PIN_3);

		pins->analog_PIN_1 = preferences.getInt(keyAnalog1Pin, pins->analog_PIN_1);
		pins->analog_PIN_2 = preferences.getInt(keyAnalog2Pin, pins->analog_PIN_2);
		pins->analog_PIN_3 = preferences.getInt(keyAnalog3Pin, pins->analog_PIN_3);

		pins->LASER_PIN_1 = preferences.getInt(keyLaser1Pin, pins->LASER_PIN_1);
		pins->LASER_PIN_2 = preferences.getInt(keyLaser2Pin, pins->LASER_PIN_2);
		pins->LASER_PIN_3 = preferences.getInt(keyLaser3Pin, pins->LASER_PIN_3);

		pins->dac_fake_1 = preferences.getInt(keyDACfake1Pin, pins->dac_fake_1);
		pins->dac_fake_2 = preferences.getInt(keyDACfake2Pin, pins->dac_fake_2);

		pins->identifier_setup = preferences.getString(keyIdentifier, pins->identifier_setup).c_str();
		if (WifiController::getSsid() != nullptr)
			log_i("ssid bevor:%s", WifiController::getSsid());
		else
			log_i("ssid is nullptr");
		String ssid = preferences.getString(keyWifiSSID);

		String pw = preferences.getString(keyWifiPW);
		bool ap = preferences.getInt(keyWifiAP);
		WifiController::setWifiConfig(ssid, pw, ap);
		log_i("ssid after:%s pref ssid:%s", WifiController::getSsid().c_str(), preferences.getString(keyWifiSSID, WifiController::getSsid()).c_str());
	}

	bool getPreferences()
	{

		preferences.begin(prefNamespace, false);
		applyPreferencesToPins();

		WifiController::getJDoc()->clear();

		// Assign to JSON WifiController::getJDoc()ument
		setPinsToJson(keyMotorXStepPin, motor.pins[Stepper::X].STEP);
		setPinsToJson(keyMotorXDirPin, motor.pins[Stepper::X].DIR);
		setPinsToJson(keyMotorEnableX, motor.pins[Stepper::X].ENABLE);

		setPinsToJson(keyMotorYStepPin, motor.pins[Stepper::Y].STEP);
		setPinsToJson(keyMotorYDirPin, motor.pins[Stepper::Y].DIR);
		setPinsToJson(keyMotorEnableY, motor.pins[Stepper::Y].ENABLE);

		setPinsToJson(keyMotorZStepPin, motor.pins[Stepper::Z].STEP);
		setPinsToJson(keyMotorZDirPin, motor.pins[Stepper::Z].DIR);
		setPinsToJson(keyMotorEnableZ, motor.pins[Stepper::Z].ENABLE);

		setPinsToJson(keyMotorAStepPin, motor.pins[Stepper::A].STEP);
		setPinsToJson(keyMotorADirPin, motor.pins[Stepper::A].DIR);
		setPinsToJson(keyMotorEnableA, motor.pins[Stepper::A].ENABLE);

		setPinsToJson(keyLEDCount, led.ledconfig.ledCount);
		setPinsToJson(keyLEDPin, led.ledconfig.ledPin);

		setPinsToJson(keyDigital1Pin, pins->digital_PIN_1);
		setPinsToJson(keyDigital2Pin, pins->digital_PIN_2);

		setPinsToJson(keyAnalog1Pin, pins->analog_PIN_1);
		setPinsToJson(keyAnalog2Pin, pins->analog_PIN_2);
		setPinsToJson(keyAnalog3Pin, pins->analog_PIN_3);

		setPinsToJson(keyLaser1Pin, pins->LASER_PIN_1);
		setPinsToJson(keyLaser2Pin, pins->LASER_PIN_2);
		setPinsToJson(keyLaser3Pin, pins->LASER_PIN_3);

		setPinsToJson(keyDACfake1Pin, pins->dac_fake_1);
		setPinsToJson(keyDACfake2Pin, pins->dac_fake_2);

		(*WifiController::getJDoc())[keyIdentifier] = pins->identifier_setup;
		(*WifiController::getJDoc())[keyWifiSSID] = WifiController::getSsid();
		(*WifiController::getJDoc())[keyWifiPW] = WifiController::getSsid();

		serializeJson(*WifiController::getJDoc(), Serial);

		preferences.end();
		return true;
	}

	void loop()
	{
		if (Serial.available())
		{
			DeserializationError error = deserializeJson((*WifiController::getJDoc()), Serial);

			if (error)
			{
				log_i("%s", error);
			}
			else
			{
				setPreferences();
				getPreferences();
			}
		}
	}

	void act()
	{
		resetPreferences();
		WifiController::getJDoc()->clear();
		(*WifiController::getJDoc())["return"] = 1;
	}

	void set()
	{
		setPreferences();
		WifiController::getJDoc()->clear();
		(*WifiController::getJDoc())["return"] = 1;
	}

	void get()
	{
		getPreferences();
		WifiController::getJDoc()->clear();
		(*WifiController::getJDoc())["return"] = 1;
	}

}
