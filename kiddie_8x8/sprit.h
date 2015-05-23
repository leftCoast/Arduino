#ifndef sprit_h
#define sprit_h

#include <timeObj.h>
#include <idlers.h>

#include <Adafruit_LEDBackpack.h>

static const uint8_t PROGMEM
off_bmp[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},
on_bmp[] =
{ B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111
};


//Base class just adds startSprit() & active() to idler.
class sprit : public idler {
  
  public :
                    sprit(void);
    
    virtual void      startSprit(void);
    virtual void      stopSprit(void);
    virtual boolean   active(void);
    
  protected :
  
    boolean spritRunning;
};


// For showing bitmaps stored in program memory.
class bitmap8x8 : public sprit {
  
  public :
                      bitmap8x8(Adafruit_8x8matrix* inMatrix);  // Set numFrames in here.
    
    virtual void      startSprit(void);
    virtual void      idle(void);
    virtual void      showFrame(void);                        // fill in this to call showBitmap()
    virtual void      showBitmap(const uint8_t* bitmap,unsigned long Ms,boolean inverse=false);
      
  protected :       
            Adafruit_8x8matrix* matrix;
            timeObj             frameTimer;
            int                 frameNum;
            int                 numFrames;
};


// For scrolling text. Use a leading space so it'll
// scroll onto the screen.
class text8x8 : public sprit {
   
  public :
                      text8x8(Adafruit_8x8matrix* inMatrix,char* inStr, float inStepDelay);
                      ~text8x8(void);
    
    virtual void      startSprit(void);
    virtual void      idle(void);
  
  protected :
  
            Adafruit_8x8matrix* matrix;
            char*               theStr;
            timeObj             stepTimer;
            float               stepDelay;
            int                 stepNum;
            int                 endNum;
};

#endif // sprit_h
