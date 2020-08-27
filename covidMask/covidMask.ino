#include <chainPixels.h>
#include <neoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <debug.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <squareWave.h>
#include <textBuff.h>
#include <timeObj.h>

neoPixel      bioRing(12,2);
colorMultiMap bioColorMapper;
runningAvg    smoother(20);

void setup() {

  Serial.begin(9600);
  bioRing.begin();
  bioRing.setAll(&black);
  bioRing.show();
  /*
  while(true) {
  bioRing.setAll(&blue);
  bioRing.show();
  delay(500);
  bioRing.setAll(&red);
  bioRing.show();
  delay(500);
  }
  */
  bioColorMapper.addColor(948,&red);
  bioColorMapper.addColor(947,&blue);
}

void loop() {
  
  colorObj  bioColor;
  float       vacVal;
  
  vacVal = analogRead(A5);
  vacVal = smoother.addData(vacVal);
  Serial.println(vacVal);
  bioColor = bioColorMapper.Map(vacVal);
  bioRing.setAll(&bioColor);
  bioRing.show();
  delay(100);
}
