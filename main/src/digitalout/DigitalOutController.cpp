#include "DigitalOutController.h"


// This is for digitaloutout

namespace RestApi
{
	void DigitalOut_act()
	{
		serialize(moduleController.get(AvailableModules::digitalout)->act(deserialize()));
	}

	void DigitalOut_get()
	{
		serialize(moduleController.get(AvailableModules::digitalout)->get(deserialize()));
	}

	void DigitalOut_set()
	{
		serialize(moduleController.get(AvailableModules::digitalout)->set(deserialize()));
	}
}

DigitalOutController::DigitalOutController(/* args */){};
DigitalOutController::~DigitalOutController(){};

// Custom function accessible by the API
int DigitalOutController::act(DynamicJsonDocument jsonDocument)
{
	// here you can do something
	Serial.println("digitalout_act_fct");
	isBusy = true;
	int triggerdelay = 10;

	// set default parameters
	// is trigger active?
	if(jsonDocument.containsKey(keyDigitalOut1IsTrigger))
		is_digital_trigger_1 = (jsonDocument)[keyDigitalOut1IsTrigger];
	else
		is_digital_trigger_1 = false;
	if(jsonDocument.containsKey(keyDigitalOut2IsTrigger))
		is_digital_trigger_2 = (jsonDocument)[keyDigitalOut2IsTrigger];
	else
		is_digital_trigger_2 = false;
	if(jsonDocument.containsKey(keyDigitalOut3IsTrigger))
		is_digital_trigger_3 = (jsonDocument)[keyDigitalOut3IsTrigger];
	else
		is_digital_trigger_3 = false;
	
	// on-delay
	if(jsonDocument.containsKey(keyDigitalOut1TriggerDelayOn))
		digitalout_trigger_delay_1_on = (jsonDocument)[keyDigitalOut1TriggerDelayOn];
	else
		digitalout_trigger_delay_1_on = 0;
	if(jsonDocument.containsKey(keyDigitalOut2TriggerDelayOn))
		digitalout_trigger_delay_2_on = (jsonDocument)[keyDigitalOut2TriggerDelayOn];
	else
		digitalout_trigger_delay_2_on = 0;
	if(jsonDocument.containsKey(keyDigitalOut3TriggerDelayOn))	
		digitalout_trigger_delay_3_on = (jsonDocument)[keyDigitalOut3TriggerDelayOn];
	else
		digitalout_trigger_delay_3_on = 0;
	
	// off-delay
	if(jsonDocument.containsKey(keyDigitalOut1TriggerDelayOff))
		digitalout_trigger_delay_1_off = (jsonDocument)[keyDigitalOut1TriggerDelayOff];
	else
		digitalout_trigger_delay_1_off = 0;
	if(jsonDocument.containsKey(keyDigitalOut2TriggerDelayOff))
		digitalout_trigger_delay_2_off = (jsonDocument)[keyDigitalOut2TriggerDelayOff];
	else
		digitalout_trigger_delay_2_off = 0;
	if(jsonDocument.containsKey(keyDigitalOut3TriggerDelayOff))
		digitalout_trigger_delay_3_off = (jsonDocument)[keyDigitalOut3TriggerDelayOff];
	else
		digitalout_trigger_delay_3_off = 0;

	// reset trigger
	if(jsonDocument.containsKey(keyDigitalOutIsTriggerReset)){
		if((jsonDocument)[keyDigitalOutIsTriggerReset]){
			is_digital_trigger_1 = false;
			is_digital_trigger_2 = false;
			is_digital_trigger_3 = false;
		}
	}

	// 
	int digitaloutid=0;
	int digitaloutval=0;
	if (jsonDocument.containsKey(keyDigitalOutid))
		digitaloutid = (jsonDocument)[keyDigitalOutid];
	if (jsonDocument.containsKey(keyDigitalOutVal))
		digitaloutval = (jsonDocument)[keyDigitalOutVal];
		

	// print debugging information
	log_d("digitaloutid %d", digitaloutid);
	log_d("digitaloutval %d", digitaloutval);
	log_d("digitalout_trigger_delay_1_on %d", digitalout_trigger_delay_1_on);
	log_d("digitalout_trigger_delay_1_off %d", digitalout_trigger_delay_1_off);
	log_d("digitalout_trigger_delay_2_on %d", digitalout_trigger_delay_2_on);
	log_d("digitalout_trigger_delay_2_off %d", digitalout_trigger_delay_2_off);
	log_d("digitalout_trigger_delay_3_on %d", digitalout_trigger_delay_3_on);
	log_d("digitalout_trigger_delay_3_off %d", digitalout_trigger_delay_3_off);
	log_d("is_digital_trigger_1 %d", is_digital_trigger_1);
	log_d("is_digital_trigger_2 %d", is_digital_trigger_2);
	log_d("is_digital_trigger_3 %d", is_digital_trigger_3);

	if (digitaloutid == 1)
	{
		digitalout_val_1 = digitaloutval;
		if (digitaloutval == -1)
		{
			// perform trigger
			digitalWrite(pins.digitalout_PIN_1, HIGH);
			delay(triggerdelay);
			digitalWrite(pins.digitalout_PIN_1, LOW);
		}
		else
		{
			digitalWrite(pins.digitalout_PIN_1, digitalout_val_1);
			Serial.print("digitalout_PIN ");
			Serial.println(pins.digitalout_PIN_1);
		}
	}
	else if (digitaloutid == 2)
	{
		digitalout_val_2 = digitaloutval;
		if (digitaloutval == -1)
		{
			// perform trigger
			digitalWrite(pins.digitalout_PIN_2, HIGH);
			delay(triggerdelay);
			digitalWrite(pins.digitalout_PIN_2, LOW);
		}
		else
		{
			digitalWrite(pins.digitalout_PIN_2, digitalout_val_2);
			Serial.print("digitalout_PIN ");
			Serial.println(pins.digitalout_PIN_2);
		}
	}
	else if (digitaloutid == 3)
	{
		digitalout_val_3 = digitaloutval;
		if (digitaloutval == -1)
		{
			// perform trigger
			digitalWrite(pins.digitalout_PIN_3, HIGH);
			delay(triggerdelay);
			digitalWrite(pins.digitalout_PIN_3, LOW);
		}
		else
		{
			digitalWrite(pins.digitalout_PIN_3, digitalout_val_3);
			Serial.print("digitalout_PIN ");
			Serial.println(pins.digitalout_PIN_3);
		}
	}
	return 1;
}

