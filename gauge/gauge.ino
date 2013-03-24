#include <SD.h>              // Thing for SD card
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "screenObj.h"
#include "drawObj.h"
#include "label.h"
#include "timeObj.h"
#include "radioBtn.h"
#include "bmpObj.h"

#define HEADING_SIZE 2
#define NUM_SIZE 5
#define UNIT_SIZE 1
#define BASELINE1 30
#define BASELINE2 150


radioBtnSet rBtnSet;
radioBtn    startBtn;
radioBtn    stopBtn;
radioBtn    timerBtn;

label vLbl("Vacuum",HEADING_SIZE);
label vOutVal("-00.0",NUM_SIZE);
label vUnits(" in/Hg",UNIT_SIZE);

label lLbl("Leakdown",HEADING_SIZE);
label lOutVal("-00.0",NUM_SIZE);
label lUnits(" in/Hg/min",UNIT_SIZE);


colorObj background(WHITE);
colorObj textColor(BLACK);

#define vacummPin A4

mapper data2voltMapper(0, 1024, 0.0049, 5);
//mapper volt2psiMapper(0.2,4.6,-16.7,2.2);
mapper volt2psiMapper(0.2,4.5,-16.7,0);
mapper psi2hgMapper(-14.734624,2.2,30,-4.4792455);

timeObj leakTimer(250);
float readings[4];
byte index = 0;
float lastAvarage;
float lastResult;


float getleakdown(void) {
  
  float avarage;
  float result;
  
  if (leakTimer.ding()) {
    avarage = 0;
    for (int i=0;i<4;i++) {
      avarage = avarage + readings[i];
    }
    avarage = avarage / 4.0;
    result = (lastAvarage - avarage) * 4 * 60;
    lastAvarage = avarage;
    lastResult = result;
    return(result);
  } else {
    return(lastResult); 
  }
}


float readInHg(void) {

  int sensorValue;
  float volts;
  float psi;
  float inHg;

  sensorValue = analogRead(vacummPin);
  volts = data2voltMapper.Map(sensorValue);
  psi = volt2psiMapper.Map(volts);
  inHg = psi2hgMapper.Map(psi);
  readings[index] = inHg;
  index++;
  if (index>3) index = 0;
  return inHg;
}


void drawScreen(void) {
  
  screen->fillScreen(background.getColor16());

  vLbl.draw();
  vOutVal.draw();
  vUnits.draw();
  
  lLbl.draw();
  lOutVal.draw();
  lUnits.draw();
  rBtnSet.draw();
 
}

void setup(void) {
  
  // the file itself
  File bmpFile;
  Point inDest;
  //Serial.begin(9600);
  //Serial.println("Starting in setup();");
  //Serial.print("idelers list : "); Serial.println((unsigned long)&theIdlers);
  //Serial.print("touch List : "); Serial.println((unsigned long)&theTouchList);
  if (initSDCard()) {
    inDest.x = 0;
    inDest.y = 0;
    bmpFile = SD.open("panel3.bmp");
    bmpObj mBitmap(bmpFile,inDest);
    mBitmap.getInfo();
  }
  
  leakTimer.start();
  if (initScreen(PORTRAIT)) {

    theTouchList.addObj(&rBtnSet);
    rBtnSet.addObj(&startBtn);
    rBtnSet.addObj(&stopBtn);
    rBtnSet.addObj(&timerBtn);
    
    vLbl.setLocation(5,BASELINE1-vLbl.getTextHeight());
    vLbl.setColors(textColor.getColor16(),background.getColor16());
    
    vOutVal.setLocation(5,vLbl.maxY()+10);
    vOutVal.setColors(textColor.getColor16(),background.getColor16());
    vOutVal.setPrecision(1);
    vOutVal.setJustify(TEXT_RIGHT);
    
    vUnits.setLocation(vOutVal.maxX(),vOutVal.maxY()-vUnits.getTextHeight());
    vUnits.setColors(textColor.getColor16(),background.getColor16());
    
    lLbl.setLocation(5,BASELINE2-lLbl.getTextHeight());
    lLbl.setColors(textColor.getColor16(),background.getColor16());
    
    lOutVal.setLocation(5,lLbl.maxY()+10);
    lOutVal.setColors(textColor.getColor16(),background.getColor16());
    lOutVal.setPrecision(1);
    lOutVal.setJustify(TEXT_RIGHT);
    
    lUnits.setLocation(lOutVal.maxX(),lOutVal.maxY()-lUnits.getTextHeight());
    lUnits.setColors(textColor.getColor16(),background.getColor16());
    
    stopBtn.setLocation(20,lOutVal.maxY()+40);
    startBtn.setLocation(120,lOutVal.maxY()+40);
    timerBtn.setLocation(220,lOutVal.maxY()+40);
    
    drawScreen();
  }
  else
    while(1);
}


void loop() {
  //Serial.println("hello?");
  idle();
  vOutVal.setValue(readInHg());
  lOutVal.setValue(getleakdown());
  vOutVal.draw();
  lOutVal.draw();
  if (rBtnSet.getRefresh()) rBtnSet.draw();
}
 
 
 
// ****************************************
// ************************ BITMAP WORKSHOP
// ****************************************

