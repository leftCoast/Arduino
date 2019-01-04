#include "PNLabel.h"



PNLabel::PNLabel(int inLocX, int inLocY, int inWidth,int inHeight,int textSize)
  : label(inLocX,inLocY,inWidth,inHeight) {
    
  setTextSize(textSize);
  //setColors(&blue,&white);
  mFormattedPN  = NULL;
  mRawPN        = NULL;
}

  
PNLabel::~PNLabel(void) {

  if (mFormattedPN) free(mFormattedPN);
  if (mRawPN) free(mRawPN);
}


void PNLabel::setValue(char* str) {

  int numChars;
  int index;
  
  if (str) {                              // Samity, they could pass in a NULL. They do that now and then.
    numChars = strlen(str);               // Ok have something. Lets count 'em.
    if (numChars) {                       // Got some chars then.
      if (mRawPN) {                       // Work on setting up mRawPN as a buffer.
        free(mRawPN);                     // Not NULL? Free it.
      }
      mRawPN = (char*)malloc(numChars+1); // Grab some RAM.
      if (mRawPN) {                       // If we got the RAM..
        index = 0;                        // We'll use this to index mRawPN.
        for(int i=0;i<=numChars;i++) {    // We'll loop though including the EOL.
          switch(str[i]) {                // There may be better ways of doing this,
            case '0'  :                   //  but this makes it realaly obvious what we're doing.
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
            case '\0' : mRawPN[index++] = str[i]; break;  // Stuff in the "filtered" char.
          }
        }
        formatPN();                     // Create the formatted version.
        label::setValue(mFormattedPN);  // Set the label with the formatted string.
        free(mRawPN);                   // Dump mRawPN.
        mRawPN = NULL;                  // Mark it.
        return;                         // Basically, if it was formattable, we formatted it.
      }                                 // We exit now.
    }
  }
  label::setValue(str);                 // Formatter didn't like something, just pass it on.                
}


void PNLabel::formatPN(void) {

  if (mFormattedPN) {
    free(mFormattedPN);
    mFormattedPN = NULL;
  }
  mFormattedPN = (char*)malloc(strlen(mRawPN) + 8);  // b ( ) b - \0;
  switch(mRawPN[0]) {
    case '1'  : formatOne();    break;
    case '*'  : formatStar();   break;
    case '#'  : formatHash();   break;
    default   : formatStd();    break;
  }
}


void PNLabel::formatOne() {

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


void PNLabel::formatStar() {

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


void PNLabel::formatHash() {

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


void PNLabel::formatStd() {

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
