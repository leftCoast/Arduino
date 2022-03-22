#include "testApp.h"
#include <runningAvg.h>
#include <colorRect.h>
#include <label.h>
#include <triDRender.h>
#include <facetList.h>
#include <stlList.h>
#include <arrayList.h>
#include <resizeBuff.h>

#include <debug.h>


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

#define TXT_H        8
#define LINE_H       10
#define LBL_X        60
#define LBL_Y_BEGIN  240
#define LBL_W        70
#define VAL_X        LBL_X + LBL_W + 10
#define VAL_W        40
#define VAL2_X       VAL_X + VAL_W + 10



//****************************************************************************************
// grid:
// 
// The cross hair thing.
//****************************************************************************************


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


   
//****************************************************************************************
// bubble:
// 
// The thing that floats about under the grid
//****************************************************************************************


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



//****************************************************************************************
// setAngleBtn:
// 
// The thing the user touches to set "this" angle as zero.
//****************************************************************************************   


setAngleBtn::setAngleBtn(int xLoc,int yLoc,const char* path,testApp* inApp)
   :iconButton(xLoc,yLoc,path) { ourApp = inApp; }


setAngleBtn::~setAngleBtn(void) {  }


void setAngleBtn::doAction(void) { ourApp->setOffsets(true); }



//****************************************************************************************
// clearAngleBtn:
// 
// The thing the user touches to set ground angle back to zero.
//****************************************************************************************


clearAngleBtn::clearAngleBtn(int xLoc,int yLoc,const char* path,testApp* inApp)
   :iconButton(xLoc,yLoc,path) { ourApp = inApp; }


clearAngleBtn::~clearAngleBtn(void) {  }


void clearAngleBtn::doAction(void) { ourApp->setOffsets(false); }



//****************************************************************************************
// The testApp:
// 
// The thing that holds everyhting together.
//****************************************************************************************


runningAvg smootherX(5);
runningAvg smootherY(5);

float rad_2_deg(float x) { return x*180.0/PI; }
float deg_2_rad(double x) { return (x*PI/180.0); }


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
   scrColor.setColor(LC_OLIVE);
   scrColor.blend(&green,20);
   offsetX = 0;
   offsetY = 0;
   offsetZ = 0; 
}

 
testApp::~testApp(void) {

   if (bno) delete(bno);
}


triDTriangle* createCircleThing(int slices,double coneAngle,double radius) {
   

   double         slice;
   double         twoPi;
   triDVector     vectI;
   triDVector     vectA;
   triDTriangle*  facets;
   triDRotation   rotation;
   
   facets = NULL;
   resizeBuff(sizeof(triDTriangle)*slices,(byte**)&facets);
   twoPi = 2*M_PI;
   slice = twoPi/(float)slices;
   Serial.println(slice,5);
   vectI.setVector(0,0,radius);
   rotation = setRotation(0,coneAngle,0);
   vectI.rotateVect(&rotation);
   for (int i=0;i<slices;i++) {
      
      facets[i].corners[0].x = 0;                     // 
      facets[i].corners[0].y = 0;
      facets[i].corners[0].z = radius;
                
      vectA.setVector(&vectI);                     // Initial vector.
      rotation = setRotation(0,0,i*slice);      // How far we'll need to shift the vector..
      vectA.rotateVect(&rotation);                 // Add this much to the rotation.
      facets[i].corners[2].x = vectA.getX();
      facets[i].corners[2].y = vectA.getY();
      facets[i].corners[2].z = vectA.getZ();
      
      rotation = setRotation(0,0,slice);  // We've aleready added i*slice. We need one more slice.
      vectA.rotateVect(&rotation);
      facets[i].corners[1].x = vectA.getX();
      facets[i].corners[1].y = vectA.getY();
      facets[i].corners[1].z = vectA.getZ();

      //printTriDTriangle(&(facets[i]));
   }
   return facets;
}





