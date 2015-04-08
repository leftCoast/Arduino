
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <multiMap.h>
#include <timeObj.h>

#define DEF_ONTIME 20  // ms


class sparkle : public linkListObj {
  
  public :
            sparkle(int inIndex, colorObj* color=&white, float onTime=DEF_ONTIME, float dormantTime=0, float buildTime=0, float decayTime=0);
            ~sparkle(void);
  
          boolean   checkSparkle(void);
          colorObj  getColor(colorObj * backgroundColor);
          int       getIndex(void);
          sparkle*  getNext(void);
  
  protected :
  
  int          index;
  colorObj     baseColor;
  multiMap     blendMapper;
  timeObj*      lifeTimer;
};
  
  
  
class sparkleList : public linkList, public idler {
    
    public :
                      sparkleList(void);
                      ~sparkleList(void);
                      
                sparkle*  getSparkles(void);
                int       numSparkles(void);
        virtual void      idle(void);
  };
