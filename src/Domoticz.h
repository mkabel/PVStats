#pragma once

#ifndef DOMOTICZ_H
#define DOMOTICZ_H

#include <Arduino.h>
#include <PubSubClient.h>
class WiFiClient;

class Domoticz 
{
public:

    Domoticz();
    ~Domoticz();

    void InitialiseDomoticz(WiFiClient& wifiClient);
    bool CheckDomoticz();
    bool LogValue2Domoticz(int id, int value);
    bool LogValue2Domoticz(int id, float value);
    bool LogValue2Domoticz(int id, float value1, float value2);

private:

    void DomoticzReconnect();
    bool Publish2Domoticz( String message );

    PubSubClient mqttClient;
};

#endif // DOMOTICZ_H