#ifndef editField_h
#define editField_h

#include "label.h"
#include "idlers.h"
#include "timeObj.h"
#include "litlOS.h"

#include "keystroke.h"


class editField : public label,
                  public idler,
                  public timeObj {

  public:
                editField(char* defText);
                editField(int inLocX, int inLocY, int inWidth,int inHeight,char* defText);
                editField(int inLocX, int inLocY, int inWidth,int inHeight);
  virtual       ~editField(void);

  virtual void    initEditField(void);
  virtual void    insertChar(char theChar);
  virtual void    deleteChar(void);
  virtual void    handleKeystroke(keystroke* inKeystroke);
  virtual void    getCursorPos(int* cursX,int* cursY,int* cursH);
  virtual void    idle(void);
  virtual void    drawSelf(void);

          int     cursorPos;
          bool    cursorOnOff;
};

#endif
