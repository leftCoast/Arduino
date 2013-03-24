#ifndef sonarObj_h
#define sonarObj_h

#include "timeObj.h"

class sonarObj {

public:
  sonarObj(int inPin);
  ~sonarObj(void);
  
  void idle(void);
  float distanceCm(void);

private:
  void attachHandler(void);
  void detachHandler(void);
  bool pulseDone(void);
  long readTime();
  void clearTime(void);
  
  int           state;
  int           pin;
  float         result;
  timeObj*      endTimer;
  bool          isAttached;
};

#endif