int DigitalOutController::set(DynamicJsonDocument jsonDocument)
{
	// here you can set parameters
	int digitaloutid = (jsonDocument)["digitaloutid"];
	int digitaloutpin = (jsonDocument)["digitaloutpin"];
	if (DEBUG)
		Serial.print("digitaloutid ");
	Serial.println(digitaloutid);
	if (DEBUG)
		Serial.print("digitaloutpin ");
	Serial.println(digitaloutpin);

	if (digitaloutid != 0 and digitaloutpin != 0)
	{
		if (digitaloutid == 1)
		{
			pins.digitalout_PIN_1 = digitaloutpin;
			pinMode(pins.digitalout_PIN_1, OUTPUT);
			digitalWrite(pins.digitalout_PIN_1, LOW);
		}
		else if (digitaloutid == 2)
		{
			pins.digitalout_PIN_2 = digitaloutpin;
			pinMode(pins.digitalout_PIN_2, OUTPUT);
			digitalWrite(pins.digitalout_PIN_2, LOW);
		}
		else if (digitaloutid == 3)
		{
			pins.digitalout_PIN_3 = digitaloutpin;
			pinMode(pins.digitalout_PIN_3, OUTPUT);
			digitalWrite(pins.digitalout_PIN_3, LOW);
		}
	}
	Config::setDigitalOutPins(pins);
	isBusy = false;
	return 1;
}

// Custom function accessible by the API
DynamicJsonDocument DigitalOutController::get(DynamicJsonDocument jsonDocument)
{
	// GET SOME PARAMETERS HERE
	int digitaloutid = jsonDocument["digitaloutid"];
	int digitaloutpin = 0;
	int digitaloutval = 0;

	if (digitaloutid == 1)
	{
		if (DEBUG)
			Serial.println("digitalout 1");
		digitaloutpin = pins.digitalout_PIN_1;
		digitaloutval = digitalout_val_1;
	}
	else if (digitaloutid == 2)
	{
		if (DEBUG)
			Serial.println("AXIS 2");
		if (DEBUG)
			Serial.println("digitalout 2");
		digitaloutpin = pins.digitalout_PIN_2;
		digitaloutval = digitalout_val_2;
	}
	else if (digitaloutid == 3)
	{
		if (DEBUG)
			Serial.println("AXIS 3");
		if (DEBUG)
			Serial.println("digitalout 1");
		digitaloutpin = pins.digitalout_PIN_3;
		digitaloutval = digitalout_val_3;
	}

	jsonDocument.clear();
	jsonDocument["digitaloutid"] = digitaloutid;
	jsonDocument["digitaloutval"] = digitaloutval;
	jsonDocument["digitaloutpin"] = digitaloutpin;
	return jsonDocument;
}

void DigitalOutController::setup()
{
	Config::getDigitalOutPins(pins);
	Serial.println("Setting Up digitalout");
	/* setup the output nodes and reset them to 0*/
	pinMode(pins.digitalout_PIN_1, OUTPUT);

	digitalWrite(pins.digitalout_PIN_1, HIGH);
	delay(50);
	digitalWrite(pins.digitalout_PIN_1, LOW);

	pinMode(pins.digitalout_PIN_2, OUTPUT);
	digitalWrite(pins.digitalout_PIN_2, HIGH);
	delay(50);
	digitalWrite(pins.digitalout_PIN_2, LOW);

	pinMode(pins.digitalout_PIN_3, OUTPUT);
	digitalWrite(pins.digitalout_PIN_3, HIGH);
	delay(50);
	digitalWrite(pins.digitalout_PIN_3, LOW);
}

void DigitalOutController::loop(){

	// run trigger table based on previously set parameters
	if (is_digital_trigger_1){
		digitalWrite(pins.digitalout_PIN_1, HIGH);
		//log_i("digitalout_PIN_1 HIGH");
		delay(digitalout_trigger_delay_1_on);
		digitalWrite(pins.digitalout_PIN_1, LOW);
		//log_i("digitalout_PIN_1 LOW");
		delay(digitalout_trigger_delay_1_off);
	}
	if (is_digital_trigger_2){
		digitalWrite(pins.digitalout_PIN_2, HIGH);
		//log_i("digitalout_PIN_2 HIGH");
		delay(digitalout_trigger_delay_2_on);
		digitalWrite(pins.digitalout_PIN_2, LOW);
		//log_i("digitalout_PIN_2 LOW");
		delay(digitalout_trigger_delay_2_off);
	}
	if (is_digital_trigger_3){
		digitalWrite(pins.digitalout_PIN_3, HIGH);
		//log_i("digitalout_PIN_3 HIGH");
		delay(digitalout_trigger_delay_3_on);
		digitalWrite(pins.digitalout_PIN_3, LOW);
		//log_i("digitalout_PIN_3 LOW");
		delay(digitalout_trigger_delay_3_off);
	}

}
