#ifndef cEditPanel_h
#define cEditPanel_h

#include "cClockOS.h"


// *****************************************************
//                      cEditPanel
// *****************************************************


class cEditPanel : public panel {

  public:
          enum        cEditComs { noCommand, help, setColor, getColor, cout, cin, exit };
          
          cEditPanel(void);
  virtual ~cEditPanel(void);
  
  virtual void        setup(void);
  virtual void        loop(void);
  virtual void        drawSelf(void);
  virtual void        checkParse(void);
          void        doGetColor(void);
          void        doSetColor(void);
          void        doCout();
          void        doCin();
          
          lilParser   mParser;
          colorRect*  mColorRects[24];
};

#endif
