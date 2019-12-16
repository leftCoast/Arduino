#include "cEditPanel.h"
#include <EEPROM.h>

#define SWATCH_WIDTH  40
#define SWATCH_X_OFFSET 15 


         
cEditPanel::cEditPanel(void)
  : panel(colorEditApp,noMenuBar) {  }


cEditPanel::~cEditPanel(void) {  }


  
void cEditPanel::setup(void) {

  int     trace;
  label*  hourLbl;

  mParser.addCmd(help,"?");
  mParser.addCmd(getColor,"gcolor");
  mParser.addCmd(setColor,"scolor");
  mParser.addCmd(cout,"cout");
  mParser.addCmd(cin,"cin");
  mParser.addCmd(exit,"exit");
  
  trace = 1;
  for (int i=0;i<12;i++) {
    mColorRects[i] = new colorRect(SWATCH_X_OFFSET,trace,SWATCH_WIDTH,10);
    mColorRects[i]->setColor(&(colors[i]));
    addObj(mColorRects[i]);
    hourLbl = new label(0,trace+1,SWATCH_X_OFFSET,10);
    if (hourLbl) {
      hourLbl->setColors(&white);
      hourLbl->setJustify(TEXT_RIGHT);
      hourLbl->setValue(i);
      addObj(hourLbl);
    }
    trace = trace+10;
  }
  trace = 1;
  for (int i=12;i<24;i++) {
    mColorRects[i] = new colorRect(SWATCH_X_OFFSET+64,trace,SWATCH_WIDTH,10);
    mColorRects[i]->setColor(&(colors[i]));
    addObj(mColorRects[i]);
    hourLbl = new label(64,trace+1,SWATCH_X_OFFSET,10);
    if (hourLbl) {
      hourLbl->setColors(&white);
      hourLbl->setJustify(TEXT_RIGHT);
      hourLbl->setValue(i);
      addObj(hourLbl);
    }
    trace = trace+10;
  }

}


void cEditPanel::loop(void) { checkParse(); }


void cEditPanel::drawSelf(void) { screen->fillScreen(&black); }


void cEditPanel::doGetColor(void) {

  char* paramBuff;
  int   hourVal;
  
  if (mParser.numParams()==1) {
    
    paramBuff = mParser.getParam();
    hourVal = atoi (paramBuff);
    free(paramBuff);
    if (hourVal>=0&&hourVal<24) {
      Serial.print("Hour ");Serial.print(hourVal);Serial.print(" RGB Value : ");
      Serial.print(colors[hourVal].getRed());
      Serial.print(" ");Serial.print(colors[hourVal].getGreen());
      Serial.print(" ");Serial.println(colors[hourVal].getBlue());
    }
  }
}


void cEditPanel::doSetColor(void) {

  char* paramBuff;
  int   hourVal;
  int   redVal;
  int   greenVal;
  int   blueVal;
  
  if (mParser.numParams()==4) {
    
    paramBuff = mParser.getParam();
    hourVal = atoi (paramBuff);
    free(paramBuff);
    if (hourVal>=0 && hourVal<24) {
      paramBuff = mParser.getParam();
      redVal = atoi (paramBuff);
      free(paramBuff);
  
      paramBuff = mParser.getParam();
      greenVal = atoi (paramBuff);
      free(paramBuff);
  
      paramBuff = mParser.getParam();
      blueVal = atoi (paramBuff);
      free(paramBuff);
      colors[hourVal].setColor(redVal,greenVal,blueVal);
      mColorRects[hourVal]->setColor(redVal,greenVal,blueVal);
      ourOS.saveParams();
    } else {
      Serial.println("No, values between 0..23 please.");
    }
  }
}

          
void cEditPanel::doCout() {

  for (int i=0;i<24;i++) {
    Serial.print("scolor ");Serial.print(i);Serial.print(" ");
    Serial.print(colors[i].getRed());Serial.print(" ");
    Serial.print(colors[i].getGreen());Serial.print(" ");
    Serial.print(colors[i].getBlue());Serial.print(EOL);
  }
}


void cEditPanel::doCin() {

  char* paramBuff;
  int   hourVal;
  int   redVal;
  int   greenVal;
  int   blueVal;
  int   params;
  
  hourVal = 0;
  params = mParser.numParams();
  while (params) {
    paramBuff = mParser.getParam();
    redVal = atoi (paramBuff);
    free(paramBuff);
    params--;

    paramBuff = mParser.getParam();
    greenVal = atoi (paramBuff);
    free(paramBuff);
    params--;

    paramBuff = mParser.getParam();
    blueVal = atoi (paramBuff);
    free(paramBuff);
    params--;

    if (params>=0) {
      colors[hourVal].setColor(redVal,greenVal,blueVal);
      mColorRects[hourVal]->setColor(redVal,greenVal,blueVal);
    }
    hourVal++;
  }
  if (hourVal==24) {
    ourOS.saveParams();
  } else {
    Serial.println("Numbers didn't line up. NOT saving data.");
  }
}


void cEditPanel::checkParse(void) {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       
      case noCommand      : break;
      case help           : 
        Serial.println("Commands:");
        Serial.println("   gcolor followed by number returns R,G,B value of that color.");
        Serial.println("   scolor followed by 4 numbers. Color#,R,G,B sets that color's value.");
        Serial.println("   exit closes the color editor.");
      break;
      case getColor : doGetColor(); break;
      case setColor : doSetColor(); break;
      case cout     : doCout();     break;
      case cin      : doCin();      break;
      case exit     : close();      break;
      default       : 
        Serial.println("Command line interfaces just suck, don't they?");
        Serial.println("Try typing ? for a list of commands.");
      break;
    }
  } 
}
