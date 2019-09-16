#ifndef baseButton_h
#define baseButton_h

#include "switchable.h"
#include "floraOS.h"
#include <bmpPipe.h>

class baseButton : public switchable, public idler {
  
  public :
                  baseButton(char* inLabel,int x, int y,int width, int height);
  virtual         ~baseButton(void);

  virtual bool    acceptEvent(event* inEvent,point* locaPt);  // Is this event for us?
  virtual void    setText(char* intext);
  virtual void    idle(void);
  virtual void    drawSelf(void);

                  colorObj  BGColor;
                  colorObj  activeBColor;
                  colorObj  activeTColor;
                  colorObj  clickedBColor;
                  colorObj  clickedTColor;
                  
          char*   label;
};


class baseIconButton :  public baseButton,
                        public bmpPipe {

  public :
               
                  baseIconButton(int x,int y,int width,int height,char* filePath);
  virtual         ~baseIconButton(void);
  
  virtual void    setText(char* intext);
  virtual void    drawSelf(void);

          char*   mPath;
};

#endif
