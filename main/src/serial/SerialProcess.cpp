#include "SerialProcess.h"

SerialProcess::SerialProcess(/* args */)
{
}

SerialProcess::~SerialProcess()
{
}

void SerialProcess::loop()
{
	if (Serial.available())
	{
		String s = Serial.readString();
		int si = s.length() * 8;
		DynamicJsonDocument jsonDocument(si);
		DeserializationError error = deserializeJson(jsonDocument, s);
		// free(Serial);
		if (error)
		{
			Serial.print(F("deserializeJson() failed: "));
			Serial.println(error.f_str());
			return;
		}
		Serial.flush();
		if (jsonDocument.containsKey("tasks"))
		{
			log_i("task to process:%i", jsonDocument["tasks"].size());
			for (int i = 0; i < jsonDocument["tasks"].size(); i++)
			{
				String task_s = jsonDocument["tasks"][i]["task"];
				JsonObject doc = jsonDocument["tasks"][i].as<JsonObject>();
				jsonProcessor(task_s, doc);
			}
		}
		else
		{
			log_i("process singel task");
			String task_s = jsonDocument["task"];
			JsonObject ob = jsonDocument.as<JsonObject>();
			jsonProcessor(task_s, ob);
		}
	}
}

void SerialProcess::serialize(DynamicJsonDocument doc)
{
	// serializeJsonPretty((*WifiController::getJDoc()), Serial);
	serializeJson(doc, Serial);
}

void SerialProcess::serialize(int success)
{
	Serial.println(success);
}

void SerialProcess::jsonProcessor(String task, JsonObject jsonDocument)
{
	if (task == modules_set_endpoint)
		serialize(moduleController.set(jsonDocument));
	if (task == modules_get_endpoint)
		serialize(moduleController.get());
	/*
	  Drive Motors
	*/
	if (moduleController.get(AvailableModules::motor) != nullptr)
	{
		if (task == motor_act_endpoint)
		{
			serialize(moduleController.get(AvailableModules::motor)->act(jsonDocument));
		}
		if (task == motor_set_endpoint)
		{
			serialize(moduleController.get(AvailableModules::motor)->set(jsonDocument));
		}
		if (task == motor_get_endpoint)
		{
			serialize(moduleController.get(AvailableModules::motor)->get(jsonDocument));
		}
	}
	/*
	  Drive LED Matrix
	*/
	if (moduleController.get(AvailableModules::led) != nullptr)
	{
		if (task == ledarr_act_endpoint)
			serialize(moduleController.get(AvailableModules::led)->act(jsonDocument));
		if (task == ledarr_set_endpoint)
			serialize(moduleController.get(AvailableModules::led)->set(jsonDocument));
		if (task == ledarr_get_endpoint)
			serialize(moduleController.get(AvailableModules::led)->get(jsonDocument));
	}

	if (moduleController.get(AvailableModules::analogJoystick) != nullptr)
	{
		if (task == analog_joystick_set_endpoint)
			serialize(moduleController.get(AvailableModules::analogJoystick)->set(jsonDocument));
		if (task == analog_joystick_get_endpoint)
			serialize(moduleController.get(AvailableModules::analogJoystick)->get(jsonDocument));
	}

	if (task == scanwifi_endpoint)
	{
		RestApi::scanWifi();
	}
	if (task == connectwifi_endpoint)
	{
		WifiController::connect(jsonDocument);
	}
	if (task == reset_nv_flash_endpoint)
	{
		RestApi::resetNvFLash();
	}
	if (task == bt_connect_endpoint)
	{
		String mac = jsonDocument["mac"];
		int ps = jsonDocument["psx"];

		if (ps == 0)
		{
			BtController::setMacAndConnect(mac);
		}
		else
		{
			BtController::connectPsxController(mac, ps);
		}
	}
	Serial.println(task);
}
SerialProcess serial;