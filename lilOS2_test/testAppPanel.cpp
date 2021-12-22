#include "testAppPanel.h"
#include <alertObj.h>
#include <fSelectObj.h>


// And it all starts up again..
testAppPanel::testAppPanel(lilOS* ourOS,int ourAppID)
   : panel(ourAppID) { needRefresh = false; }


testAppPanel::~testAppPanel(void) {  }


void testAppPanel::setup(void) {
   
   //alertStart.setTime(5000);
   aFlasher = new flasher(20,160,10,10);
   aFlasher->setOnOff(true);
   addObj(aFlasher);
   stdComBtn* aBtn = newStdBtn(40,50,icon32,newItemCmd,this);
   addObj(aBtn);
   choosenPath = new label(20,300,200,18);
   addObj(choosenPath);
}


void testAppPanel::loop(void) {
   
   if (needRefresh && !ourEventMgr.active()) {
      setNeedRefresh();
      needRefresh = false;
   }
}


// The default here is to not draw ourselves. You can change that.
void testAppPanel::drawSelf(void) {

   colorObj aColor;
   
   aColor.setColor(LC_GREY);
   screen->fillRect(this,&aColor);
}


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void testAppPanel::closing(void) { }



void  testAppPanel::handleCom(stdComs comID) {

   alertObj* anAlert;
   
   switch(comID) {
      case okCmd     :
         aFlasher->setColors(&green,&black);
         choosenPath->setValue(choosAlert->getPathResult());
         choosAlert = NULL;
         needRefresh = true;
      break;
      case cancelCmd :
         aFlasher->setColors(&red,&black);
         choosAlert = NULL;
         choosenPath->setValue("Canceled");
         needRefresh = true;
      break;
      case newItemCmd :
         choosAlert = new fSelectObj(this);
      break;
      default  : panel::handleCom(comID); break;
   }
   
}
