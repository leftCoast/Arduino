#include "label.h"
#include <resizeBuff.h>


char  temp[TEMP_BUFF_SIZE];


label::label() : drawObj() { initLabel(); }


// These next two actually fit our rectangle to the text.
label::label(char* inText)
	: drawObj() {

	initLabel();
	setValue(inText);
	setSize(getTextWidth(),getTextHeight());
}


label::label(char* inText, int inSize) 
	: drawObj() {

	initLabel();
	setValue(inText);
	setTextSize(inSize);
	setSize(getTextWidth(),getTextHeight());
}


label::label(int inLocX, int inLocY, int inWidth,int inHeight)
	: drawObj(inLocX,inLocY,inWidth,inHeight) { initLabel(); }


label::label(int inLocX, int inLocY, int inWidth,int inHeight, char* inText)
	: drawObj(inLocX,inLocY,inWidth,inHeight) {
	
	initLabel();
	setValue(inText);
}


label::label(int inLocX, int inLocY, int inWidth,int inHeight, char* inText,int textSize)
	: drawObj(inLocX,inLocY,inWidth,inHeight) {

	initLabel();
	setValue(inText);
	setTextSize(textSize);
}


label::label(rect* inRect,char* inText,int textSize)
	: drawObj(inRect) {

	initLabel();
	setValue(inText);
	setTextSize(textSize);
}


// We don't call initLabel() in this one. Everyting comes from the copied label.
// So be sure we check everything.
label::label(label* aLabel)
	: drawObj(aLabel) {
	
	buff = NULL;								// Best to have this initialized!
	setValue(aLabel->buff);
	setTextSize(aLabel->textSize);
	setJustify(aLabel->justify);
	setColors(&aLabel->textColor,&aLabel->backColor);
	transp = aLabel->transp;
	setPrecision(aLabel->prec);
	needRefresh = true;
}	

	
label::~label() { resizeBuff(0,&buff); }


void label::initLabel(void) {

  textSize = DEF_TEXT_SIZE;
  setColors(&black,&white);
  setJustify(TEXT_LEFT);
  buff = NULL;
  prec = DEF_PRECISION;
  needRefresh = true;
}


void label::setTextSize(int inSize) {

  textSize = inSize;
  needRefresh = true;
}


void label::setJustify(int inJustify) {

  justify = inJustify;
  needRefresh = true;
}


void label::setColors(colorObj* tColor, colorObj* bColor) {

  textColor.setColor(tColor);
  backColor.setColor(bColor);
  transp = false;           // we set both.
  needRefresh = true;
}


void label::setColors(colorObj* tColor) {

	setColors(tColor,&backColor);
	transp = true;
}
  

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
   
  dtostrf(val,0,prec,temp);
  setValue(temp);
  needRefresh = true;
}


void label::setValue(float val) {
  double dVal = val;
  setValue(dVal); 
}


void label::setValue(char* str) {

	int numChars;

	if (!str) {
		if (resizeBuff(1,&buff)) {
			buff[0] = '\0';
		}
	} else {
		numChars = strlen(str) + 1;
		if (resizeBuff(numChars,&buff)) {
			strcpy (buff,str);
		}
	}
	needRefresh = true;
}


// We want to know how long the string is.. (*** REMEBER NOT COUNTING '\0' ***)
int label::getNumChars(void) { return strlen(buff); }


// We want to know how many chars can we display?
int label::getViewChars(void) { return width/(CHAR_WIDTH*textSize); }

	
// We asked above how much you have. Hand it over!	(They better add one for the \0!)	
void label::getText(char* inBuff) { strcpy(inBuff,buff); }
				
				
int label::getTextWidth(void) { return(CHAR_WIDTH*textSize*strlen(buff)); }


int label::getTextHeight(void) { return(CHAR_HEIGHT*textSize); }


void label::drawSelf(void) {

	int	charDif; 
   int	numCharsDisp; 
	
	if (buff) {																			// Sanity. If we have anything to display..
		numCharsDisp = getViewChars();											// Save off how many chars we can display.
		if (transp) {																	// If we don't draw background bits..
			screen->setTextColor(&textColor);									// We just set the one color as a flag for this.
		} else {																			// Else, we DO want to draw the background bits.
			screen->setTextColor(&textColor,&backColor);						// We set BOTH colors as a flag for both.
		}
		screen->setTextSize(textSize);											// Set the text size.
		screen->setTextWrap(false);												// Turn off wrap. It just makes a mess of everything.
		screen->setCursor(x,y);														// Move the cursor to our XY location.
		charDif =  numCharsDisp - strlen(buff);								// Calculate the total amount of blanks we'll need.
		if (charDif==0) {																// If its a perfect fit..
			screen->drawText(buff);													// Just draw it out. EASY PEASY! (And we're done)
		} else if (charDif>0) {														// Else, we'll need some padding..
			switch (justify) {														// Left, Right, Center will be handled differently.
				case TEXT_LEFT :														// ** LEFT **
					screen->drawText(buff);											// Draw the text.
					for(int i=1;i<=charDif;i++) screen->drawText(" ");		// Add the padding.
				  break;																	// And that's it.
				case TEXT_RIGHT :														// ** RIGHT **
				  for(int i=1;i<=charDif;i++) screen->drawText(" ");		// First we add the padding.
				  screen->drawText(buff);											// Then draw the text.
				  break;																	// And we're done.
				case TEXT_CENTER :													// ** CENTER **
					int leadSp = charDif/2;											// Do integer divide by 2 for the lead blank count.
					int trailSp = charDif-leadSp;									// Subtract the lead count from the blank count for the trailing count.
				  for(int i=1;i<=leadSp;i++) screen->drawText(" ");		// Add the lead padding.
				  screen->drawText(buff);											// Draw the string.
				  for(int i=1;i<=trailSp;i++) screen->drawText(" ");		// Add the trailing padding.
				  break;																	// And again, we're done.
				}
		} else {                      											// Else the string needs truncation..
			if (numCharsDisp<(TEMP_BUFF_SIZE-1)) {								// Make sure we have enough room to maneuver.
				switch (justify) {													// Left, Right, Center will be handled differently.
					case TEXT_LEFT :													// ** LEFT **
						temp[0] = '\0';												// Clear the temp buffer.
						strncat(temp,buff,numCharsDisp);							// Stamp in the characters to display.
						screen->drawText(temp);										// Draw the string.
					break;																// And we're done.
					case TEXT_RIGHT :													// ** RIGHT**						
						screen->drawText((char*)&(buff[-charDif])); 			// Just draw the text starting after the clipped portion.
					break;
					case TEXT_CENTER :												// ** CENTER **
						int firstChar = -charDif/2;								// Calculate first char to display.
						temp[0] = '\0';												// "Clear" the temp buffer.
						strncat(temp,&buff[firstChar],numCharsDisp);			// Stamp in the characters to display.
						screen->drawText(temp);										// Draw the string.
					break;																// And we're done.
				}
			} else {
				screen->drawText("Overflow");										// Our temp buffer is just to small!
			}
		}
	}
}







