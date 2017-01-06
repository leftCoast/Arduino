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


label::label(word inLocX, word inLocY, int inWidth,int inHeight) :
drawObj(inLocX,inLocY,inWidth,inHeight) {
  initLabel();
}


label::label(word inLocX, word inLocY, int inWidth,int inHeight, char* inText) :
drawObj(inLocX,inLocY,inWidth,inHeight) {
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
  needRefresh = true;
}


void label::setJustify(word inJustify) {

  justify = inJustify;
  needRefresh = true;
}


void label::setColors(word inTextColor, word inBackColor) {

  textColor = inTextColor;
  backColor = inBackColor;
  transp = false;           // we set both.
  needRefresh = true;
}


void label::setColors(colorObj inTextColor, colorObj inBackColor) {

  setColors(inTextColor.getColor16(),inBackColor.getColor16());
}


void label::setColors(colorObj tColor) { setColors(tColor,white); transp = true; }


void label::setColors(word tColor)  { setColors(tColor,white); transp = true; }
  

void label::setPrecision(int inPrec) {

  if (inPrec>=0) {
    prec = inPrec;
    needRefresh = true;
  }
}


void label::setValue(int val) {

  snprintf (temp,TEMP_BUFF_SIZE,"%d",val);
  setValue(temp);
  needRefresh = true;
}


void label::setValue(unsigned long val) {

  snprintf (temp,TEMP_BUFF_SIZE,"%lu",val);
  setValue(temp);
  needRefresh = true;
}


void label::setValue(double val) {

  dtostrf(val,0,prec,temp);	
  setValue(temp);
  needRefresh = true;
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
  needRefresh = true;
}


word label::getTextWidth(void) {

  return(CHAR_WIDTH*textSize*strlen(buff));
}


word label::getTextHeight(void) { 
  return(CHAR_HEIGHT*textSize); 
}


void label::drawSelf(void) {

  int numCharsDisp;  // How many we have room for?
  int charDif;     
  colorObj  tColor(textColor);
  colorObj  bColor(backColor);
  
  if (buff) {
    if (transp) {
      screen->setTextColor(&tColor);
    } else {
      screen->setTextColor(&tColor,&bColor);
    }
    screen->setTextSize(textSize);
    screen->setTextWrap(false);
    screen->setCursor(locX,locY);

    numCharsDisp = width/(CHAR_WIDTH*textSize);
    charDif =  numCharsDisp - strlen(buff);
    if (charDif==0) {               //Exact amount..
      screen->drawText(buff);
    } 
    else if (charDif>0) {          // Needs padding..
      switch (justify) {
      case TEXT_LEFT :
        screen->drawText(buff);
        for(int i=1;i<=charDif;i++) screen->drawText(" ");
        break;
      case TEXT_RIGHT :
        for(int i=1;i<=charDif;i++) screen->drawText(" ");
        screen->drawText(buff);
        break;
      }
    } 
    else {                      // Needs truncation..
      switch (justify) {
      case TEXT_LEFT :
        for(int i=0;i<numCharsDisp;i++) screen->drawText((char*)(buff[i]));
        break;
      case TEXT_RIGHT :
        for(int i=0;i<numCharsDisp;i++) screen->drawText((char*)(buff[i-charDif]));  // Subtract 'cause its negitive, dumy!
        break;
      }
    }
  }
}







