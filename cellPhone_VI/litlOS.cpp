
#include "litlOS.h"

#include <screen.h>


colorObj  aColor;   // A color object anyone can use.

homePanel::homePanel(blockFile* inFile,unsigned long rootID)
  : panel(inFile,rootID,true) {
    
}


homePanel::~homePanel(void) {  }

  
void homePanel::drawSelf(void) { }



  
litlOS::litlOS(void) {

  mFile = NULL;
  mSysPanel = NULL;
}


litlOS::~litlOS(void) {  }


void litlOS::begin(void) {

  mFile = new blockFile(SYS_FILE_PATH);
  if (mFile->isEmpty()) {
    initOSFile();
  }
  mSysPanel = new homePanel(mFile,mFile->readRootBlockID());
  viewList.addObj(mSysPanel);
  hookup();
}


void litlOS::initOSFile(void) {  }


void litlOS::idle(void) {  }
