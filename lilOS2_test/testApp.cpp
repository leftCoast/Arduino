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
#define BUBBLE_SX    BOUND_CX - BUBBLE_RAD
#define BUBBLE_SY    BOUND_CY - BUBBLE_RAD

#define DIST_LIMIT   BOUND_DIA/2 - BUBBLE_RAD

  
grid::grid(int centerX,int centerY)
   :drawObj(BOUND_X,BOUND_Y,BOUND_DIA,BOUND_DIA) {

   center.x = centerX;
   center.y = centerY;
   gridColor.setColor(LC_CYAN);
}


grid::~grid(void) { }


void grid::setColor(colorObj* inColor) { gridColor.setColor(inColor); }


void grid::drawSelf(void) {

   screen->drawCircle(BOUND_X,BOUND_Y,BOUND_DIA,&gridColor);
   screen->drawCircle(TARGET_X,TARGET_Y,TARGET_DIA,&gridColor);
   screen->drawHLine(BOUND_X,BOUND_CY,(BOUND_DIA-TARGET_DIA)/2,&gridColor);
   screen->drawHLine(BOUND_CX+(TARGET_DIA/2),BOUND_CY,(BOUND_DIA-TARGET_DIA)/2,&gridColor);
   screen->drawVLine(BOUND_CX,BOUND_Y,(BOUND_DIA-TARGET_DIA)/2,&gridColor);
   screen->drawVLine(BOUND_CX,BOUND_CY+(TARGET_DIA/2),(BOUND_DIA-TARGET_DIA)/2,&gridColor);
}
   

bubble::bubble(int startX,int startY,grid* inGrid)
   : drawObj(startX,startY,BUBBLE_RAD*2,BUBBLE_RAD*2) {
   
   lastX    = startX;
   lastY    = startY;
   ourGrid  = inGrid;
}

   
bubble::~bubble(void) {  }

void bubble::eraseSelf(void) { screen->fillCircle(lastX,lastY,width,&black); }

void bubble::drawSelf(void) {

   screen->fillCircle(x,y,width,&red);
   screen->fillCircle(x+4,y+4,width-8,&black);
   ourGrid->setNeedRefresh();
}


   
setAngleBtn::setAngleBtn(int xLoc,int yLoc,const char* path,testApp* inApp)
   :iconButton(xLoc,yLoc,path) { ourApp = inApp; }


setAngleBtn::~setAngleBtn(void) {  }


void setAngleBtn::doAction(void) { ourApp->setOffsets(); }







testApp::testApp(lilOS* ourOS,int ourAppID)
   : panel(ourAppID) {
      
   colorObj mixColor;

   bubbleTimer.setTime(BUBBLE_MS);
   center.x = BOUND_CX;
   center.y = BOUND_CY;
   gridMap.addColor(0,&magenta);
   mixColor.setColor(&white);
   mixColor.blend(&black,40);
   gridMap.addColor(2,&white);
   mixColor.setColor(&yellow);
   mixColor.blend(&black,30);
   gridMap.addColor(10,&mixColor);
   mixColor.setColor(&blue);
   mixColor.blend(&black,50);
   gridMap.addColor(30,&mixColor);
   bno = new Adafruit_BNO055(55, 0x28);
   offsetX = 0;
   offsetY = 0; 
}

 
testApp::~testApp(void) {

   if (bno) delete(bno);
}


void testApp::setup(void) {

   if (!bno->begin()) {
      Serial.print("No BNO055 detected");
   } else {
      displaySensorDetails();
      bno->setExtCrystalUse(true);
      sinMult = (BOUND_DIA/2.0)-(BUBBLE_RAD);
   }
   theGrid = new grid(BOUND_CX,BOUND_CY);
   theBubble = new bubble(BUBBLE_SX,BUBBLE_SY,theGrid);
   addObj(theBubble);
   addObj(theGrid);
   setAngleBtn* ourAngleBtn = new setAngleBtn(20,280,iconPath(mPanelID,"crossHr.bmp"),this);
   ourAngleBtn->setMask(&(ourOSPtr->icon32Mask)); 
   addObj(ourAngleBtn);
}


char* testApp::iconPath(int appID,const char* iconName) {

   strcpy(pathBuff,ourOSPtr->getPanelFolder(appID));
   strcat(pathBuff,iconName);
   return pathBuff; 
}


void testApp::setOffsets(void) {
   
   sensors_event_t   event;
  
   bno->getEvent(&event);
   offsetX = -round(sin(theCalc.deg_2_rad(event.orientation.z))*sinMult);
   offsetY = round(sin(theCalc.deg_2_rad(event.orientation.y))*sinMult);
}


void  testApp::displaySensorDetails(void) {
  
  sensor_t sensor;
  
  bno->getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
}


void testApp::loop(void) {  

   point             newLoc;
   point             touchPt;
   float             dist;
   colorObj          gridColor;
   sensors_event_t   event;
  
   if (bubbleTimer.ding()) {
      bubbleTimer.stepTime();
      bno->getEvent(&event);
      touchPt = center;
      touchPt.x = touchPt.x - round(sin(theCalc.deg_2_rad(event.orientation.z))*sinMult);
      touchPt.y = touchPt.y + round(sin(theCalc.deg_2_rad(event.orientation.y))*sinMult);
      touchPt.x = touchPt.x - offsetX;
      touchPt.y = touchPt.y - offsetY;
      dist = distance(center,touchPt);
      if (dist<=DIST_LIMIT) {
         newLoc.x = touchPt.x - BUBBLE_RAD;
         newLoc.y = touchPt.y - BUBBLE_RAD;
         if (newLoc.x!=oldLoc.x||newLoc.y!=oldLoc.y) {
            theBubble->setLocation(newLoc.x,newLoc.y);
            oldLoc = newLoc;
            gridColor = gridMap.map(dist);
            theGrid->setColor(&gridColor);
         }
      }
   }
}


void testApp::drawSelf(void) {

   colorObj scrColor(LC_OLIVE);
   
   scrColor.blend(&green,20);
   screen->fillScreen(&scrColor);
   screen->fillCircle(BOUND_X,BOUND_Y,BOUND_DIA,&black);
}
