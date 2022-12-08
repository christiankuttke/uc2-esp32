#pragma once
#include "ArduinoJson.h"
#include "../config/ConfigController.h"
#include "../wifi/Endpoints.h"

class SerialProcess
{
private:
    void jsonProcessor(String task,JsonObject jsonDocument);
    void serialize(DynamicJsonDocument doc);
    void serialize(int success);
    /* data */
public:
    SerialProcess(/* args */);
    ~SerialProcess();
    void loop();
};

extern SerialProcess serial;
