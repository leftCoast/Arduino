#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Adafruit_ILI9341.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <adafruit_1431_Obj.h> // ADAFRUIT_1431
//#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <bmpPipe.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#define SD_CS         ADAFRUIT_1431_SPI_SD_CS
#define DISP_BG_FILE  "/dbos/paper.bmp"
//bmpPipe paper;
label firstLabel(20,20,100,20,"File name");
void setup() {

  Serial.begin(9600); while(!Serial);
  Serial.println("Serial's online.");

  if (!initScreen(ADAFRUIT_1431,INV_PORTRAIT,0)) {
    Serial.println("Init screen card fail.");
    while(true); // Kill the process.
  }
  screen->fillScreen(&blue);
  Serial.println("Screen\'s online.");
  screen->drawPixel(20,20,&black);
  Serial.println("Should be blue with one black pixel");
  delay(3000);
  screen->fillScreen(&white);
  //screen->setFont(&FreeMono9pt7b);
  screen->setTextColor(&black);
  firstLabel.draw();
  /*
  if (paper.openPipe(DISP_BG_FILE)) {
    Serial.println("Opened pipe.");
    paper.showPipe();
    //paper.drawBitmap(0,0);
  } else {
    Serial.println("Failed to open pipe.");
  }
  delay(3000);
  screen->fillScreen(&red);
  */
}


void loop() {
  idle();

}
