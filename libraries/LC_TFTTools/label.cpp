#include "label.h"

#define TEMP_BUFF_SIZE 80    // Temp buff for doing text formatting and things. 
char  temp[TEMP_BUFF_SIZE];

label::label() : 
drawObj() {
  initLabel();
}


label::label(char* inText) : 
drawObj() {

  initLabel();
  setValue(inText);
  setSize(getTextWidth(),getTextHeight());
}


label::label(char* inText, word inSize) : 
drawObj() {

  initLabel();
  setValue(inText);
  setTextSize(inSize);
  setSize(getTextWidth(),getTextHeight());
}


label::label(Point inLoc, int inWidth,int inHeight) : 
drawObj(inLoc,inWidth,inHeight) {
  initLabel();
}


label::label(Point inLoc, int inWidth,int inHeight, char* inText) :
drawObj(inLoc,inWidth,inHeight) {
  initLabel();
  setValue(inText);
}


label::~label() { 
  freeBuff(); 
}


void label::initLabel(void) {

  textSize = DEF_TEXT_SIZE;
  setColors(BLACK,WHITE);
  setJustify(TEXT_LEFT);
  buff = NULL;
  prec = DEF_PRECISION;
}


void label::freeBuff(void) {

  if (buff!=NULL) {
    free(buff);
    buff = NULL;
  }  
}


void label::setTextSize(word inSize) {

  textSize = inSize;
  setRefresh();
}


void label::setJustify(word inJustify) {

  justify = inJustify;
  setRefresh();
}


void label::setColors(word inTextColor, word inBackColor) {

  textColor = inTextColor;
  backColor = inBackColor;
  setRefresh();
}


void label::setColors(colorObj inTextColor, colorObj inBackColor) {

  setColors(inTextColor.getColor16(),inBackColor.getColor16());
}


void label::setPrecision(int inPrec) {

  if (inPrec>=0) {
    prec = inPrec;
    setRefresh();
  }
}


void label::setValue(int val) {

  snprintf (temp,TEMP_BUFF_SIZE,"%d",val);
  setValue(temp);
  setRefresh();
}


void label::setValue(unsigned long val) {

  snprintf (temp,TEMP_BUFF_SIZE,"%u",val);
  setValue(temp);
  setRefresh();
}


void label::setValue(double val) {

  dtostrf(val,0,prec,temp);	
  setValue(temp);
  setRefresh();
}

void label::setValue(float val) { 
  setValue((double) val); 
}

void label::setValue(char* str) {

  word numChars;

  freeBuff();
  numChars = strlen(str) + 1;
  buff = (char*) malloc(numChars);
  strcpy (buff,str);
  setRefresh(); 
}


word label::getTextWidth(void) {

  return(CHAR_WIDTH*textSize*strlen(buff));
}


word label::getTextHeight(void) { 
  return(CHAR_HEIGHT*textSize); 
}


void label::draw(void) {

  word numCharsDisp;  // How many we have room for?
  int charDif;     

  if (buff) {
    screen->setTextColor(textColor,backColor);
    screen->setTextSize(textSize);
    screen->setTextWrap(false);
    screen->setCursor(location.x, location.y);

    numCharsDisp = width/(CHAR_WIDTH*textSize);
    charDif =  numCharsDisp - strlen(buff);
    if (charDif==0) {               //Exact amount..
      screen->print(buff);
    } 
    else if (charDif>0) {          // Needs padding..
      switch (justify) {
      case TEXT_LEFT :
        for(int i=1;i<=charDif;i++) screen->print(" ");
        break;
      case TEXT_RIGHT :
        for(int i=1;i<=charDif;i++) screen->print(" ");
        screen->print(buff);
        break;
      }
    } 
    else {                      // Needs truncation..
      switch (justify) {
      case TEXT_LEFT :
        for(int i=0;i<numCharsDisp;i++) screen->print(buff[i]);
        break;
      case TEXT_RIGHT :
        for(int i=0;i<numCharsDisp;i++) screen->print(buff[i-charDif]);  // Subtract 'cause its negitive, dumy!
        break;
      }
    }
  }
}







