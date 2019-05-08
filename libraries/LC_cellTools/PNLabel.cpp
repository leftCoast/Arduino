#include "PNLabel.h"
#include <resizeBuff.h>
   
   
// Drop in a string and this'll strip out anything
// that's not a "dial-able" character.
// ** WARNING ** This writes to the string in place. So you can't pass a string that was
// allocated at compile time IE char myNum = "1 408 340-0352"
void filterPNStr(char* str) {

	int numChars;
	int index;
	
	if (str) {									// Sanity, they could pass in a NULL. They do that now and then.
		numChars = strlen(str);				// Ok have something. Lets count 'em.
		index = 0;								// We'll use this to index mRawPN.
		for(int i=0;i<=numChars;i++) {	// We'll loop though including the EOL.
			switch(str[i]) {					// There may be better ways of doing this,
				case '0'  :						//  but this makes it realaly obvious what we're doing.
				case '1'  :
				case '2'  :
				case '3'  :
				case '4'  :
				case '5'  :
				case '6'  :
				case '7'  :
				case '8'  :
				case '9'  :
				case '#'  :
				case '*'  :
				case '\0' : str[index++] = str[i]; break;  // Stuff in the "filtered" char.
			}
		}
	}
}


// Your standard drawObject kind of constructor..
PNLabel::PNLabel(int inLocX, int inLocY, int inWidth,int inHeight,int textSize)
  : label(inLocX,inLocY,inWidth,inHeight) {
    
  setTextSize(textSize);
  mFormattedPN  = NULL;
  mRawPN        = NULL;
}

 
// And destructor.. 
PNLabel::~PNLabel(void) {

  if (mFormattedPN) free(mFormattedPN);
  if (mRawPN) free(mRawPN);
}


// This sets our label to a nice formatted phone number string.
void PNLabel::setValue(char* str) {

	int numBytes;
	
	if (str) {																		// No NULLs!
		filterPNStr(str);															// Remove the "junk".
		numBytes = strlen(str)+1;												// Count what's left over.
		if (resizeBuff(numBytes,(uint8_t**)&mRawPN)) {					// Grab some RAM.
			strcpy(mRawPN,str);													// copy this over to our raw string
			numBytes = strlen(mRawPN) + 8; 									// b ( ) b - \0;
  			if (resizeBuff(numBytes,(uint8_t**)&mFormattedPN)) {		// Grab RAM for our formatted version.
				formatPN();															// Create the formatted version.
				label::setValue(mFormattedPN);								// Set the label with the formatted string.
				resizeBuff(0,(uint8_t**)&mFormattedPN);					// Recycle our formattedPN string.
			} else {
				label::setValue("Out of RAM!");								// Set the label; We're out of RAM!"
			}
			resizeBuff(0,(uint8_t**)&mRawPN);								// Recycle mRawPN's RAM.
		} else {
			label::setValue("Out of RAM!");									// Set the label; We're out of RAM!"
		}
	} else {
		label::setValue("");														// NULLs look like this.
	}
}


// How big of a buffer should you create to fit the raw number string?
// Well, this should be more than big enough.
int PNLabel::getRawNumBytes(void) { return strlen(buff) + 1; }


// Someone wants the raw, numbers only versoin of the phone number. Probably
// wants to try dialing it.
void PNLabel::getRawPN(char* inBuff) {

	if (buff) {
		strcpy(inBuff,buff);
	}
}
				

// This is the formatting method check first letter opening method. It
// decides, from reading the first character, what subsequent method to
// call.				
void PNLabel::formatPN(void) {
  switch(mRawPN[0]) {
    case '1'  : formatOne();    break;
    case '*'  : formatStar();   break;
    case '#'  : formatHash();   break;
    default   : formatStd();    break;
  }
}

