
#include "adafruit_1431_Obj.h"
#include "debug.h"
#include "screen.h"
#include "fontLabel.h"
#include "runningAvg.h"
#include "colorObj.h"
#include "mapper.h"
#include <Wire.h>

class clockObj {

  public:
          clockObj(byte addr);
  virtual ~clockObj(void);

          void  setTime(int hour,int min, int sec);
          void  getTime(int* hour,int* min, int* sec);

          byte  mAddr;
};


clockObj::clockObj(byte addr) { mAddr = addr; }


clockObj::~clockObj(void) {  }


void  clockObj::setTime(int hour,int min,int sec) {  }


void  clockObj::getTime(int* hour,int* min, int* sec) {

  int seconds = 0;
  int minutes = 0;
  int hours   = 0;
    
  // send request to receive data starting at register 0
  Wire.beginTransmission(mAddr);  // 0x68 is DS3231 device address
  Wire.write((byte)0);            // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(0x68, 3);      // request three bytes (seconds, minutes, hours)

  if (Wire.available()>=3) {
    seconds = Wire.read();          // get seconds
    minutes = Wire.read();          // get minutes
    hours   = Wire.read();          // get hours
    seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111)); // convert BCD to decimal
   minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111)); // convert BCD to decimal
    hours   = (((hours & 0b00100000)>>5)*20 + ((hours & 0b00010000)>>4)*10 + (hours & 0b00001111)); // convert BCD to decimal (assume 24 hour mode)
  }
  *hour   = hours;
  *min    = minutes;
  *sec    = seconds;
}


#define OLED_CS     10
#define OLED_RST    6
#define LABEL_OFFSET  1

colorObj colors[12];
fontLabel num(0, 70, 80, 108);
fontLabel shade(0+LABEL_OFFSET, 70+LABEL_OFFSET, 80, 108);
clockObj  theClk(0x68);
runningAvg  smoother(25);
mapper  dimmer(0,1024,0,100);
int gHour = 0;
int gSec = 0;
int gMin = 0;

void setup() {
   
  Wire.begin();
  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    db.trace((char*)"No Screen?!\n");
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
  //for (int i = 0; i <= 10; i++) colors[i].blend(&black, 20);
  //for (int i = 6; i <= 11; i++) colors[i].blend(&white, 20);

  num.setTextSize(4);
  num.setJustify(TEXT_RIGHT);
  num.setColors(&white);
  viewList.addObj(&num);
  
  shade.setTextSize(4);
  shade.setJustify(TEXT_RIGHT);
  shade.setColors(&black);
  viewList.addObj(&shade);
  
}



void loop() {
  
  int hour;
  int min;
  int sec;
  
  
  idle();
  theClk.getTime(&hour,&min,&sec);
  if (hour!=gHour) {
    gHour = hour;
    num.setValue(gHour);
    shade.setValue(gHour);
  }
}
