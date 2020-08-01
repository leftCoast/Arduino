
#include <SD.h>
#include <Adafruit_GFX.h>
#include <adafruit_1947_Obj.h>
#include <AnimatedGIF.h>

#include <colorObj.h>
#include <lists.h>
#include <screen.h>
#include <bitmap.h>
#include <timeObj.h>

AnimatedGIF gif;
File f;

int         xLoc;
int         yLoc;
int         width;
bitmap      aBitmap;

void * GIFOpenFile(char *fname, int32_t *pSize)
{
  f = SD.open(fname);
  if (f)
  {
    *pSize = f.size();
    return (void *)&f;
  }
  return NULL;
} /* GIFOpenFile() */

void GIFCloseFile(void *pHandle)
{
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
     f->close();
} /* GIFCloseFile() */

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
       return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();
    return iBytesRead;
} /* GIFReadFile() */

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{ 
  int i = micros();
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
  //Serial.printf("Seek time = %d us\n", i);
  return pFile->iPos;
} /* GIFSeekFile() */

// Draw a line of image directly on the LCD
void GIFDraw(GIFDRAW *pDraw) {

   colorObj aColor;
   byte     colorIndex;
   word     RGB656Color;
   
   if (width==0) {
      aBitmap.setSize(pDraw->iWidth,1,true);
      width = pDraw->iWidth;
      xLoc = (240-width)/2;
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
            Serial.println("Transparent");
         }
      }   
   }
   screen->blit(xLoc,yLoc+pDraw->y,&aBitmap);
}


void setup() {
   
  Serial.begin(115200);
  while (!Serial);

   xLoc = 0;  // Picture location.
   yLoc = 20;
   width = 0;  // We don't have a buffer yet.
   Serial.begin(115200);
   if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
      Serial.println("NO SCREEN!");
      Serial.flush();
      while(true);
   }
   screen->fillScreen(&black);
   SD.begin(4);
   gif.begin(LITTLE_ENDIAN_PIXELS);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("About to call gif.open");
  if (gif.open((char *)"/bender.GIF", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw))
  {
    //Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    while (gif.playFrame(true, NULL))
    {      
    }
    gif.close();
  }
  else
  {
    Serial.println("Error opening file");
    while (1)
    {};
  }
}
