#include <LOLIN_EPD.h>
#include <Adafruit_GFX.h>

/*D32 Pro*/
#define EPD_CS 14
#define EPD_DC 27
#define EPD_RST 33  // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY -1 // can set to -1 to not use a pin (will wait a fixed delay)

LOLIN_IL3897 EPD(250, 122, EPD_DC, EPD_RST, EPD_CS, EPD_BUSY); //hardware SPI

// forward declarations
int getOrder(float value);
int getDeltaSize(int delta);
int getSize(float value);

void setupEPD(void) 
{
  EPD.begin();
  EPD.clearBuffer();
  EPD.fillScreen(EPD_WHITE);
}
 
void epdDisplay()
{
  EPD.display();
}

void epdDrawLines()
{
  EPD.drawFastHLine(  1,  25, 250, EPD_BLACK);
  EPD.drawFastHLine(  1,  26, 250, EPD_BLACK);
  EPD.drawFastVLine( 83,  27,  80, EPD_BLACK);
  EPD.drawFastVLine( 84,  27,  80, EPD_BLACK);
  EPD.drawFastVLine(167,  27,  80, EPD_BLACK);
  EPD.drawFastVLine(168,  27,  80, EPD_BLACK);
  EPD.drawFastHLine(  1, 107, 250, EPD_BLACK);
  EPD.drawFastHLine(  1, 108, 250, EPD_BLACK);
}

void epdSetDate(String dateString)
{
  EPD.setTextColor(EPD_BLACK);

  EPD.setTextSize(2);
  EPD.setCursor(125 - (6*dateString.length()), 3);
  EPD.println(dateString);

}

void epdDrawValues( int box, float current, float previous, int delta, float consumption)
{
  int x = 0;
  int y = 60;
  int accuracy = 0;
  
  if ( box == 1 ) {
    x = 1;
    if ( getOrder(current) < 3 )
      accuracy = 1;
  } else if ( box == 2 ) {
    x = 85;
  } else if ( box == 3 ) {
    x = 169;
  }
  
  EPD.setTextColor(EPD_BLACK);

  int fontSize = 3;
  EPD.setTextSize(fontSize);
  EPD.setCursor(x+41 - (6*fontSize*(getOrder(current)+2*accuracy)-fontSize)/2, y);
  EPD.println(current, accuracy);

  fontSize = 1;
  EPD.setTextSize(fontSize);
  EPD.setCursor(x+2, 31);
  //EPD.setCursor(x+2, y -(8+20));
  EPD.println(previous, accuracy);
  
  EPD.setTextSize(fontSize);
  EPD.setCursor(x+79 - (fontSize*6*getDeltaSize(delta)-fontSize) , 31);
  //EPD.setCursor(x+79 - (fontSize*6*getDeltaSize(delta)-fontSize) , y-(8+20));
  EPD.print(delta);
  EPD.println("%");

  if ( consumption > 0 ) {
    EPD.setCursor(x+2, y+(3*8+12));
    EPD.println(consumption, accuracy);

    float delta = current - consumption;
    EPD.setCursor(x+79 - (fontSize*6*(getSize(delta)+2*accuracy)-fontSize), y+(3*8+12));
    EPD.println(delta, accuracy);
  }
}

void epdDayStatistics(float current, float previous, int delta, float consumed)
{
  epdDrawValues(1, current, previous, delta, consumed);
}

void epdMonthStatistics(float current, float previous, int delta, float consumed)
{
  epdDrawValues(2, current, previous, delta, consumed);
}

void epdYearStatistics(float current, float previous, int delta, float consumed)
{
  epdDrawValues(3, current, previous, delta, consumed);
}

void epdGenerating(bool generating)
{
  if ( generating ) 
  {
    EPD.setTextColor(EPD_BLACK);
    EPD.setTextSize(1);
    EPD.setCursor(95, 113);
    EPD.print("GENERATING");
  }
}

void epdPrintUpdateTime(String time)
{
  EPD.setTextColor(EPD_BLACK);
  EPD.setTextSize(1);
  EPD.setCursor(218, 113);
  EPD.println(time);
}

void epdPrintBatteryState(int percentage, float voltage)
{
  EPD.setTextColor(EPD_BLACK);
  EPD.setTextSize(1);
  EPD.setCursor(1, 113);
  EPD.print(percentage);
  EPD.print("%, ");
  EPD.print(voltage,1);
  EPD.println(" V");
}

void epdSleep()
{
  Serial.println("Put EPD display in sleep");
  EPD.deepSleep();
}

///////////////////////////////////////////////////////
// Helpers

int getOrder(float value)
{
  // returns 'size' of value, i.e. the order of magnitude
  return (value < 1) ? 1 : floor(log10(value+0.5)+1);
}

int getSize(float value)
{
  int sign = value < 0 ? 1 : 0;
  return getOrder(abs(value)) + sign;
}

int getDeltaSize(int delta)
{
  return getSize(delta) + 1;
}
