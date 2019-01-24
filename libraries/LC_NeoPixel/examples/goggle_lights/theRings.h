#include <timeObj.h>
#include <chainPixels.h>


#define FRAME_MS  60

extern colorMultiMap theColors;

class starLight : public pixelGroup,
                  public timeObj {

  public :
                starLight(void);
                
          void  setupColors(void);
  virtual void  draw(void);
  
          bool  init;
};


class portLight : public pixelGroup,
                  public timeObj {

    public :
                portLight(void);
                
          void  setupColors(void);     
  virtual void  draw(void);
  
          bool  init;
};
