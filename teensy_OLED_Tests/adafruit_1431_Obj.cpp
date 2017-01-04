#include <SPI.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1351.h> 
#include "adafruit_1431_Obj.h"


#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

adafruit_1431_Obj::adafruit_1431_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST)
  :displayObj(true,true,false,true) {

  theOLED = NULL;
  cs = inCS;
  dc = inDC;
  mosi = inMOSI;
  sclk = inSCLK;
  rst = inRST;
}

adafruit_1431_Obj::~adafruit_1431_Obj(void) {

    if (theOLED) { 
      delete theOLED;
      theOLED = NULL;
    }
  }

  
void adafruit_1431_Obj::begin(void) { 

  theOLED = new Adafruit_SSD1351(cs, dc, mosi, sclk, rst);
  if (theOLED!=NULL) {
    theOLED->begin();
  }
}


void adafruit_1431_Obj::setTextColor(colorObj* inColor)   { theOLED->setTextColor(inColor->getColor16()); }
void adafruit_1431_Obj::setTextSize(byte inSize)          { theOLED->setTextColor(inSize); }
void adafruit_1431_Obj::setCursor(int inX,int inY)        { theOLED->setCursor(inX,inY); }
void adafruit_1431_Obj::drawText(char* inText)            { Serial.println("In print text"); theOLED->print(inText); }


void adafruit_1431_Obj::fillScreen(colorObj* inColor)     { 

  Serial.println("In fill screen");
  //theOLED->fillScreen(inColor->getColor16());
  
  byte screenX;
  byte screenY;
  word color = inColor->getColor16();
  Serial.println("Gonna make the settings..");
  SPISettings settings(24000000, MSBFIRST, SPI_MODE3); // Teensy 3.1 max SPI
  Serial.println("Settings made, now lets NOT set them..");
  //SPI.beginTransaction(settings);
  Serial.println("Just set the settings");
   
  theOLED->writeCommand(SSD1351_CMD_SETROW);    // Y range
  theOLED->writeData(0); 
  theOLED->writeData(SCREEN_HEIGHT - 1);
  theOLED->writeCommand(SSD1351_CMD_SETCOLUMN); // X range
  theOLED->writeData(0);
  theOLED->writeData(SCREEN_WIDTH  - 1);
  theOLED->writeCommand(SSD1351_CMD_WRITERAM);  // Begin write
  Serial.println("Just sent the block of commands");

  
  digitalWrite(cs, LOW);                       // Chip select
  digitalWrite(dc, HIGH);                       // Data mode
  // Now just issue raw 16-bit values for every pixel...
  Serial.println("Chip select done. Starting loops..");
  
  for(screenY=0; screenY<SCREEN_HEIGHT; screenY++) {
    for(screenX=0; screenX<SCREEN_WIDTH; screenX++) {
      
      // SPI FIFO technique from Paul Stoffregen's ILI9341_t3 library:
      Serial.println("In loop, going to check for space..");
      while(KINETISK_SPI0.SR & 0xC000) Serial.println("Waiting for space.."); // Wait for space in FIFO
      Serial.println("They sid there was space.");
      KINETISK_SPI0.PUSHR = color | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
    }
  }
  Serial.println("Loops done, cleaning up.");
  
  KINETISK_SPI0.SR |= SPI_SR_TCF;           // Clear transfer flag
  while((KINETISK_SPI0.SR & 0xF000) ||      // Wait for SPI FIFO to drain
       !(KINETISK_SPI0.SR & SPI_SR_TCF));   // Wait for last bit out
  digitalWrite(cs, HIGH);                   // Deselect
  SPI.endTransaction();
  Serial.println("All done, exiting.");
 }
