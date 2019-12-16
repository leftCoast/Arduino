#ifndef cEditPanelII_h
#define cEditPanelII_h

#include "cClockOS.h"


// *****************************************************
//                      cEditPanelII
// *****************************************************


class cEditPanelII : public panel {

  public:
          enum        cEditComs { noCommand, help, setColor, getColor, adjRed, adjGreen, adjBlue, exit };
          enum        primaryColors { pRed, pGreen, pBlue };
          
          cEditPanel(void);
  virtual ~cEditPanel(void);
  
  virtual void        setup(void);
  virtual void        loop(void);
  virtual void        drawSelf(void);
  virtual void        checkParse(void);
          void        doGetColor(void);
          void        doSetColor(void);
          void        doAdjColor(primaryColors pColor);
          
          lilParser   mParser;
};



#endif
