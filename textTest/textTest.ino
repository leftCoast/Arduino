#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeMono12pt7b.h>

#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSerifItalic12pt7b.h>
#include <Fonts/FreeSerifBoldItalic12pt7b.h>

#include <Fonts/Picopixel.h>

#include <colorObj.h>

#define TFT_WIDTH    172
#define TFT_HEIGHT   320
#define TFT_CS       10
#define TFT_RST      3
#define TFT_BL       5
#define TFT_DC       9


#define X      10
#define Y      50
#define STEP   50
#define W      145
#define H      23

Adafruit_ST7789* theTFT;

void setup() {

   int   yPos = Y;

   /// Fire up the screen
   theTFT = new Adafruit_ST7789(TFT_CS,TFT_DC,TFT_RST);
   if (theTFT==NULL) {
      Serial.print("No TFT");
      while(1);
   }
   theTFT->init(TFT_WIDTH, TFT_HEIGHT);
   theTFT->fillScreen(black.getColor16());

   // Try the different ways to draw
   // ORiginal with size 3
   theTFT->drawRect(X,yPos,W,H,yellow.getColor16());
   theTFT->setTextColor(red.getColor16());
   theTFT->setTextSize(3); // Makes it a bit bigger.
   theTFT->setCursor(X,yPos);
   theTFT->print("Red gum");
   yPos = yPos + STEP;
   
   // Same but using Free fonts.
   // Using size 0 to get the sizes to be close.
   //theTFT->drawRect(X,yPos,W,H,yellow.getColor16());
   theTFT->setTextColor(blue.getColor16());
   theTFT->setTextSize(0); // Has odd effects with fonts
   theTFT->setFont(&FreeSansBoldOblique12pt7b);
   theTFT->setCursor(X,yPos);
   theTFT->print("Red jam");
   yPos = yPos + STEP;

   
   // Back to the original
   theTFT->drawRect(X,yPos,W,H,yellow.getColor16());
   theTFT->setTextColor(green.getColor16());
   theTFT->setTextSize(3); // Back to original
   theTFT->setFont(NULL);
   theTFT->setCursor(X,yPos);
   theTFT->print("Ol' tug!");
   yPos = yPos + STEP;
}


void loop() {  }
