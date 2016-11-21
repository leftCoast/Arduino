#ifndef fireLine_h
#define fireLine_h

#include <mapper.h>
#include <colorObj.h>
#include <liteLine.h>

class fireLine : public liteLine {
  
  public:
    fireLine(neoPixel* inLites,int inLength);
    ~fireLine(void);
            void     begin(void);
    virtual colorObj calcColor(int index,int i);
    virtual void     setLights(int index,boolean wrap=false);
};

#endif
