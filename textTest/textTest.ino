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
#define W      300
#define H      23

Adafruit_ST7789* theTFT;

void setup() {
   char text[] = {"Red jam at night"};
   int16_t  yPos = Y;
   int16_t  bX;
   int16_t  bY;
   int16_t  bW;
   int16_t  bH;
   int      offset;


   Serial.begin(9600);
   /// Fire up the screen
   theTFT = new Adafruit_ST7789(TFT_CS,TFT_DC,TFT_RST);
   if (theTFT==NULL) {
      Serial.print("No TFT");
      while(1);
   }
   theTFT->init(TFT_WIDTH, TFT_HEIGHT);
   theTFT->setRotation(1);
   theTFT->fillScreen(black.getColor16());

   
   // Same but using Free fonts.
   // Using size 0 to get the sizes to be close.
   theTFT->drawRect(X,yPos,W,H,yellow.getColor16());
   theTFT->setTextColor(blue.getColor16());
   theTFT->setTextSize(1); // Has odd effects with fonts
   theTFT->setFont(&FreeSans18pt7b);
   theTFT->setCursor(X,yPos+H);
   theTFT->getTextBounds(text,0,0,&bX,&bY,&bW,&bH);
   //theTFT->getTextBounds(text,X,yPos+H,&bX,&bY,&bW,&bH);
   theTFT->drawRect(bX,bY,bW,bH,green.getColor16());
   offset = (W-bW)/2;
   Serial.println(W);
   Serial.println(bW);
   Serial.println(W-bW);
   Serial.println(offset);
   theTFT->setCursor(X+offset,yPos+H);
   theTFT->print(text);
   yPos = yPos + STEP;

   /*
   // Back to the original
   theTFT->drawRect(X,yPos,W,H,yellow.getColor16());
   theTFT->setTextColor(green.getColor16());
   theTFT->setTextSize(3); // Back to original
   theTFT->setFont(NULL);
   theTFT->setCursor(X,yPos);
   theTFT->print("Ol' tug!");
   yPos = yPos + STEP;
   */
}


void loop() {  }