void testApp::setup(void) {

   int      yVal;
   int      offset;
   label*   aLabel;

   twoDPoint   centerPt;
   twoDPoint   initPt;
   twoDPoint   tracePt;
   float       radians;
   float       twoPi = 2*M_PI;

//   int         slices = 24;
//   centerPt.x = 120;
//   centerPt.y = 200;
//   
//   initPt.x = centerPt.x;
//   initPt.y = centerPt.y - 50;
//   
//   screen->drawCircle(round(centerPt.x),round(centerPt.y),5,&red);
//   for (int i=0;i<slices;i++) {
//      radians = i * (twoPi/slices);
//      tracePt = initPt;
//      rotate(&tracePt,radians,&centerPt);
//      screen->drawCircle(round(tracePt.x),round(tracePt.y),5,&blue);
//      db.trace("Slice: ",i,true);
//   }
   
     
   if (!bno->begin()) {
      Serial.print("No BNO055 detected");
   } else {
      bno->setExtCrystalUse(true);
      sinMult = (BOUND_DIA/2.0)-(BUBBLE_RAD);
   }
   
   
   triDRotation angle;
   triDPoint   location;
   triDPoint   center;
   triDRender* renderMan = new triDRender(20,100,180,180);
   
//   arrayList* arrayModel;
//   triDTriangle* theModel = createCircleThing(12,deg_2_rad(15),100);
//   arrayModel = new arrayList(theModel,12);
//   renderMan->begin(arrayModel);
   
   stlList* STLModel;
   STLModel = new stlList("/Test3D.STL");
//   //STLModel = new stlList("/teensyM.STL");
//   //STLModel = new stlList("/hemi.STL");
   center = STLModel->getModelCenter();
   renderMan->begin(STLModel);
   renderMan->setObjScale(5);
   angle.xRad = deg_2_rad(0);
   angle.yRad = deg_2_rad(180);
   angle.zRad = deg_2_rad(0);
   renderMan->setObjAngle(&angle);
   location.x = 0;
   location.y = 0;
   location.z = 30;
   renderMan->setObjLoc(&location);
   addObj(renderMan);
   
   /*
   theGrid = new grid(BOUND_CX,BOUND_CY);
   theBubble = new bubble(BUBBLE_SX,BUBBLE_SY,theGrid);
   addObj(theBubble);
   addObj(theGrid);

   clearAngleBtn* ourCAngleBtn = new clearAngleBtn(20,240,iconPath(mPanelID,"globeCH.bmp"),this);
   ourCAngleBtn->setMask(&(ourOSPtr->icon32Mask)); 
   addObj(ourCAngleBtn);
   
   setAngleBtn* ourSAngleBtn = new setAngleBtn(20,280,iconPath(mPanelID,"crossHr.bmp"),this);
   ourSAngleBtn->setMask(&(ourOSPtr->icon32Mask)); 
   addObj(ourSAngleBtn);

   yVal     = LBL_Y_BEGIN;
   offset   = LINE_H;

   aLabel = new label(VAL_X,yVal,VAL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Abs.");
   addObj(aLabel);

   aLabel = new label(VAL2_X,yVal,VAL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Rel.");
   addObj(aLabel);
   
   yVal = yVal + offset;
   
   aLabel = new label(LBL_X,yVal,LBL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Angle x :");
   addObj(aLabel);

   xLabel = new label(VAL_X,yVal,VAL_W,TXT_H);
   xLabel->setColors(&cyan,&scrColor);
   xLabel->setJustify(TEXT_RIGHT);
   xLabel->setPrecision(1);
   xLabel->setValue(0);
   addObj(xLabel);

   x2Label = new label(VAL2_X,yVal,VAL_W,TXT_H);
   x2Label->setColors(&cyan,&scrColor);
   x2Label->setJustify(TEXT_RIGHT);
   x2Label->setPrecision(1);
   x2Label->setValue(0);
   addObj(x2Label);

   yVal = yVal + offset;

   aLabel = new label(LBL_X,yVal,LBL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Angle y :");
   addObj(aLabel);

   yLabel = new label(VAL_X,yVal,VAL_W,TXT_H);
   yLabel->setColors(&cyan,&scrColor);
   yLabel->setJustify(TEXT_RIGHT);
   yLabel->setPrecision(1);
   yLabel->setValue(0);
   addObj(yLabel);

   y2Label = new label(VAL2_X,yVal,VAL_W,TXT_H);
   y2Label->setColors(&cyan,&scrColor);
   y2Label->setJustify(TEXT_RIGHT);
   y2Label->setPrecision(1);
   y2Label->setValue(0);
   addObj(y2Label);

   yVal = yVal + offset;

   aLabel = new label(LBL_X,yVal,LBL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Angle z :");
   addObj(aLabel);

   zLabel = new label(VAL_X,yVal,VAL_W,TXT_H);
   zLabel->setColors(&cyan,&scrColor);
   zLabel->setJustify(TEXT_RIGHT);
   zLabel->setPrecision(1);
   zLabel->setValue(0);
   addObj(zLabel);

   z2Label = new label(VAL2_X,yVal,VAL_W,TXT_H);
   z2Label->setColors(&cyan,&scrColor);
   z2Label->setJustify(TEXT_RIGHT);
   z2Label->setPrecision(1);
   z2Label->setValue(0);
   addObj(z2Label);

   yVal = yVal + offset;

   aLabel = new label(LBL_X,yVal,LBL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Offset x :");
   addObj(aLabel);

   offsetXLabel = new label(VAL_X,yVal,VAL_W,TXT_H);
   offsetXLabel->setColors(&cyan,&scrColor);
   offsetXLabel->setJustify(TEXT_RIGHT);
   offsetXLabel->setPrecision(1);
   offsetXLabel->setValue(offsetX);
   addObj(offsetXLabel);

    yVal = yVal + offset;

   aLabel = new label(LBL_X,yVal,LBL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Offset y :");
   addObj(aLabel);

   offsetYLabel = new label(VAL_X,yVal,VAL_W,TXT_H);
   offsetYLabel->setColors(&cyan,&scrColor);
   offsetYLabel->setJustify(TEXT_RIGHT);
   offsetYLabel->setPrecision(1);
   offsetYLabel->setValue(offsetY);
   addObj(offsetYLabel);

   yVal = yVal + offset;

   aLabel = new label(LBL_X,yVal,LBL_W,TXT_H);
   aLabel->setColors(&cyan,&scrColor);
   aLabel->setJustify(TEXT_RIGHT);
   aLabel->setValue("Offset z :");
   addObj(aLabel);

   offsetZLabel = new label(VAL_X,yVal,VAL_W,TXT_H);
   offsetZLabel->setColors(&cyan,&scrColor);
   offsetZLabel->setJustify(TEXT_RIGHT);
   offsetZLabel->setPrecision(1);
   offsetZLabel->setValue(offsetZ);
   addObj(offsetZLabel);
   */
}


