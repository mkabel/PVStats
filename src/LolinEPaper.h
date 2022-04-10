#pragma once

void setupEPD(void);
void epdDisplay();
void epdSetDate(String dateString);
void epdDrawLines();
void epdDrawValues( int box, float current, float previous, int delta, float consumption);
void epdDayStatistics(float current, float previous, int delta, float consumed);
void epdMonthStatistics(float current, float previous, int delta, float consumed);
void epdYearStatistics(float current, float previous, int delta, float consumed);
void epdGenerating(bool generating);
void epdPrintUpdateTime(String time);
void epdPrintBatteryState(int percentage, float voltage);
void epdSleep();