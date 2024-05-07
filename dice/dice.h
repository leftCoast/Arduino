#ifndef dice_h
#define dice_h

#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <idlers.h>
#include <timeObj.h>

#define FRAME_MS  20

class dice :   public Adafruit_8x8matrix,
               public idler {

   public :
            dice(void);
   virtual  ~dice(void);

            void  begin(uint8_t ourI2CAddr);
            void  clearDie(void);
            void  drawBmp(const uint8_t* bmp);
            void  doFuzz(float fuzzMs=-1);
            void  showNum(int num,float showMs=-1);
            bool  isWaiting(void);
   virtual  void  idle(void);
   
   protected :

            void clearSratch(void);
            void orBMaps(const uint8_t* bmp1,const uint8_t* bmp2);
      
            enum     states { prestart, waiting, fuzzing, showing };
            states   ourState;
            timeObj  stateTimer;
            timeObj  frameTimer;
            uint8_t  scratch[8];
            uint8_t  bmpA[8] = {
               B00000000,
               B00000000,
               B00000000,
               B00011000,
               B00011000,
               B00000000,
               B00000000,
               B00000000
            };
            uint8_t bmpB[8] = {
               B11000000,
               B11000000,
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B00000011,
               B00000011
            };
            uint8_t bmpC[8] = {
               B00000011,
               B00000011,
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B11000000,
               B11000000
            };
            uint8_t bmpD[8] = {
               B00000000,
               B00000000,
               B00000000,
               B11000011,
               B11000011,
               B00000000,
               B00000000,
               B00000000
            };
            uint8_t bmpE[8] = {
               B10000001,
               B01000010,
               B00100100,
               B00011000,
               B00011000,
               B00100100,
               B01000010,
               B10000001
            };
};

#endif
