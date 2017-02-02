#include <SD.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <servo.h>
#include <timeObj.h>

#include <adafruit_1947_Obj.h>
#include <bmpObj.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <screen.h>


#define TEXT_SIZE   2
#define LEFT_EDGE  36
#define BASELINE1  127
#define BASELINE2  237
#define PRECISION  1
#define TEXT_WIDTH  91
#define TEXT_HEIGHT 14

#define vacummPin A4

//label vOutVac("-000.0",NUM_SIZE);
label vOutVac(LEFT_EDGE,BASELINE1,TEXT_WIDTH,TEXT_HEIGHT,"-000.0");
//label vOutLeak("-000.0",NUM_SIZE);
label vOutLeak(LEFT_EDGE,BASELINE2,TEXT_WIDTH,TEXT_HEIGHT,"-000.0");
colorObj  labelColor(GREEN);

mapper data2voltMapper(0, 1024, 0.0049, 5);   // Update for 3.2V chip
mapper volt2psiMapper(0.2,4.5,-16.7,0);
mapper psi2hgMapper(-14.734624,2.2,30,-4.4792455);

float lastRead = 0;
timeObj  mTimer(500);

void setup(void) {
  
  point destPt;
  
  //Serial.begin(9600);while(!Serial);
  Serial.println("init Screen..");
  if (initScreen(ADAFRUIT_1947,PORTRAIT)) {
    Serial.println("Success!");
    screen->fillScreen(&black);
    destPt.x = 0;
    destPt.y = 0;
    bmpObj mBitmap("vacPnl.bmp",destPt);
    mBitmap.getInfo();
    if (mBitmap.haveInfo){
      Serial.println("Found bitmap, plotting..");
      //mBitmap.outputInfo();
      mBitmap.plotBmp();
      Serial.println("Done plotting..");
    } else {
     Serial.println("No bitmap!");
    } 
  } else {
    Serial.println("Fail! Halting..");
     while(true);
  }

    labelColor.blend(&black,30);
    vOutVac.setTextSize(TEXT_SIZE);
    vOutVac.setJustify(TEXT_RIGHT);
    vOutVac.setPrecision(PRECISION);
    vOutVac.setColors(&labelColor);
    viewList.addObj(&vOutVac);
    screen->drawRect(LEFT_EDGE,BASELINE1,TEXT_WIDTH,TEXT_HEIGHT,&red);
    
    vOutLeak.setTextSize(TEXT_SIZE);
    vOutLeak.setJustify(TEXT_RIGHT);
    vOutLeak.setPrecision(PRECISION);
    vOutLeak.setColors(&labelColor);
    viewList.addObj(&vOutLeak);
    //screen->drawRect(LEFT_EDGE,BASELINE2,TEXT_WIDTH,TEXT_HEIGHT,&red);
    
    mTimer.start();
}


void readings(void) {

  int sensorValue;
  float volts;
  float psi;
  float inHg;
  float leakdown;

  sensorValue = analogRead(vacummPin);
  volts = data2voltMapper.Map(sensorValue);
  psi = volt2psiMapper.Map(volts);
  inHg = psi2hgMapper.Map(psi);
  vOutVac.setValue(inHg);
  if (mTimer.ding()) {
    mTimer.stepTime();
    leakdown = (lastRead - inHg) * 120.0;
    if (leakdown>999.9) {
      vOutLeak.setValue("+++.+");
    } 
    else if (leakdown<-999.99) {
      vOutLeak.setValue("---.-");
    } 
    else {
      vOutLeak.setValue(leakdown);
    }
    lastRead = inHg;
  }
}


void loop(void) {
  
  idle();
  readings();
}
