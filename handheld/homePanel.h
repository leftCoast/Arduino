#ifndef homePanel_h
#define homePanel_h

#include <panel.h>
#include <litlOS.h> 
#include <bmpObj.h> 



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