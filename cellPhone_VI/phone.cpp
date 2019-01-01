
#include "phone.h"
#include <label.h>
#include "litlOS.h"


#define KEY_W       40
#define KEY_H       30

#define ROW_GAP     50

#define KEY_R0      25
#define KEY_R1      KEY_R0 + ROW_GAP
#define KEY_R2      KEY_R1 + ROW_GAP
#define KEY_R3      KEY_R2 + ROW_GAP
#define KEY_R4      KEY_R3 + ROW_GAP
#define KEY_R5      KEY_R4 + ROW_GAP

#define COL_GAP     70

#define KEY_C1      30
#define KEY_C2      KEY_C1 + COL_GAP
#define KEY_C3      KEY_C2 + COL_GAP

#define KEY_RAD     4

#define TEXT_SIZE   2
#define CHAR_WIDTH  TEXT_SIZE * 6
#define TEXT_HEIGHT TEXT_SIZE * 8



// *****************************************************
// ********************  phoneBtn  *********************
// *****************************************************


phoneBtn::phoneBtn(int x,int y,char inKey,phone* inPhone)
  : drawObj(x,y,KEY_W,KEY_H,true) {
  
  mKeystroke[0] = inKey;
  mKeystroke[1] = '\0';
  mPhone = inPhone;
  mPhone->addObj(this);    // If we add to our phone object, we'll be auto deleted when its recycled.
  if (mKeystroke[0]=='C') {
    width = width + COL_GAP;
  }
}


phoneBtn::~phoneBtn(void) {  }


void phoneBtn::drawSelf(void) {

  if (mKeystroke[0]=='X') {
    if (clicked) {
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
      screen->setTextColor(&red);
    } else {
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&red);
      screen->drawRoundRect(x,y,width,height,KEY_RAD,&black);
      screen->setTextColor(&black);
    }
  } else if (mKeystroke[0]=='C') {
    if (clicked) {
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
      screen->setTextColor(&green);
    } else {
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&green);
      screen->drawRoundRect(x,y,width,height,KEY_RAD,&black);
      screen->setTextColor(&white);
    }
  } else {
    if (clicked) {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&blue);
      screen->setTextColor(&white);
    } else {
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
      screen->setTextColor(&white);
    }
  }
  screen->setTextSize(TEXT_SIZE);
  screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
  if (mKeystroke[0]=='C') {
    screen->drawText("Call");
  } else {
    screen->drawText(mKeystroke); 
  }
}


void phoneBtn::doAction(void) { mPhone->keystroke(mKeystroke[0]); }



// *****************************************************
// *********************  phone   **********************
// *****************************************************


phone::phone(void) 
  : panel(phoneApp,false) {

  mRawPN = NULL;
  mFormattedPN = NULL;
}


phone::~phone(void) { 
  
  if (mRawPN) free(mRawPN);
  if (mFormattedPN) free(mFormattedPN);
}


void phone::setup(void) {

  pBtndel   = new phoneBtn(KEY_C3,KEY_R0,'D',this);
  
  pBtn7     = new phoneBtn(KEY_C1,KEY_R1,'7',this);
  pBtn8     = new phoneBtn(KEY_C2,KEY_R1,'8',this);
  pBtn9     = new phoneBtn(KEY_C3,KEY_R1,'9',this);
  
  pBtn4     = new phoneBtn(KEY_C1,KEY_R2,'4',this);
  pBtn5     = new phoneBtn(KEY_C2,KEY_R2,'5',this);
  pBtn6     = new phoneBtn(KEY_C3,KEY_R2,'6',this);
  
  pBtn1     = new phoneBtn(KEY_C1,KEY_R3,'1',this);
  pBtn2     = new phoneBtn(KEY_C2,KEY_R3,'2',this);
  pBtn3     = new phoneBtn(KEY_C3,KEY_R3,'3',this);
  
  pBtn0     = new phoneBtn(KEY_C1,KEY_R4,'0',this);
  pBtnStar  = new phoneBtn(KEY_C2,KEY_R4,'*',this);
  pBtnHash  = new phoneBtn(KEY_C3,KEY_R4,'#',this);
  
  pBtnCall  = new phoneBtn(KEY_C1,KEY_R5,'C',this);
  pBtnClose = new phoneBtn(KEY_C3,KEY_R5,'X',this);
  numDisplay  = new label(KEY_C1,KEY_R0,KEY_W+COL_GAP,KEY_H,"",1);
  addObj(numDisplay);
}


void phone::loop(void) {  }

void phone::drawSelf(void) {

  screen->fillScreen(&white);
}


void phone::keystroke(char inKey) {

  switch(inKey) {
    case '0'   :
    case '1'   :
    case '2'   :
    case '3'   :
    case '4'   :
    case '5'   :
    case '6'   :
    case '7'   :
    case '8'   :
    case '9'   : 
      addChar(inKey);
      formatPN();
      numDisplay->setValue(mFormattedPN);
    break;
    case 'D'   :
      deleteChar();
      formatPN();
      numDisplay->setValue(mFormattedPN);
    break;
    case 'C'   : call();   break;
    case 'X'   : 
      hangup();
      close();
    break;
  }  
}


void phone::addChar(char keyStroke) {
  
  int   numChars;
  char* newStr;
  
  if (!mRawPN) {
    mRawPN = (char*)malloc(2);
    mRawPN[0] = keyStroke;
    mRawPN[1] = '\0';
  } else {
    numChars = strlen(mRawPN);
    newStr = (char*)malloc(numChars+2);
    strcpy(newStr,mRawPN);
    newStr[numChars] = keyStroke;
    newStr[numChars+1] = '\0';
    free(mRawPN);
    mRawPN = newStr;
  }
}


void phone::deleteChar(void) { mRawPN[strlen(mRawPN)-1] = '\0'; }


void phone::formatOne() {

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


void phone::formatStar() {

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


void phone::formatHash() {

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


void phone::formatStd() {

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

    
void phone::formatPN(void) {

  if (mFormattedPN) {
    free(mFormattedPN);
    mFormattedPN = NULL;
  }
  mFormattedPN = (char*)malloc(strlen(mRawPN) + 6);  // b ( ) b - \0;
  switch(mRawPN[0]) {
    case '1'  : formatOne();    break;
    case '*'  : formatStar();   break;
    case '#'  : formatHash();   break;
    default   : formatStd();    break;
  }
}


void phone::call(void) {

  
}


void phone::hangup(void) {

  
}
