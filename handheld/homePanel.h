#ifndef homePanel_h
#define homePanel_h

//#include <panel.h>
#include <lilOS.h> 
#include <bmpObj.h> 
//#include <bmpPipe.h>


// *****************************************************
//                      homeScreen
// *****************************************************


class homeScreen : public homePanel {

  public:
          homeScreen(void);
  virtual ~homeScreen(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);

          bmpPipe*   mBackImage;
};



#endif
