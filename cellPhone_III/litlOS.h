#ifndef ltlOS_h
#define ltlOS_h

#include <colorObj.h>

#define SYS_TEDIT_BCOLOR      0
#define SYS_TEDIT_TCOLOR      1
#define SYS_TEDIT_HCOLOR      2

#define SYS_PANEL_COLOR       3
#define SYS_LABEL_BTN_COLOR   4
#define SYS_CONTROL_BTN_COLOR 5
#define SYS_CONTROL_LBL_COLOR 6
#define SYS_SHADOW_COLOR      7

#define NUM_SYS_COLORS        8   // Well, look up there and count 'em.




class litlOS {

  public:
  
          litlOS(void);
  virtual ~litlOS(void);

          colorObj  getColor(byte colorIndex);
          colorObj  systemPalette[NUM_SYS_COLORS];
};

extern  litlOS ourOS;   // As it says, this is our OS. For better or worse..


#endif
