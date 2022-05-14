#include "UI.h"

#define SET_BTN_PATH    "System/appFiles/testApp/crossHr.bmp"
#define CLEAR_BTN_PATH  "System/appFiles/testApp/globeCH.bmp"
#define MASK_PATH       "System/icons/standard/mask32.bmp"

bmpMask        icon32Mask;

float          offsetX;
float          offsetY;
float          offsetZ;
clearAngleBtn* ourCAngleBtn;
setAngleBtn*   ourSAngleBtn;


void setupUI(void) {
   
   icon32Mask.readFromBMP(MASK_PATH);
   
   ourCAngleBtn = new clearAngleBtn(20,240,CLEAR_BTN_PATH);
   ourCAngleBtn->setMask(&icon32Mask); 
   viewList.addObj(ourCAngleBtn);
   
   ourSAngleBtn = new setAngleBtn(20,280,SET_BTN_PATH);
   ourSAngleBtn->setMask(&icon32Mask); 
   viewList.addObj(ourSAngleBtn);

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
