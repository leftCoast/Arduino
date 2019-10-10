
#include "adafruit_1431_Obj.h"
#include "debug.h"
#include "screen.h"
#include "colorRect.h"
#include "mechButton.h"

#define OLED_CS     10
#define OLED_RST    6
#define WIDTH       128
#define HEIGHT      128
#define POT_BTN     4


colorObj colors[12];


colorRect qBox(1, 1, 1, 1);
colorRect cBox(1, 1, 1, 1);
mechButton      ourClicker(POT_BTN);

int hour = 1;

void setup() {

  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    db.trace((char*)"No Screen?!\n");
    while (1);
  }
  screen->fillScreen(&black);
  colors[0].setColor(LC_RED);
  colors[1].setColor(LC_ORANGE);
  colors[2].setColor(LC_YELLOW);
  colors[3].setColor(LC_GREEN);
  colors[4].setColor(LC_BLUE);
  colors[5].setColor(LC_PURPLE);
  setBoxes(hour, 1);
  viewList.addObj(&qBox);
  viewList.addObj(&cBox);
}


void setBoxes(int hours, int minutes) {

  hours--;
  if (hours >= 0 && hours < 6) {              // Up, left
    qBox.setRect(0, 0, WIDTH/2, HEIGHT);
    qBox.setColor(&black);
    cBox.setRect(WIDTH/2, 0, WIDTH/2, HEIGHT);
    cBox.setColor(&colors[hours]);
  } else if (hours < 12) {                    // Side, top
    qBox.setRect(0, 0, WIDTH, HEIGHT/2);
    qBox.setColor(&white);
    cBox.setRect(0, HEIGHT/2, WIDTH, HEIGHT/2);
    cBox.setColor(&colors[hours - 6]);
  } else if (hours < 18) {                    // Up, right
    qBox.setRect(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    qBox.setColor(&white);
    cBox.setRect(0, 0, WIDTH / 2, HEIGHT);
    cBox.setColor(&colors[hours - 12]);
  } else if (hours < 24) {                    // Side, bottom
    qBox.setRect(0, HEIGHT / 2, WIDTH, HEIGHT / 2);
    qBox.setColor(&black);
    cBox.setRect(0, 0, WIDTH, HEIGHT / 2);
    cBox.setColor(&colors[hours - 18]);
  }
}


void sleep(int ms) {
  for (int j = 0; j < ms; j++) {
    idle();
    delay(1);
  }
}



void loop() {
  idle();
  if (ourClicker.clicked()) {
    while(ourClicker.clicked());
    hour++;
    if (hour>24) hour = 1;
    setBoxes(hour, 1);
  }
}
