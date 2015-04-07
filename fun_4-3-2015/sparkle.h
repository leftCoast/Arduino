
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <multiMap.h>
#include <timeObj.h>

#define DEF_ONTIME 20  // ms


class sparkle : public linkListObj, idler {
  
  public :
            sparkle(int inIndex, colorObj* color=&white, float onTime=DEF_ONTIME, float dormantTime=0, float buildTime=0, float decayTime=0);
            ~sparkle(void);
  
  virtual void      idle(void);
          boolean   checkSparkle(void);
          colorObj  getColor(colorObj * backgroundColor);
  
  protected :
  
  int          index;
  colorObj     baseColor;
  multiMap     blendMapper;
  timeObj*      lifeTimer;
};
  
  
  
