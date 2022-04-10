#include "Domoticz.h"
#include <Arduino.h>

#include <WiFiClient.h>
#include "Timer.h"
#include "mqtt_credentials.h"

// forward declarations
String idxString(int index);
String nvalueString(int value);
String svalueString(String value);
String closingString();
String ComposeDomoticzOneInt(int id, int value);
String ComposeDomoticzOneFloat(int id, float value);
String ComposeDomoticzTwoFloats(int id, float value1, float value2);

///////////////////////////////////////////////////////////////////////////////

Domoticz::Domoticz()
{
}

Domoticz::~Domoticz()
{
}

void Domoticz::InitialiseDomoticz(WiFiClient& wifiClient)
{
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(mqttServer, 1883);
}

void Domoticz::DomoticzReconnect() 
{
  // Loop until we're reconnected
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    //String mqttId = "ESP" + String(ESP.getChipId()) + "_" + sensorName;
    String mqttClientId = "PVStats";
    if (mqttClient.connect(mqttClientId.c_str(),mqtt_id,mqtt_pwd)) 
    {
      Serial.print(mqttClientId);
      Serial.println(" connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

bool Domoticz::CheckDomoticz()
{
  if ( !mqttClient.connected() ) 
  {
    DomoticzReconnect();
  }
  
  return mqttClient.loop();
}

bool Domoticz::Publish2Domoticz( String message )
{
  Serial.println(message);
  
  unsigned int len = message.length() + 2;
  char buf[len];
  message.toCharArray(buf, len);

  bool succes = mqttClient.publish("domoticz/in", buf);
  if ( !succes ) {
    // for some reason publishing failed... try forced disconnect. Next loop it will try a reconnect
    Serial.println("ERROR: Failed to publish MQTT message. Forced disconnect on client side");
    mqttClient.disconnect();
  }
  
  return succes;
}

bool Domoticz::LogValue2Domoticz(int id, int value)
{
  return Publish2Domoticz(ComposeDomoticzOneInt(id, value));  
}

bool Domoticz::LogValue2Domoticz(int id, float value)
{
  return Publish2Domoticz(ComposeDomoticzOneFloat(id, value));  
}

bool Domoticz::LogValue2Domoticz(int id, float value1, float value2)
{
  return Publish2Domoticz(ComposeDomoticzTwoFloats(id, value1, value2));
}

////////////////////////////////////////////////////////////////////////

String idxString(int index)
{
  return String( "{ \"idx\" : " + String(index) );
}

String nvalueString(int value)
{
  return String( ", \"nvalue\" : " + String(value) );
}

String svalueString(String value)
{
  return String( ", \"svalue\" : \"" + value );  
}

String closingString()
{
  return String(" }");
}

String ComposeDomoticzOneInt(int id, int value)
{
  return String( idxString(id) + nvalueString(value) + closingString() );
}

String ComposeDomoticzOneFloat(int id, float value)
{
  return String( idxString(id) + svalueString(String(value)) + "\"" + closingString() );
}

String ComposeDomoticzTwoFloats(int id, float value1, float value2)
{
  String values = String(value1) + ";" + String(value2) + ";0\"";
  return String( idxString(id) + nvalueString(0) + svalueString(values) + closingString() );
}
