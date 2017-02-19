#include <SD.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <adafruit_1947_Obj.h>
#include <bmpPipe.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <screen.h>
#include "bmpLabel.h"

#define TEXT_SIZE   2
#define LEFT_EDGE  36
#define BASELINE1  127
#define BASELINE2  237
#define PRECISION  1
#define TEXT_WIDTH  91
#define TEXT_HEIGHT 14

#define vacummPin A12
  
bmpLabel vOutVac(LEFT_EDGE,BASELINE1,TEXT_WIDTH,TEXT_HEIGHT,"-000.0");
bmpLabel vOutLeak(LEFT_EDGE,BASELINE2,TEXT_WIDTH,TEXT_HEIGHT,"-000.0");

mapper data2voltMapper(0, 1023, 0, 6.6);   // Update for 3.2V chip
mapper volt2psiMapper(0.2,4.5,-16.7,0);
mapper psi2hgMapper(-14.734624,2.2,30,-4.4792455);

float lastRead = 0;

timeObj  hgTimer(100);
timeObj  ldTimer(500);

char lastHg[20] = "";
char lastLeak[20] = "";

void setup(void) {
 
  colorObj  labelColor;
  
  //Serial.begin(9600);while(!Serial);
  Serial.println("init Screen..");
  if (initScreen(ADAFRUIT_1947,PORTRAIT,ADAFRUIT_1947_SPI_SD_CS)) {
    Serial.println("Success!");
    screen->fillScreen(&black);
    bmpPipe mBitmap;
    if (mBitmap.openPipe("vacPnl.bmp")) {
      mBitmap.drawBitmap(0,0);
    }
  } else {
    Serial.println("Fail! Halting..");
     while(true);
  }

    labelColor.setColor(&green);
    labelColor.blend(&black,40);
    vOutVac.setTextSize(TEXT_SIZE);
    vOutVac.setJustify(TEXT_RIGHT);
    vOutVac.setPrecision(PRECISION);
    vOutVac.setColors(&labelColor);
    vOutVac.openPipe("vacPnl.bmp");
    viewList.addObj(&vOutVac);
    //screen->drawRect(LEFT_EDGE,BASELINE1,TEXT_WIDTH,TEXT_HEIGHT,&red);
    
    vOutLeak.setTextSize(TEXT_SIZE);
    vOutLeak.setJustify(TEXT_RIGHT);
    vOutLeak.setPrecision(PRECISION);
    vOutLeak.setColors(&labelColor);
    vOutLeak.openPipe("vacPnl.bmp");
    viewList.addObj(&vOutLeak);
    //screen->drawRect(LEFT_EDGE,BASELINE2,TEXT_WIDTH,TEXT_HEIGHT,&red);
    
    hgTimer.start();
    ldTimer.start();
}


void readings(void) {

  int sensorValue;
  float volts;
  float psi;
  float inHg;
  float leakdown;
  char textBuff[20];
  
  sensorValue = analogRead(vacummPin);
  sensorValue = analogRead(vacummPin);
  volts = data2voltMapper.Map(sensorValue);
  psi = volt2psiMapper.Map(volts);
  inHg = psi2hgMapper.Map(psi);
  if (hgTimer.ding()) {
    dtostrf(round(inHg*10)/10.0,0,1,textBuff);
    if (strcmp(textBuff,lastHg)) {
      vOutVac.setValue(textBuff);
      strcpy(lastHg,textBuff);
    }
    hgTimer.stepTime();
  }
  if (ldTimer.ding()) {
    ldTimer.stepTime();
    leakdown = (lastRead - inHg) * 120.0;
    Serial.print("leakdown : ");Serial.println(leakdown,10);
    if (leakdown>999.9) {
      vOutLeak.setValue("+++.+");
      strcpy(lastLeak,"+++.+");
    } 
    else if (leakdown<-999.99) {
      vOutLeak.setValue("---.-");
      strcpy(lastLeak,"---.-");
    } 
    else {
      dtostrf(round(leakdown*10)/10.0,0,1,textBuff);
      if (strcmp(textBuff,lastLeak)) {
        vOutLeak.setValue(textBuff);
        strcpy(lastLeak,textBuff);
      }
    }
    lastRead = inHg;
  }
}


void loop(void) {
  
  idle();
  readings();
}