// The rest are different ways a phone number can be formatted geven the
// first character.
void PNLabel::formatOne(void) {

  int numChars;

  numChars = strlen(mRawPN);
  switch(numChars) {
    case 0  : mFormattedPN[0] = '\0'; break;
    case 1  : 
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '\0';
    break;
    case 2  :
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '(';
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = ')';
      mFormattedPN[4] = '\0';
    break;
    case 3  :
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '(';
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = mRawPN[2];
      mFormattedPN[4] = ')';
      mFormattedPN[5] = '\0';
    break;
    case 4  :
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '(';
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = mRawPN[2];
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = ')';
      mFormattedPN[6] = '\0';
    break;
    case 5  :
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '(';
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = mRawPN[2];
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = ')';
      mFormattedPN[6] = ' ';
      mFormattedPN[7] = mRawPN[4];
      mFormattedPN[8] = '\0';
    break;
    case 6  :
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '(';
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = mRawPN[2];
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = ')';
      mFormattedPN[6] = ' ';
      mFormattedPN[7] = mRawPN[4];
      mFormattedPN[8] = mRawPN[5];
      mFormattedPN[9] = '\0';
    break;
    case 7  :
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '(';
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = mRawPN[2];
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = ')';
      mFormattedPN[6] = ' ';
      mFormattedPN[7] = mRawPN[4];
      mFormattedPN[8] = mRawPN[5];
      mFormattedPN[9] = mRawPN[6];
      mFormattedPN[10] = '\0';
    break;
    default :
      mFormattedPN[0] = '1';
      mFormattedPN[1] = '(';
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = mRawPN[2];
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = ')';
      mFormattedPN[6] = ' ';
      mFormattedPN[7] = mRawPN[4];
      mFormattedPN[8] = mRawPN[5];
      mFormattedPN[9] = mRawPN[6];
      mFormattedPN[10] = '-';
      strcpy(&mFormattedPN[11],&mRawPN[7]);
    break;
  }
}


void PNLabel::formatStar(void) {

  int numChars;

  numChars = strlen(mRawPN);
  switch(numChars) {
    case 0  : mFormattedPN[0] = '\0'; break;
    case 1  : 
      mFormattedPN[0] = '*';
      mFormattedPN[1] = '\0';
    break;
    case 2  :
      mFormattedPN[0] = '*';
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = '\0';
    break;
    case 3  :
      mFormattedPN[0] = '*';
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = mRawPN[2];
      mFormattedPN[3] = '\0';
    break;
    default  :
      mFormattedPN[0] = '*';
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = mRawPN[2];
      mFormattedPN[3] = ' ';
      strcpy(&mFormattedPN[4],&mRawPN[3]);
    break;
  }
}


void PNLabel::formatHash(void) {

  int numChars;

  numChars = strlen(mRawPN);
  switch(numChars) {
    case 0  : mFormattedPN[0] = '\0'; break;
    case 1  :
      mFormattedPN[0] = '#';
      mFormattedPN[1] = '\0';
    break;
    default  :
      mFormattedPN[0] =  '#';
      strcpy(&mFormattedPN[1],&mRawPN[1]);
    break;
  }
}


void PNLabel::formatStd(void) {

  int numChars;

  numChars = strlen(mRawPN);
  switch(numChars) {
    case 0  : mFormattedPN[0] = '\0'; break;
    case 1  :
      mFormattedPN[0] = mRawPN[0];
      mFormattedPN[1] = '\0';
    break;
    case 2  :
      mFormattedPN[0] = mRawPN[0];
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = '\0';
    break;
    case 3  :
      mFormattedPN[0] = mRawPN[0];
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = mRawPN[2];
      mFormattedPN[3] = '\0';
    break;
    case 4  :
      mFormattedPN[0] = mRawPN[0];
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = mRawPN[2];
      mFormattedPN[3] = '-';
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = '\0';
    break;
    case 5  :
      mFormattedPN[0] = mRawPN[0];
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = mRawPN[2];
      mFormattedPN[3] = '-';
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = mRawPN[4];
      mFormattedPN[6] = '\0';
    break;
    case 6  :
      mFormattedPN[0] = mRawPN[0];
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = mRawPN[2];
      mFormattedPN[3] = '-';
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = mRawPN[4];
      mFormattedPN[6] = mRawPN[5];
      mFormattedPN[7] = '\0';
    break;
    case 7  :
      mFormattedPN[0] = mRawPN[0];
      mFormattedPN[1] = mRawPN[1];
      mFormattedPN[2] = mRawPN[2];
      mFormattedPN[3] = '-';
      mFormattedPN[4] = mRawPN[3];
      mFormattedPN[5] = mRawPN[4];
      mFormattedPN[6] = mRawPN[5];
      mFormattedPN[7] = mRawPN[6];
      mFormattedPN[8] = '\0';
    break;
    default   :
      mFormattedPN[0] = '(';
      mFormattedPN[1] = mRawPN[0];
      mFormattedPN[2] = mRawPN[1];
      mFormattedPN[3] = mRawPN[2];
      mFormattedPN[4] = ')';
      mFormattedPN[5] = ' ';
      mFormattedPN[6] = mRawPN[3];
      mFormattedPN[7] = mRawPN[4];
      mFormattedPN[8] = mRawPN[5];
      mFormattedPN[9] = '-';
      strcpy(&mFormattedPN[10],&mRawPN[6]);
    break;
  }
}