void testApp::showValues(float x,float y,float z) {
   /*
   xLabel->setValue(x);
   yLabel->setValue(y);
   zLabel->setValue(z);
   x2Label->setValue(x-offsetX);
   y2Label->setValue(y-offsetY);
   z2Label->setValue(z-offsetZ);
   */
}


void testApp::showOffsets(float x,float y,float z) {
   /*
   offsetXLabel->setValue(x);
   offsetYLabel->setValue(y);
   offsetZLabel->setValue(z);
   */
}


char* testApp::iconPath(int appID,const char* iconName) {

   strcpy(pathBuff,ourOSPtr->getPanelFolder(appID));
   strcat(pathBuff,iconName);
   return pathBuff; 
}


void testApp::setOffsets(bool setClear) {
   
   sensors_event_t   event;

   if (setClear) {
      bno->getEvent(&event);
      offsetX = event.orientation.z;  // Yes messed up. FIX LATER.
      offsetY = event.orientation.y;
      offsetZ = event.orientation.x;
   } else {
      offsetX = 0;
      offsetY = 0;
      offsetZ = 0;
   }
   showOffsets(offsetX,offsetY,offsetZ);
}


void testApp::loop(void) {  

   point             newLoc;
   point             touchPt;
   float             dist;
   colorObj          gridColor;
   sensors_event_t   event;

   /*
   if (bubbleTimer.ding()) {
      bubbleTimer.stepTime();
      bno->getEvent(&event);
      touchPt = center;
      showValues(event.orientation.z,event.orientation.y,event.orientation.x);
      touchPt.x = touchPt.x - round(sin(theCalc.deg_2_rad(event.orientation.z - offsetX))*sinMult);
      touchPt.y = touchPt.y + round(sin(theCalc.deg_2_rad(event.orientation.y - offsetY))*sinMult);
      touchPt.x = smootherX.addData(touchPt.x);
      touchPt.y = smootherY.addData(touchPt.y);
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
   */
}


void testApp::drawSelf(void) {
    
   screen->fillScreen(&scrColor);
   //screen->fillCircle(BOUND_X,BOUND_Y,BOUND_DIA,&black);
}
