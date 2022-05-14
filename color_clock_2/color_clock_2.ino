#include "Wire.h"
#include "RTClib.h"
#include <neoPixel.h>
//#include "lilParser.h"
#include <colorObj.h>

enum        mainComs  { noCommand,
                        help,
                        setHour,
                        setMin,
                        setSec,
                        reset,
                        cEdit,
                        gColor,
                        sColor
                      };

//RTC_DS3231  rtc;
//lilParser   mParser;
//colorObj    colors[];
neoPixel    theNeoPixel(1,3);

void setup() {
  Serial.begin(57600);  
  theNeoPixel.begin();
  theNeoPixel.setPixelColor(0,&blue);
  theNeoPixel.show();
}

void loop() {     // During loop..
   
   //idle();         // Idlers get their time.
}
