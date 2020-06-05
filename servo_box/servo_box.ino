#include "screen.h"
#include "adafruit_1431_Obj.h"
#include "idlers.h"
#include "colorRect.h"
#include "runningAvg.h"

#define OLED_CS   10
#define SD_CS     4
#define OLED_RST  6
#define POT_BTN   2
#define POT_PIN   A0
#define SERVO_PIN 22

mapper      percentMap(1023,0,0,100);
runningAvg  smoother(20);
colorMapper swatchMapper;
colorRect   swatch(10,10,10,10);

void setup() {
   if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
      while (1);
   }
   screen->fillScreen(&green);
   pinMode(POT_BTN, INPUT_PULLUP);
   swatch.setColor(&blue);
   viewList.addObj(&swatch);
   swatchMapper.setColors(&green,&blue);
}

void loop() {

  int       rawPotVal;
  float     percent;
  colorObj  aColor;
  
  idle();         // Idlers get their time.
  rawPotVal = analogRead(POT_PIN);
  percent = percentMap.Map(rawPotVal);
  percent = smoother.addData(percent);
  aColor = swatchMapper.Map(percent);
  swatch.setColor(&aColor);
  if (!digitalRead(POT_BTN)) {
      screen->fillScreen(&aColor);
  }
}
