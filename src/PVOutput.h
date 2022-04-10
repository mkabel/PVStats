/* 
 *  PVOutput GET methods
 */

#pragma once

#include <Arduino.h>
#include <HTTPClient.h>

typedef struct {
  String date;
  int outputs;
  int generated;
  float efficiency;
  int consumed;
  bool generating;
  bool valid;
} SOLAR;

class PVOutput 
{

public:

  PVOutput(HTTPClient& http);
  ~PVOutput() {};

  int MonthAggregate(time_t t, String& payload);
  int YearAggregate(time_t t, String& payload);
  int GetStatus(time_t t, String& payload );
  int GetOutput(String& from, String& to, String mORy, String& payload );

  SOLAR ParseStatus(String& payload, int httpCode);
  SOLAR ParseAggregate(String& payload, int httpCode);

  String getTimeString(time_t t);

private:

  int Get(String& queryString, String& payload );
  HTTPClient& _http;
};
