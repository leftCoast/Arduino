#include "radioBtn.h"



radioBtn::radioBtn(void) {  }
radioBtn::~radioBtn(void) {  }
 
void radioBtn::setSet(radioBtnSet* inSet) { ourSet = inSet; }

void radioBtn::setState(byte inState) {

  boolean didAction;
  
  didAction = (inState==active && state==touched);
  touchObj::setState(inState);
  if (didAction) {
    state = inactive;
    ourSet->setStates(this);
  }
} 

  

radioBtnSet::radioBtnSet(void) : touchObj(), linkList(false) {  }
radioBtnSet::~radioBtnSet(void) {  }

void radioBtnSet::addObj(radioBtn* newObj) {

   newObj->setSet(this);
  linkList::addToEnd(newObj);
}
    

void radioBtnSet::draw(void) {
 
  linkListObj* trace;
  
  trace = theList;
  while(trace!=NULL) {
    ((radioBtn*)trace)->draw();
    trace = trace->next;
  }
}


void radioBtnSet::drawSelf(void) {  }


boolean radioBtnSet::checkTouch(Point where) {
  
  linkListObj* trace;
  boolean      done;
  
  trace = theList;
  done = false;
  while(trace!=NULL&&!done) {
    done = ((radioBtn*)trace)->checkTouch(where);
    if (!done) trace = trace->next;
  }
  return done;
}


void radioBtnSet::setStates(radioBtn* hitBtn) {
  
   linkListObj* trace;
  
  trace = theList;
  while(trace!=NULL) {
    if ((radioBtn*)trace!=hitBtn) {
      ((radioBtn*)trace)->setState(active);
    }
    trace = trace->next;
  }
}


