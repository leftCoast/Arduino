#ifndef toolsPanel_h
#define toolsPanel_h

#include <panel.h>

class toolsPanel;

// *****************************************************
// ******************  toolsCloseBtn  *******************
// *****************************************************


class toolsCloseBtn :  public closeBtn {

   public:
          toolsCloseBtn(toolsPanel* ourPanel);
  virtual ~toolsCloseBtn(void);

  virtual void  doAction(void);

          toolsPanel* mPanel;
};



// *****************************************************
// ******************  toolsPanel  *******************
// *****************************************************


class toolsPanel :  public panel {

  public:
          toolsPanel(void);
  virtual ~toolsPanel(void);
           
  virtual void    setup(void);
  virtual void    loop(void);
          void    showStatus(void);
  virtual void    drawSelf(void);
  virtual void    closing(void);

          timeObj     statusTimer;
          textView*   mText;
          battPercent*  mBatPct;
          RSSIicon*     mRSSI;
};

#endif
