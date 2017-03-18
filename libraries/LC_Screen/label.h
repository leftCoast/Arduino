#ifndef label_h
#define label_h

#include "drawObj.h"

#define DEF_TEXT_SIZE  1  // If not set, what size do we want?
#define CHAR_WIDTH 6      // Actual size is 6 * text size.
#define CHAR_HEIGHT 8     // Actual size is 8 * text size.
#define DEF_PRECISION 2   // Number of digits past decimal point.

#define MAX_PREC			 12							  // #chars - 3 '-','0','.'. The calculator can disp 16.
#define TEMP_BUFF_SIZE 41 + MAX_PREC		// Temp buff for doing text formatting and things.
						  													// Think about what you'll need here. Its important!
						  													// float can get to 3.4xx E+38 add '-', '.' & \0 
						  													// gives 41 chars.

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
  label(int inLocX, int inLocY, int inWidth,int inHeight);
  label(int inLocX, int inLocY, int inWidth,int inHeight, char* inText);
  ~label(void);
  
  void setTextSize(word size);  // 1,2,3.. - Ends up as multiples of 8 pixals.
  void setJustify(word inJustify);
  void setColors(colorObj* tColor);
  void setColors(colorObj* tColor, colorObj* bColor);
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
  
  word        textSize;
  word    	  justify;
  colorObj    backColor;
  colorObj    textColor;
  boolean     transp;
  char*       buff;
  int         prec;      // For formatting floats.
};


#endif

