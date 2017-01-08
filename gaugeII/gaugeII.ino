#include <SD.h>
#include <SPI.h>
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

#include <adafruit_1431_Obj.h>
#include <adafruit_1947_Obj.h>
#include <bmpObj.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <screen.h>


#define NUM_SIZE   2
#define LEFT_EDGE  45
#define BASELINE1  70
#define BASELINE2  163
#define PRECISION  1

#define HOT_OFF_X 39
#define HOT_OFF_Y 248
#define HOT_OFF_W 47
#define HOT_OFF_H 45

#define HOT_ON_X 100
#define HOT_ON_Y 248
#define HOT_ON_W 47
#define HOT_ON_H 45

#define vacummPin A4
#define pumpPin   2      // Now we know.. 

label vOutVac("-000.0",NUM_SIZE);
label vOutLeak("-000.0",NUM_SIZE);

mapper data2voltMapper(0, 1024, 0.0049, 5);
mapper volt2psiMapper(0.2,4.5,-16.7,0);
mapper psi2hgMapper(-14.734624,2.2,30,-4.4792455);

float lastRead = 0;
timeObj  mTimer(500);
rect    mHotSpotOff(HOT_OFF_X,HOT_OFF_Y,HOT_OFF_W,HOT_OFF_H);
rect    mHotSpotOn(HOT_ON_X,HOT_ON_Y,HOT_ON_W,HOT_ON_H);
drawObj mObject;

void setup(void) {

//TSPoint inDest;
point inDest;

  Serial.begin(9600);
  pinMode(pumpPin,OUTPUT);      // Whatever happens, shut off the pump.
  digitalWrite(pumpPin,HIGH);
  Serial.println("ini Screen..");
  if (initScreen(ADAFRUIT_1947,INV_PORTRAIT)) {
    Serial.print("Success!");
    screen->fillScreen(&black);
    if (initSDCard()) {
      inDest.x = 0;
      inDest.y = 0;
      bmpObj mBitmap("panel.bmp",inDest);
      //bmpObj mBitmap("on.bmp",inDest);
      mBitmap.getInfo();
      if (mBitmap.haveInfo){
        mBitmap.plotBmp();
      } else {
       Serial.println("No bitmap!");
      } 
    } else {
      Serial.println("No SD card!");
    }
  } else {
    Serial.println("Fail! Halting..");
     while(true);
  }

    vOutVac.setLocation(LEFT_EDGE,BASELINE1);
    vOutVac.setJustify(TEXT_RIGHT);
    vOutVac.setPrecision(PRECISION);

    vOutLeak.setLocation(LEFT_EDGE,BASELINE2);
    vOutLeak.setJustify(TEXT_RIGHT);
    vOutLeak.setPrecision(PRECISION);


    //mObject.setLocation(HOT_OFF_X,HOT_OFF_Y);
    //mObject.setSize(HOT_OFF_W,HOT_OFF_H);
    //mObject.setLocation(HOT_ON_X,HOT_ON_Y);
    //mObject.setSize(HOT_ON_W,HOT_ON_H);
    //mObject.draw();

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
  vOutVac.draw();
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
    vOutLeak.draw();
    lastRead = inHg;
  }
}


void loop(void) {
  point inPt;

  readings();
  inPt = screen->getPoint();
  if (screen->touched()) {
    if (mHotSpotOff.inRect(inPt.x,inPt.y)) {
      digitalWrite(pumpPin,HIGH);
      // screen->frameRectInvert(mHotSpotOff.location,mHotSpotOff.width,mHotSpotOff.height); // Some day maybe I will be able to write this
      delay(200);
      // screen->frameRectInvert(mHotSpotOff.location,mHotSpotOff.width,mHotSpotOff.height); // Some day maybe I will be able to write this.
    } 
    else if (mHotSpotOn.inRect(inPt.x,inPt.y)) { 
      digitalWrite(pumpPin,LOW);
      // screen->frameRectInvert(mHotSpotOn.location,mHotSpotOn.width,mHotSpotOn.height); // Some day maybe I will be able to write this.
      delay(200);
      //screen->frameRectInvert(mHotSpotOn.location,mHotSpotOn.width,mHotSpotOn.height);  // Some day maybe I will be able to write this.
    }    
  }
}




