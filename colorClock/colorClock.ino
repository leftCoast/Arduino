
#include "adafruit_1431_Obj.h"
#include "debug.h"
#include "screen.h"
#include "fontLabel.h"

#define OLED_CS     10
#define OLED_RST    6
#define LABEL_OFFSET  1

colorObj colors[12];
fontLabel num(0, 70, 80, 108);
fontLabel shade(0+LABEL_OFFSET, 70+LABEL_OFFSET, 80, 108);
void setup() {

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
  idle();
  for (int i = 0; i < 12; i++) {
    screen->fillScreen(&colors[i]);
    num.setValue(i+1);
    shade.setValue(i+1);
    for (int j = 0; j < 3000; j++) {
      idle();
      delay(1);
    }
  }
}
