
#include <neoPixel.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>
#include <blinker.h>

#define RED_VAL   739.0
#define BLUE_VAL  738.45
#define DARK_PERC 50

neoPixel      bioRing(12,6);
colorMultiMap bioColorMapper;
runningAvg    smoother(20);
colorObj      tColor;
//blinker       aBlinker;

void setup() {

  //Serial.begin(9600);
  bioRing.begin();
  bioRing.setAll(&black);
  bioRing.show();
  tColor.setColor(&red);
  tColor.blend(&black,DARK_PERC);
  bioColorMapper.addColor(RED_VAL,&tColor);
  tColor.setColor(&blue);
  tColor.blend(&black,DARK_PERC);
  bioColorMapper.addColor(BLUE_VAL,&tColor);
  
  tColor.setColor(&green);
  tColor.blend(&black,DARK_PERC);
  //aBlinker.setOnOff(true);
}


void loop() {
  
  colorObj  bioColor;
  float       vacVal;
  //idle();
  vacVal = analogRead(A12);
  vacVal = smoother.addData(vacVal);
  /*
  Serial.print(RED_VAL);Serial.print(",");
  Serial.print(vacVal);Serial.print(",");
  Serial.println(BLUE_VAL);
  */
  bioColor = bioColorMapper.Map(vacVal);
  bioRing.setAll(&bioColor);
  bioRing.show();
  delay(50);
}
