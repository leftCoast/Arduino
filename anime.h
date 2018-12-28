
#ifndef anime_h
#define anime_h

#include <colorObj.h>
#include <timeObj.h>
#include <idlers.h>
#include <drawObj.h>

class anime : public idler, public drawObj, public timeObj, public colorMultiMap {

  public:
        anime(word inX,word inY,word inWidth,word inHeight,float inTime);

        void     begin(void);
        colorObj calcColor(word inX);
virtual void     drawSelf(void);
virtual void     idle(void);

        word     offset;
};

#endif
