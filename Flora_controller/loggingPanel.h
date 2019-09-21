#ifndef loggingPanel_h
#define loggingPanel_h

#include <panel.h>
#include <litlOS.h> 
#include <bmpObj.h> 
#include "tools.h"


// *****************************************************
//                      loggingPanel
// *****************************************************


class loggingPanel :  public panel {

  public:
          loggingPanel(void);
  virtual ~loggingPanel(void);
          
  virtual void  setup(void);
  virtual void  drawSelf(void);
};


#endif
