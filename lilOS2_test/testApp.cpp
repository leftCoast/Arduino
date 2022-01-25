#include "testApp.h"

#define BOUND_X      20
#define BOUND_Y      40
#define BOUND_DIA    200
#define BOUND_CX     BOUND_DIA/2+BOUND_X
#define BOUND_CY     BOUND_DIA/2+BOUND_Y

#define TARGET_DIA   40
#define TARGET_X     BOUND_CX - TARGET_DIA/2
#define TARGET_Y     BOUND_CY - TARGET_DIA/2

#define BUBBLE_RAD   16
#define BUBBLE_MS    40

bubble::bubble(void)
   : drawObj(120-BUBBLE_RAD,BOUND_CY-BUBBLE_RAD,BUBBLE_RAD*2,BUBBLE_RAD*2) {  }

   
bubble::~bubble(void) {  }

void bubble::eraseSelf(void) { screen->fillCircle(lastX,lastY,width,&black); }

void bubble::drawSelf(void) { screen->fillCircle(x,y,width,&red); }



 
testApp::testApp(lilOS* ourOS,int ourAppID)
   : panel(ourAppID) { bubbleTimer.setTime(BUBBLE_MS); }

   
testApp::~testApp(void) {  }

void testApp::setup(void) {

   theBubble = new bubble();
   addObj(theBubble);
}


void testApp::loop(void) {  

  point newLoc;
  point touchPt;
  
  if (bubbleTimer.ding()) {
    bubbleTimer.stepTime();
    if(screen->touched()) {
      touchPt = screen->getPoint();
      newLoc.x = touchPt.x - BUBBLE_RAD;
      newLoc.y = touchPt.y - BUBBLE_RAD;
      if (newLoc.x!=oldLoc.x||newLoc.y!=oldLoc.y) {
        theBubble->setLocation(newLoc.x,newLoc.y);
        oldLoc = newLoc;
      }
    }
  }
}

void testApp::drawSelf(void) {

   screen->fillScreen(&green);
   screen->fillCircle(BOUND_X,BOUND_Y,BOUND_DIA,&black);
   screen->drawCircle(TARGET_X,TARGET_Y,TARGET_DIA,&blue);
   screen->drawHLine(BOUND_X,BOUND_CY,(BOUND_DIA-TARGET_DIA)/2,&blue);
   screen->drawHLine(BOUND_CX+(TARGET_DIA/2),BOUND_CY,(BOUND_DIA-TARGET_DIA)/2,&blue);
   screen->drawVLine(BOUND_CX,BOUND_Y,(BOUND_DIA-TARGET_DIA)/2,&blue);
   screen->drawVLine(BOUND_CX,BOUND_CY+(TARGET_DIA/2),(BOUND_DIA-TARGET_DIA)/2,&blue);
}
