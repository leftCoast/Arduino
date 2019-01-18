#ifndef editField_h
#define editField_h

#include "label.h"
#include "idlers.h"
#include "timeObj.h"
//#include "litlOS.h"

#include "keystroke.h"

// It works for now. But it needs work. Things like writing beyond
// its window isn't dealt with. Etc. Sop not ready for prime time
// but for getting stuff into a prototype? Sure. Just be careful.


class editField : public label,
                  public idler,
                  public timeObj {

  public:
                editField(rect* inRect,char* inText,word inTextSize);
                editField(int inLocX, int inLocY, int inWidth,int inHeight,char* inText,word inTextSize);
  virtual       ~editField(void);

  virtual void    setValue(char* str);
  virtual void    insertChar(char theChar);
  virtual void    deleteChar(void);
  virtual void    handleKeystroke(keystroke* inKeystroke);
  virtual void    getCursorPos(int* cursX,int* cursY,int* cursH);
  virtual void    idle(void);

          int     cursorPos;
          bool    cursorOnOff;
};

#endif
