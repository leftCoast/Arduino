#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "screenObj.h"

#define BOXSIZE 30
#define PENRADIUS 4

#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define ERASE_BTN_HEIGHT 6
screenObj tft;
int oldcolor, currentcolor;
colorObj backgroundSt(BLACK);
colorObj backgroundEnd(22,11,152);

void drawScreen(void) {
  tft.fillScreenGradient(backgroundSt.getColor16(),backgroundEnd.getColor16());
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);

  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
  currentcolor = RED;

  tft.fillRect(0,tft.height()-ERASE_BTN_HEIGHT, tft.width(),ERASE_BTN_HEIGHT,BLACK);
  tft.drawRect(0,tft.height()-ERASE_BTN_HEIGHT, tft.width(),ERASE_BTN_HEIGHT, WHITE);
}

void setup(void) {


  if (tft.init(LANDSCAPE)) {
    drawScreen();
    //Serial.begin(9600);
  }
  else
    while(1);
}


void loop() {

  Point p = tft.getPoint();
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    if (p.y > tft.height()-ERASE_BTN_HEIGHT) {
      // press the bottom of the screen to erase 
      drawScreen();
      while(p.z>MINPRESSURE)
        p = tft.getPoint();  // Hold 'till the finger is gone.
    } 
    else {

      if (p.y < BOXSIZE) {
        oldcolor = currentcolor;

        if (p.x < BOXSIZE) { 
          currentcolor = RED; 
          tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
        } 
        else if (p.x < BOXSIZE*2) {
          currentcolor = YELLOW; 
          tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
        }
        else if (p.x < BOXSIZE*3) {
          currentcolor = GREEN; 
          tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
        }
        else if (p.x < BOXSIZE*4) {
          currentcolor = CYAN; 
          tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
        }
        else if (p.x < BOXSIZE*5) {
          currentcolor = BLUE; 
          tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, WHITE);
        }
        else if (p.x < BOXSIZE*6) {
          currentcolor = MAGENTA; 
          tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);
        }
        else if (p.x < BOXSIZE*7) {
          currentcolor = WHITE; 
          tft.drawRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);
        }
        if (oldcolor != currentcolor) {
          if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == GREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
          if (oldcolor == CYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
          if (oldcolor == WHITE) tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);
        }
      }
      if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
        tft.fillRect(p.x, p.y, PENRADIUS,PENRADIUS, currentcolor);
      }
    }
  }
}










