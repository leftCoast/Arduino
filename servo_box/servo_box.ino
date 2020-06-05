#include "screen.h"
#include "adafruit_1431_Obj.h"
#include "idlers.h"
#include "colorRect.h"
#include "runningAvg.h"
#include "servo.h"
#include "label.h"

#define OLED_CS   10
#define SD_CS     4
#define OLED_RST  6
#define POT_BTN   2
#define POT_PIN   A0
#define SERVO_PIN 22

#define RUN_X     20
#define RUN_Y     100
#define UNIT_X    RUN_X + 75

#define S1_X     RUN_X
#define S1_Y     RUN_Y - 50

#define S2_X     RUN_X
#define S2_Y     S1_Y - 30

mapper      percentMap(1023,0,0,100);
runningAvg  smoother(20);
servo       aServo(SERVO_PIN);
label       runningVal("---.--",2);
label       units("%",2);
label       unitsS1("%",2);
label       unitsS2("%",2);
label       saved1("---.--",2);
label       saved2("---.--",2);


void setup() {
   if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
      while (1);
   }
   screen->fillScreen(&black);
   pinMode(POT_BTN, INPUT_PULLUP);
   
   runningVal.setLocation(RUN_X,RUN_Y);
   runningVal.setJustify(TEXT_RIGHT);
   runningVal.setPrecision(2);
   runningVal.setColors(&red,&black);
   viewList.addObj(&runningVal);

   units.setLocation(UNIT_X,RUN_Y);
   units.setJustify(TEXT_LEFT);
   units.setColors(&red,&black);
   viewList.addObj(&units);

   saved1.setLocation(S1_X,S1_Y);
   saved1.setJustify(TEXT_RIGHT);
   saved1.setPrecision(2);
   saved1.setColors(&red,&black);
   viewList.addObj(&saved1);

   unitsS1.setLocation(UNIT_X,S1_Y);
   unitsS1.setJustify(TEXT_LEFT);
   unitsS1.setColors(&red,&black);
   viewList.addObj(&unitsS1);

   saved2.setLocation(S1_X,S2_Y);
   saved2.setJustify(TEXT_RIGHT);
   saved2.setPrecision(2);
   saved2.setColors(&red,&black);
   viewList.addObj(&saved2);

   unitsS2.setLocation(UNIT_X,S2_Y);
   unitsS2.setJustify(TEXT_LEFT);
   unitsS2.setColors(&red,&black);
   viewList.addObj(&unitsS2);

}

void loop() {

  int       rawPotVal;
  float     percent;
  
  idle();         // Idlers get their time.
  rawPotVal = analogRead(POT_PIN);
  percent = percentMap.Map(rawPotVal);
  percent = smoother.addData(percent);
  aServo.setServo(percent);
  if (!digitalRead(POT_BTN)) {
      saved2.setValue(saved1.buff);
      saved1.setValue(percent);
      while(!digitalRead(POT_BTN));
  }
  runningVal.setValue(percent);
}
