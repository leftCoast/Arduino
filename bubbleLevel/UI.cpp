#include "UI.h"

#define SET_BTN_PATH    "System/appFiles/testApp/crossHr.bmp"
#define CLEAR_BTN_PATH  "System/appFiles/testApp/globeCH.bmp"
#define MASK_PATH       "System/icons/standard/mask32.bmp"

bmpMask        icon32Mask;

clearAngleBtn* ourCAngleBtn;
setAngleBtn*   ourSAngleBtn;
label*         rotationAngleX;
label*         rotationAngleY;
label*         rotationAngleZ;
label*         rotX;
label*         rotY;
label*         rotZ;
label*         modelAngleX;
label*         modelAngleY;
label*         modelAngleZ;
label*         modelX;
label*         modelY;
label*         modelZ;



void setupUI(void) {
   
   icon32Mask.readFromBMP(MASK_PATH);
   
   ourCAngleBtn = new clearAngleBtn(20,240,CLEAR_BTN_PATH);
   ourCAngleBtn->setMask(&icon32Mask); 
   viewList.addObj(ourCAngleBtn);
   
   ourSAngleBtn = new setAngleBtn(20,280,SET_BTN_PATH);
   ourSAngleBtn->setMask(&icon32Mask); 
   viewList.addObj(ourSAngleBtn);

   int TxtX = 160;
   int rawTxty = 240;
   int offset = 0;
   int ySpace = 10;
   
   rotationAngleX = new label(200,rawTxty+offset,40,18,"0.00",1);
   rotationAngleX->setColors(&blue,&black);
   rotationAngleX->setJustify(TEXT_RIGHT);
   viewList.addObj(rotationAngleX);

   rotX = new label(TxtX,rawTxty+offset,40,18,"Raw x:",1);
   rotX->setColors(&blue,&black);
   rotX->setJustify(TEXT_RIGHT);
   viewList.addObj(rotX);
   offset = offset + ySpace;
   
   rotationAngleY = new label(200,rawTxty+offset,40,18,"0.00",1);
   rotationAngleY->setColors(&blue,&black);
   rotationAngleY->setJustify(TEXT_RIGHT);
   viewList.addObj(rotationAngleY);

   rotY = new label(TxtX,rawTxty+offset,40,18,"Raw y:",1);
   rotY->setColors(&blue,&black);
   rotY->setJustify(TEXT_RIGHT);
   viewList.addObj(rotY);
   offset = offset + ySpace;
   
   rotationAngleZ = new label(200,rawTxty+offset,40,18,"0.00",1);
   rotationAngleZ->setColors(&blue,&black);
   rotationAngleZ->setJustify(TEXT_RIGHT);
   viewList.addObj(rotationAngleZ);

   rotZ = new label(TxtX,rawTxty+offset,40,18,"Raw z:",1);
   rotZ->setColors(&blue,&black);
   rotZ->setJustify(TEXT_RIGHT);
   viewList.addObj(rotZ);
   
   offset = offset + ySpace;
   offset = offset + ySpace;

   modelAngleX = new label(200,rawTxty+offset,40,18,"0.00",1);
   modelAngleX->setColors(&blue,&black);
   modelAngleX->setJustify(TEXT_RIGHT);
   viewList.addObj(modelAngleX);

   modelX = new label(TxtX,rawTxty+offset,40,18,"Mod x:",1);
   modelX->setColors(&blue,&black);
   modelX->setJustify(TEXT_RIGHT);
   viewList.addObj(modelX);
   offset = offset + ySpace;
   
   modelAngleY = new label(200,rawTxty+offset,40,18,"0.00",1);
   modelAngleY->setColors(&blue,&black);
   modelAngleY->setJustify(TEXT_RIGHT);
   viewList.addObj(modelAngleY);

   modelY = new label(TxtX,rawTxty+offset,40,18,"Mod y:",1);
   modelY->setColors(&blue,&black);
   modelY->setJustify(TEXT_RIGHT);
   viewList.addObj(modelY);
   offset = offset + ySpace;
   
   modelAngleZ = new label(200,rawTxty+offset,40,18,"0.00",1);
   modelAngleZ->setColors(&blue,&black);
   modelAngleZ->setJustify(TEXT_RIGHT);
   viewList.addObj(modelAngleZ);

   modelZ = new label(TxtX,rawTxty+offset,40,18,"Mod z:",1);
   modelZ->setColors(&blue,&black);
   modelZ->setJustify(TEXT_RIGHT);
   viewList.addObj(modelZ);
   
}


//****************************************************************************************
// setAngleBtn:
// 
// The thing the user touches to set "this" angle as zero.
//****************************************************************************************   


setAngleBtn::setAngleBtn(int xLoc,int yLoc,const char* path)
   :iconButton(xLoc,yLoc,path) { }


setAngleBtn::~setAngleBtn(void) {  }

void setAngleBtn::doAction(void) {
   
   if (callback) {
      callback();
   }             
}



//****************************************************************************************
// clearAngleBtn:
// 
// The thing the user touches to set ground angle back to zero.
//****************************************************************************************


clearAngleBtn::clearAngleBtn(int xLoc,int yLoc,const char* path)
   :iconButton(xLoc,yLoc,path) { }


clearAngleBtn::~clearAngleBtn(void) {  }

void clearAngleBtn::doAction(void) { 

   if (callback) {   
      callback();  
   }                 
}
