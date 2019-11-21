#include "adafruit_1431_Obj.h"
#include "screen.h"
#include "mechButton.h"
#include "fontLabel.h"
#include "runningAvg.h"
#include "RTClib.h"


#define OLED_CS       10
#define OLED_RST      6
#define LABEL_OFFSET  1
#define PERC_WINDOW   3
#define POT_BTN       4


colorObj    colors[12];
fontLabel   num(0, 70, 80, 108);
fontLabel   shade(0+LABEL_OFFSET, 70+LABEL_OFFSET, 80, 108);
label       timeDisp(75,110,128-75,12);
mapper      dimmer(0,1024,0,100);
runningAvg  smoother(20);
int         dimPercent;
DateTime    drawtime;
RTC_DS3231  rtc;
mechButton  ourClicker(POT_BTN);
bool        showTime;


void setup() {
  
  rtc.begin();
  
  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    while (1);
  }
  
  screen->fillScreen(&cyan);
  colors[0].setColor(LC_RED);
  colors[1].setColor(LC_ORANGE);
  colors[2].setColor(LC_YELLOW);
  colors[3].setColor(LC_GREEN);
  colors[4].setColor(LC_BLUE);
  colors[5].setColor(LC_PURPLE);
  colors[6].setColor(LC_PINK);
  colors[7].setColor(LC_ORANGE);
  colors[8].setColor(LC_YELLOW);
  colors[9].setColor(LC_GREEN);
  colors[10].setColor(LC_LIGHT_BLUE);
  colors[11].setColor(LC_LAVENDER);

  num.setTextSize(4);
  num.setJustify(TEXT_RIGHT);
  num.setColors(&white);
  viewList.addObj(&num);
  
  shade.setTextSize(4);
  shade.setJustify(TEXT_RIGHT);
  shade.setColors(&black);
  viewList.addObj(&shade);
  viewList.addObj(&timeDisp);
  showTime = false;
  drawClock();
}


void  drawClock(void) {
  
  int index;
  colorObj  aColor;
  char  timeStr[10];
  
  DateTime now = rtc.now();
  index = now.hour();
  if (index>12) {
    index = index - 12;
  }
  index--;
  aColor.setColor(&(colors[index]));
  aColor.blend(&black,dimPercent);
  screen->fillScreen(&aColor);
  num.setValue(now.hour());
  shade.setValue(now.hour());
  
  if (showTime) {
    snprintf (timeStr,TEMP_BUFF_SIZE,"%d:%d",now.hour(),now.minute());
    timeDisp.setValue(timeStr);
  } else {
    timeDisp.setValue(""); 
  }
  drawtime = now;
}


void loop() {

  float rawData;
  int   newPercent;
  bool  needDraw;
  
  idle();
  needDraw = false;
  DateTime now = rtc.now();
  if (now.hour()!=drawtime.hour()) {
    needDraw = true;
  }
  rawData = smoother.addData(analogRead(A0));
  newPercent = round(dimmer.Map(rawData));
  if (newPercent>dimPercent+PERC_WINDOW ||newPercent<dimPercent-PERC_WINDOW) {
    dimPercent = newPercent;
    needDraw = true;
  }

  if (ourClicker.clicked()) {
    while(ourClicker.clicked());
    showTime = ! showTime;
    needDraw = true;
  }

  if (showTime && now.minute()!=drawtime.minute()) {
    needDraw = true;
  }

  if (needDraw) drawClock();
}
