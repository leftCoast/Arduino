#ifndef baseButton_h
#define baseButton_h

#include "switchable.h"
#include "floraOS.h"

class baseButton : public switchable, public idler {
  
  public :
                  baseButton(char* inLabel,int x, int y,int width, int height);
  virtual         ~baseButton(void);

  virtual bool    acceptEvent(event* inEvent,point* locaPt);  // Is this event for us?
  virtual void    idle(void);
  virtual void    drawSelf(void);

                  colorObj  BGColor;
                  colorObj  activeBColor;
                  colorObj  activeTColor;
                  colorObj  clickedBColor;
                  colorObj  clickedTColor;
                  
          char*   label;
};

#endif
