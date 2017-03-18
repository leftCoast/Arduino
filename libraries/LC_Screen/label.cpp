#include "label.h"
 
char  temp[TEMP_BUFF_SIZE];

label::label() : 
drawObj() {
  initLabel();
}

// These next two actually fit our rectangle to the text.
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


label::label(int inLocX, int inLocY, int inWidth,int inHeight) :
drawObj(inLocX,inLocY,inWidth,inHeight) {
  initLabel();
}


label::label(int inLocX, int inLocY, int inWidth,int inHeight, char* inText) :
drawObj(inLocX,inLocY,inWidth,inHeight) {
  initLabel();
  setValue(inText);
}


label::~label() { 
  freeBuff(); 
}


void label::initLabel(void) {

  textSize = DEF_TEXT_SIZE;
  setColors(&black,&white);
  setJustify(TEXT_LEFT);
  buff = NULL;
  prec = DEF_PRECISION;
  needRefresh = true;
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


void label::setColors(colorObj* tColor, colorObj* bColor) {

  textColor.setColor(tColor);
  backColor.setColor(bColor);
  transp = false;           // we set both.
  needRefresh = true;
}


void label::setColors(colorObj* tColor) { setColors(tColor,&white); transp = true; }
  

void label::setPrecision(int inPrec) {

  if (inPrec>=0) {
    prec = min(MAX_PREC,inPrec);	// No limit and the temp buffer explodes.
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
   
	// Need to round to the desired prec. value first.
  dtostrf(val,0,prec,temp);
  setValue(temp);
  needRefresh = true;
}


void label::setValue(float val) {
  double dVal = val;
  setValue(dVal); 
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
	if (buff) {
    if (transp) {
      screen->setTextColor(&textColor);
    } else {
      screen->setTextColor(&textColor,&backColor);
    }
    screen->setTextSize(textSize);
    screen->setTextWrap(false);
    screen->setCursor(x,y);
    numCharsDisp = width/(CHAR_WIDTH*textSize);
    charDif =  numCharsDisp - strlen(buff);
    if (charDif==0) {               //Exact amount..
      screen->drawText(buff);
    } else if (charDif>0) {          // Needs padding..
      switch (justify) {
      case TEXT_LEFT :
        screen->drawText(buff);
        for(int i=1;i<=charDif;i++) screen->drawText(" ");
        break;
      case TEXT_RIGHT :
        for(int i=1;i<=charDif;i++) screen->drawText(" ");
        screen->drawText(buff);
        break;
      case TEXT_CENTER :
      	int leadSp = charDif/2;
      	int trailSp = charDif-leadSp;
        for(int i=1;i<=leadSp;i++) screen->drawText(" ");
        screen->drawText(buff);
        for(int i=1;i<=trailSp;i++) screen->drawText(" ");
        break;
      }
		} else {                      // Needs truncation..
    	if (numCharsDisp<(TEMP_BUFF_SIZE-1)) {
			  switch (justify) {
				case TEXT_LEFT :						// 1/19/2017 : This bit has never been tested.
					int i;
					for(i=0;i<numCharsDisp;i++) {
						temp[i] = buff[i];
					}
					temp[i] = '\0';
					screen->drawText(temp);
					break;
				case TEXT_RIGHT :						// This bit has been tested, and works.
					screen->drawText((char*)&(buff[-charDif]));  
					break;
				}
			} else {
				screen->drawText("Overflow");
			}
		}
	}
}







