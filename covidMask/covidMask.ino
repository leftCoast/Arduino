
#include <neoPixel.h>


#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>

#define RED_VAL   738.3
#define BLUE_VAL  737.8


neoPixel      bioRing(12,2);
colorMultiMap bioColorMapper;
runningAvg    smoother(40);

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
  bioColorMapper.addColor(RED_VAL,&red);
  bioColorMapper.addColor(BLUE_VAL,&blue);
  //Serial.println("ready");
  //delay(500);
}


void loop() {
  
  colorObj  bioColor;
  float       vacVal;
  
  vacVal = analogRead(A12);
  vacVal = smoother.addData(vacVal);
  Serial.print(RED_VAL);Serial.print(",");
  Serial.print(vacVal);Serial.print(",");
  Serial.println(BLUE_VAL);
  bioColor = bioColorMapper.Map(vacVal);
  bioRing.setAll(&bioColor);
  bioRing.show();
  delay(100);
}
