#include <timeObj.h>
#include <idlers.h>

#include <Adafruit_LEDBackpack.h>

//Base class just adds startSprit() & active() to idler.
class sprit : public idler {
  
  public :
                    sprit(void);
                    ~sprit(void);
    
    virtual void      startSprit(void);
    virtual boolean   active(void);
    
    boolean spritRunning;
};


class sprit8x8 : public sprit {
  
  public :
                      sprit8x8(Adafruit_8x8matrix* inMatrix);
                      ~sprit8x8(void);
    
    virtual void      startSprit(void);
    virtual boolean   active(void);
    virtual void      idle(void);
    virtual void      showFrame(const uint8_t* bitmap,unsigned long inMs);
    virtual boolean   changeFrame(void);
            
            Adafruit_8x8matrix* matrix;
            boolean             starting;
            timeObj             frameTimer;
            int                 frameNum;
            int                 numFrames;
};
