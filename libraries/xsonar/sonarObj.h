#ifndef sonarObj_h
#define sonarObj_h

#include "idlers.h"
#include "timeObj.h"

// As this is written you can only use pin one or two.
// Forget why. I'm betting its something to do with
// intrupt handlers or timers or something.

class sonarObj : public idler {

public:
  sonarObj(int inPin);
  ~sonarObj(void);
  
  virtual void idle(void);
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
  timeObj*      timeoutTimer;
  timeObj*      endTimer;
  bool          isAttached;
};

#endif
