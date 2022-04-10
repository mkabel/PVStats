/* 
 *  PVOutput GET methods
 */

#include "PVOutput.h"
#include <TimeLib.h>
#include "defines.h"

const String baseURL = "https://pvoutput.org/service/r2/";

// forward declarations
String getDateString(time_t t);
String getDateString(int year, int month, int day);
String getValue(String& data, char separator, int index);

PVOutput::PVOutput(HTTPClient& http)
  : _http(http)
{
}

int PVOutput::MonthAggregate(time_t t, String& payload)
{
  String from = getDateString(year(t), month(t), 1);
  String to = getDateString(t);

  return GetOutput(from, to, "m", payload);
}

int PVOutput::YearAggregate(time_t t, String& payload)
{
  String from = getDateString(year(t), 1, 1);
  String to = getDateString(t);

  return GetOutput(from, to, "y", payload);
}

int PVOutput::GetStatus(time_t t, String& payload )
{
  String queryString = baseURL + "getstatus.jsp?h=1&limit=1&d=" + getDateString(t) + "&to=" + getTimeString(t);
  return Get(queryString, payload);
}

int PVOutput::GetOutput(String& from, String& to, String mORy, String& payload )
{
  String queryString = baseURL + "getoutput.jsp?df=" + from + "&dt=" + to + "&a=" + mORy;
  return Get(queryString, payload);
}

int PVOutput::Get(String& queryString, String& payload )
{
  Serial.println(queryString);
  
  _http.begin(queryString);
  _http.addHeader("Content-Type", "text/plain");
  _http.addHeader("X-Pvoutput-Apikey", API_KEY);
  _http.addHeader("X-Pvoutput-SystemId", SYS_ID);
  
  int httpCode = _http.GET();
  if (httpCode > 0) 
  {    
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) 
    {
      payload = _http.getString();
    }
  }
  else 
  {
     Serial.printf("[HTTP] GET... failed, error: %s\n", _http.errorToString(httpCode).c_str());
  }
  _http.end();

  return httpCode;
}

String PVOutput::getTimeString(time_t t)
{
  char buffer[10];
  sprintf(buffer, "%02d:%02d", hour(t), minute(t));
  return String(buffer);
}

SOLAR PVOutput::ParseStatus(String& payload, int httpCode)
{
  SOLAR results;

  results.valid = (httpCode == 200) ? true : false;
  if ( !results.valid )
    return results;

  String str;
  int i = 0;
  while ( true )
  {
    str = getValue(payload, ',', i);
    if ( str == "" )
      break;

    switch ( i )
    {
    case 0:
      results.date = str;
      Serial.println("Date: " + results.date);
      break;
    case 1:
      results.outputs = 1;
      Serial.print("Nr of outputs: ");
      Serial.println(int(results.outputs));
      break;
    case 2:
      results.generated = str.toInt();
      Serial.print("Generated: ");
      Serial.println(int(results.generated));
      break;
    case 3:
      results.efficiency = str.toFloat();
      Serial.print("Efficiency: ");
      Serial.println(float(results.efficiency),3);
      break;
    case 4:
      results.generating = (str.toInt() > 0) ? true : false;
      break;
    case 7:
      results.consumed = str.toInt();
      Serial.print("Consumed: ");
      Serial.println(int(results.consumed));
      break;
    default:
      break;
    }

    ++i;
  }

  return results;
}

SOLAR PVOutput::ParseAggregate(String& payload, int httpCode )
{
  SOLAR results;
  // aggregate results are historical values, not active ones
  results.generating = false;

  results.valid = (httpCode == 200) ? true : false;
  if ( !results.valid )
    return results;
  
  String str;
  int i = 0;
  while ( true )
  {
    str = getValue(payload, ',', i);
    if ( str == "" )
      break;

    switch ( i )
    {
    case 0:
      results.date = str;
      Serial.println("Date: " + results.date);
      break;
    case 1:
      results.outputs = str.toInt();
      Serial.print("Nr of outputs: ");
      Serial.println(int(results.outputs));
      break;
    case 2:
      results.generated = str.toInt();
      Serial.print("Generated: ");
      Serial.println(int(results.generated));
      break;
    case 3:
      results.efficiency = str.toFloat();
      Serial.print("Efficiency: ");
      Serial.println(float(results.efficiency),3);
      break;
    case 5:
      results.consumed = str.toInt();
      Serial.print("Consumed: ");
      Serial.println(int(results.consumed));
      break;
    default:
      break;
    }

    ++i;
  }

  return results;
}

///////////////////////////////////////////////////////////////////////////////

String getValue(String& data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String getDateString(time_t t)
{
   return getDateString(year(t), month(t), day(t));
}

String getDateString(int year, int month, int day)
{
  char buffer[10];
  sprintf(buffer, "%04d%02d%02d", year, month, day);
  return String(buffer);
}



