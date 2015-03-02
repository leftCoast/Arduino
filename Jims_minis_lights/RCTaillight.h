
#ifndef RCTaillight_h
#define RCTaillight_h

#include <chainPixels.h>

#define DEF_NUM_PIXELS 8

class RCTaillight : public pixelGroup {
  
  public :
  
          RCTaillight(int leftYaw,
              int rightYaw,
              int throttleLow,
              int throttleHi,
              int rollPort,
              int rollStarboard,
              int pitchDn,
              int pitchUp,
              boolean reverse,
              int numPixels=DEF_NUM_PIXELS);
              
          ~RCTaillight(void);  

          void setValues(int inYaw, int inThrottle, int inRoll, int inPitch);
  virtual void       draw(void);    
      
      protected :
 
        int yaw;          // This block is raw current value.
        int throttle;
        int roll;
        int pitch; 
        
        int oldYaw;       // This block is last value.
        int oldThrottle;
        int oldRoll;
        int oldPitch; 
        
        mapper pitchPercent;
        mapper throttlePercent;
        mapper rollPercent;
        mapper yawPercent;
};


#endif

