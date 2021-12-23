#ifndef testAppPanel_h
#define testAppPanel_h

#include <lilOS.h>
#include <stdComs.h>
#include <flasher.h>
#include <label.h>
#include <alertObj.h>
#include <fSelectObj.h>
#include <fSaveObj.h>


class testAppPanel : public panel {

   public:
               testAppPanel(lilOS* ourOS,int ourAppID);
   virtual     ~testAppPanel(void);

   virtual  void  setup(void);
   virtual  void  loop(void);
   virtual  void  drawSelf(void);
   virtual  void  closing(void);
   virtual  void  handleCom(stdComs comID);

            fSelectObj* choosAlert;
            fSaveObj*   saveAlert;
            flasher*    aFlasher;
            label*      choosenPath;
            bool        needRefresh;
};

#endif
