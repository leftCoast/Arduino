#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <timeObj.h>

#define DEF_ONTIME 20  // ms

enum sparkleState { dormant, building, glowing, decaying, dead };

class sparkle : public linkListObj, idler {
  
  public :
            sparkle(int inIndex, colorObj* color=&white, float onTime=DEF_ONTIME, float dormantTime=0, float buildTime=0, float decayTime=0);
            ~sparkle(void);
  
  virtual void      idle(void);
          boolean   checkSparkle(void);
          colorObj  getColor(colorObj * backgroundColor);
  
  protected :
  
  int           index;
  colorObj      baseColor;
  timeObj*      onTimer = NULL;
  timeObj*      dormantTimer = NULL;
  timeObj*      buildTimer = NULL;
  timeObj*      decayTimer = NULL;
  mapper*       blendPercent = NULL;
  sparkleState  state = dormant;
};


  
  
  
