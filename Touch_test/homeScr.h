#ifndef homePanel_h
#define homePanel_h

#include "ourOSObj.h"
#include <bmpObj.h> 


// *****************************************************
//                        homeScr
// *****************************************************


class homeScr : public panel {

   public:
            homeScr(void);
   virtual  ~homeScr(void);

            char* iconPath(int appID,char* iconName);
   virtual  void  setup(void);
   virtual  void  loop(void);
   virtual  void  drawSelf(void);

            char  pathBuff[80];
};



#endif
