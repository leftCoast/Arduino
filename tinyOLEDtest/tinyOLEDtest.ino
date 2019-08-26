#include <adafruit_1947_Obj.h>
#include <Adafruit_GFX.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <drawObj.h>
#include <screen.h>
#include <label.h>


class percent : public label {

  public:
                percent(int x,int y);
  virtual       ~percent(void);

          void  setPercent(float percent);
};

percent::percent(int x,int y)
  : label(x,y,96-x,8," - ",2) {
  setColors(&white); 
}


percent::~percent(void) { }


void percent::setPercent(float percent) {

  int   intPercent;
  char  pText[6];
  
  if (percent<0) {                         
    strcpy(pText," - ");
  } else {
    intPercent = round(percent);          // Comes in as a percent float.
    if (intPercent<10) {                       // If its 1-9..
      snprintf (wText,5,"  %d",wTime);    // put in two spaces.
    } else if (wTime<100) {               // Else if its less than 100..               
      snprintf (wText,5," %d",wTime);     // One space.
    } else {                              // Else.. (>99) 
      snprintf (wText,5,"%d",wTime);      // No spaces.
    }
  }
}




// Globals

float     moisture;                             // Calculated moisture %
float     tempC;                                // Temp.
uint16_t  capread;                              // Capacitence.
enum      weDo;
weDo      weAre;                                // Current state.



void setup() {
  
  if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
    while(true);
  }
  screen->fillRect(0,0,240,360,&black);
  screen->drawLine(0,65,97,65,&yellow);
  screen->drawLine(97,0,97,65,&yellow);
  
  int yPos = 0;
  
  label* readings = new label(5,yPos,96,16,"100/100",2);
  readings->setColors(&white);
  viewList.addObj(readings);

  yPos = yPos + 20;
  label* key = new label(9,yPos,96,16,"Limit/Reading",1);
  key->setColors(&white);
  viewList.addObj(key);

  yPos = yPos + 20;                      //"100/100"
  label* statText = new label(0,yPos,96,16,"Watering",2);
  statText->setColors(&white);
  viewList.addObj(statText);
}

"Starting" 
"Watering"
"Reading"
"Soaking"
void loop() {
  idle();

}