/*

// The chip select pin for the SD card on the shield
#define SD_CS 5 
// the file itself
File bmpFile;

// information we extract about the bitmap file
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

//************* HARDWARE SPI ENABLE/DISABLE 
// we want to reuse the pins for the SD card and the TFT - to save 2 pins. this means we have to
// enable the SPI hardware interface whenever accessing the SD card and then disable it when done
int8_t saved_spimode;

void disableSPI(void) {
  saved_spimode = SPCR;
  SPCR = 0;
}

void enableSPI(void) {
  SPCR = saved_spimode; 
}
//*****************************************

void setupII()
{

  
  Serial.begin(9600);
  
  tft.reset();
  
  // find the TFT display
  uint16_t identifier = tft.readRegister(0x0);
  if (identifier == 0x9325) {
    Serial.println("Found ILI9325");
  } else if (identifier == 0x9328) {
    Serial.println("Found ILI9328");
  } else {
    Serial.print("Unknown driver chip ");
    Serial.println(identifier, HEX);
    while (1);
  }  
 
  tft.begin();
  // the image is a landscape, so get into landscape mode
  tft.setRotation(0);
  
  //Serial.print("Initializing SD card...");
 
  if (!SD.begin(SD_CS)) {
    //Serial.println("failed!");
    return;
  }
  //Serial.println("SD OK!");
  
  bmpFile = SD.open("woof.bmp");

  if (! bmpFile) {
    //Serial.println("didnt find image");
    while (1);
  }
  
  if (! bmpReadHeader(bmpFile)) { 
     //Serial.println("bad bmp");
     return;
  }
  
  //Serial.print("image size "); 
  //Serial.print(bmpWidth, DEC);
  //Serial.print(", ");
  //Serial.println(bmpHeight, DEC);
  disableSPI();    // release SPI so we can use those pins to draw
 
  bmpdraw(bmpFile, 0, 0);
  // disable the SD card interface after we are done!
  disableSPI();
}



//********************************************
// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. increading the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place

#define BUFFPIXEL 20

void bmpdraw(File f, int x, int y) {

  colorObj colorBuff;
  enableSPI();     // enable the hardware SPI to talk to the SD card
  bmpFile.seek(bmpImageoffset);
  disableSPI();    // release it so we can use those pins
  
  uint32_t time = millis();
  uint16_t p;
  uint8_t g, b;
  int i, j;
  
  uint8_t sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
  uint8_t buffidx = 3*BUFFPIXEL;
  
  //Serial.print("rotation = "); Serial.println(tft.getRotation(), DEC);
  
  for (i=0; i< bmpHeight; i++) {
    // bitmaps are stored with the BOTTOM line first so we have to move 'up'

    if (screen->getRotation() == 3) {
      screen->writeRegister16(ILI932X_ENTRY_MOD, 0x1028);
      screen->goTo(x+i, y); 
    } else if  (screen->getRotation() == 2) {
      screen->writeRegister16(ILI932X_ENTRY_MOD, 0x1020);
      screen->goTo(x+bmpWidth, y+i); 
    } else if  (screen->getRotation() == 1) {
      screen->writeRegister16(ILI932X_ENTRY_MOD, 0x1018);
      screen->goTo(x+bmpHeight-1-i, y); 
    } else if  (screen->getRotation() == 0) {
      screen->writeRegister16(ILI932X_ENTRY_MOD, 0x1030);
      screen->goTo(x, y+bmpHeight-i); 
    }
    
    for (j=0; j<bmpWidth; j++) {
      // read more pixels
      if (buffidx >= 3*BUFFPIXEL) {
        enableSPI();     // enable the hardware SPI to talk to the SD card
        bmpFile.read(sdbuffer, 3*BUFFPIXEL);
        disableSPI();    // release it so we can use those pins
        buffidx = 0;
      }
      
      
      // convert pixel from 888 to 565
      b = sdbuffer[buffidx++];     // blue
      g = sdbuffer[buffidx++];     // green
      p = sdbuffer[buffidx++];     // red
      colorBuff.setColor(p,g,b);
      
      p >>= 3;
      p <<= 6;
      
      g >>= 2;
      p |= g;
      p <<= 5;
      
      b >>= 3;
      p |= b;
     
       // write out the 16 bits of color
      screen->writeData(colorBuff.getColor16());
    }
  }
  screen->writeRegister16(ILI932X_ENTRY_MOD, 0x1030);
  //Serial.print(millis() - time, DEC);
  //Serial.println(" ms");
}

boolean bmpReadHeader(File f) {
   // read header
  uint32_t tmp;
  
  if (read16(f) != 0x4D42) {
    // magic bytes missing
    return false;
  }
 
  // read file size
  tmp = read32(f);  
  //Serial.print("size 0x"); Serial.println(tmp, HEX);
  
  // read and ignore creator bytes
  read32(f);
  
  bmpImageoffset = read32(f);  
  //Serial.print("offset "); Serial.println(bmpImageoffset, DEC);
  
  // read DIB header
  tmp = read32(f);
  //Serial.print("header size "); Serial.println(tmp, DEC);
  bmpWidth = read32(f);
  bmpHeight = read32(f);

  
  if (read16(f) != 1)
    return false;
    
  bmpDepth = read16(f);
  //Serial.print("bitdepth "); Serial.println(bmpDepth, DEC);

  if (read32(f) != 0) {
    // compression not supported!
    return false;
  }
  
  //Serial.print("compression "); Serial.println(tmp, DEC);

  return true;
}

//*********************************************

// These read data from the SD card file and convert them to big endian 
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f) {
  uint16_t d;
  uint8_t b;
  b = f.read();
  d = f.read();
  d <<= 8;
  d |= b;
  return d;
}


// LITTLE ENDIAN!
uint32_t read32(File f) {
  uint32_t d;
  uint16_t b;
 
  b = read16(f);
  d = read16(f);
  d <<= 16;
  d |= b;
  return d;
}

*/






