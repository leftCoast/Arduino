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

            char* iconPath(int appID,const char* iconName);
   virtual  void  setup(void);
   virtual  void  loop(void);
            void  doStarField(void);
   virtual  void  drawSelf(void);

            char  pathBuff[80];
};



#endif
