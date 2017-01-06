#ifndef label_h
#define label_h

#include "drawObj.h"

#define DEF_TEXT_SIZE  1  // If not set, what size do we want?
#define CHAR_WIDTH 6      // Actual size is 6 * text size.
#define CHAR_HEIGHT 8     // Actual size is 8 * text size.
#define DEF_PRECISION 2   // Number of digits past decimal point.

enum {
  TEXT_RIGHT,
  TEXT_LEFT,
  TEXT_CENTER
};


class label : 
public drawObj {

public :
  label();
  label(char* inText);
  label(char* inText, word inSize);
  label(word inLocX, word inLocY, int inWidth,int inHeight);
  label(word inLocX, word inLocY, int inWidth,int inHeight, char* inText);
  ~label();
  
  void setTextSize(word size);  // 1,2,3.. - Ends up as multiples of 8 pixals.
  void setJustify(word inJustify);
  void setColors(colorObj tColor);
  void setColors(word tColor);
  void setColors(colorObj textColor, colorObj backColor);
  void setColors(word textColor, word backColor);
  void setPrecision(int inPrec);
  void setValue(int val);
  void setValue(unsigned long val);
  void setValue(float val);
  void setValue(double val);
  void setValue(char* str);

  word getTextWidth(void);    // How wide in pixels is our text?
  word getTextHeight(void);   // How tall in pixels are the charactors?
  
  virtual void drawSelf(void);

//private :
  void initLabel(void);
  void freeBuff(void);
  
  word    textSize;
  word    justify;
  word    backColor;
  word    textColor;
  boolean transp;
  char*   buff;
  int     prec;      // For formatting floats.
};


#endif

