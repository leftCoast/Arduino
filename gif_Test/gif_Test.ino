#include <Adafruit_GFX.h>
#include <adafruit_1947_Obj.h>
#include <AnimatedGIF.h>

#include <colorObj.h>
#include <lists.h>
#include <screen.h>
#include <bitmap.h>
#include <timeObj.h>

//#include "test_images/badgers.h"
#include "test_images/homer.h"

int         xLoc;
int         yLoc;
int         width;
bitmap      aBitmap;
timeObj     frameTimer(2000,true);
AnimatedGIF gif;


void setup() {
   
   xLoc = 10;  // Picture location.
   yLoc = 10;
   width = 0;  // We don't have a buffer yet.
   Serial.begin(115200);
   if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
      Serial.println("NO SCREEN!");
      Serial.flush();
      while(true);
   }
   screen->fillScreen(&black);
   gif.begin(LITTLE_ENDIAN_PIXELS);
}


void GIFDraw(GIFDRAW *pDraw) {

   colorObj aColor;
   byte     colorIndex;
   word     RGB656Color;
   
   if (width==0) {
      aBitmap.setSize(pDraw->iWidth,1,true);
      width = pDraw->iWidth;
   }
   for (int i=0;i<width;i++) {
      colorIndex = pDraw->pPixels[i];
      RGB656Color = pDraw->pPalette[colorIndex];
      aColor.setColor(RGB656Color);
      aBitmap.setColor(i,0,&aColor);
      aBitmap.setAlpha(i,0,1);
      if (pDraw->ucHasTransparency) {
         if (colorIndex==pDraw->ucTransparent) {
            aBitmap.setAlpha(i,0,0);
         }
      }   
   }
   screen->blit(xLoc,yLoc+pDraw->y,&aBitmap);
}


void loop() {

   int timeAllowed;

   if (frameTimer.ding()) {
      if (gif.open((uint8_t *)ucHomer, sizeof(ucHomer), GIFDraw)) {
         //Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
         while (gif.playFrame(true, &timeAllowed)) { frameTimer.setTime(timeAllowed,true); }
         gif.close();
      }
   }
}
