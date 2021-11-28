#ifndef menuBar_h
#define menuBar_h

#include <drawObj.h>
#include <stdComs.h>

#define MENU_BAR_H    24  // Because we have 22x22 icos to stick on it. So there!
	
#define CLOSE_X         0
#define CLOSE_Y         1
#define CLOSE_SIZE      icon22

extern colorObj	menuBarColor;

	
// *****************************************************
//								menueBar
// *****************************************************

class panel;

class menuBar  : public drawGroup {

  public:
            menuBar(panel* inPanel,bool closeBox=true);
    virtual ~menuBar(void);

    virtual void    drawSelf(void);

            panel*  mPanel;
};


#endif