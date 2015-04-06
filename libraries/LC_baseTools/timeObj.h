#ifndef timeObj_h
#define timeObj_h

// Insainly handy for doing things in the background.
// Set the timer and wait 'till it goes "ding". 
// Great for blinking LEDs, updating readings, etc.
// Not fast & accurate enough for RC Servos.

class timeObj {

public:
  timeObj(float inMs=10);

  virtual void setTime(float inMs,bool startNow=true);    // Change the time duration for next start..
  void start(void);                                       // Start the timer "now".
  void stepTime(void);                                    // Restart the timer from last end time.
  bool ding(void);                                        // Timer has expired.
  unsigned long getTime(void);                            // How long does this go for?
    
private:
  unsigned long waitTime;
  unsigned long startTime;
  unsigned long endTime;
  bool       crossing;
};

#endif

