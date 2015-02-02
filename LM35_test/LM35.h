#ifndef LM35_H
#define LM35_H

#include <mapper.h>

class LM35 {
  
  public:
    LM35(int inPin);
    ~LM35(void);
    
    float centToDegF(float cent);
    float tempC(void);
    float tempF(void);
    
 private:
    int     pin;
    mapper* intToVolts;
    mapper* voltsToTemp;
};

#endif
