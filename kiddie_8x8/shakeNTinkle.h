#include <idlers.h>
#include <timeObj.h>
#include <runningAvg.h>

/*
This is all about sensing movement.
Adafruit sells two movement sensors, one more and one less sensitive.
Easy to trigger (shake)  https://www.adafruit.com/products/1766
Harder to trigger (tinkle) https://www.adafruit.com/products/1767

Here's what we do with them. Every time idle() is called, typically each time through the event loop,
we look to see if we have a trigger on each of these pins, shakePin and tinklePin. Triggering would be a
pull to ground. If so, we hold 'till the trigger event is clear and add one to our accumulator,
shakeSum or tapSum. We continue doing this for the amount of milliseconds that is given by sumTime.

After sumTime expires, we take each sum, shakeSum and tapSum, and add them to their respective running
average objects, shake and tinkle. The amount of data values each running average object uses is given by
the inputted value numData.

Why in the world do we do all of this?
The sensors only give instant binary readouts. This is good for, say.. Turning something on and off
but that's about it. Also, you need to watching the input pin when it happens. By counting the amount 
of triggers over a given time period. (integrating) We can get a more analog value representing how
much vibration is going on. By using the two different types of sensors we can get an idea of the
type of vibration as well. We run this through the running averagers to smooth the values through
time and to allow more time for the main program to notice. Smooth curve in time as opposed to random
glitches.

So, with that being said heres your input parameters.
int inShakePin   : pin number for easy to trigger sensor.
int inTinklePin  : pin number for harder to trigger sensor.
float sumTime    : Amount of time, in milliseconds, to accumulate trigger events. 
int numData      : Number of data values used in each averager. Carefull here, eats memory with large numbers.
*/

// The default values to get you up and running. You will probably want to overwrite them 
// once you get things running to match your application. Dont't change them here, just add the two values
// to the end of the constructor when you create yours and your values will be used instead.
#define DEF_SUMTIME  50  //ms - Can be thought of as sensitivity.
#define DEF_NUMDATA  10  //values per averager. - Can be thought of as capacitance.
// NOTE Multiply the two values gives the approximate time, in ms, for a set of trigger events to die down to zero.

class shakeNTinkle : public idler {
  
  public :
  
  shakeNTinkle(int inShakePin,int inTinklePin,float sumTime=DEF_SUMTIME,int numData=DEF_NUMDATA);
  ~shakeNTinkle(void);
  
  void   begin(void);
  virtual void idle(void);
  float  getShake(void);
  float  getTinkle(void);
  
  protected:
  int       shakeSum = 0;
  int       tapSum = 0;
  int       shakePin;
  int       tinklePin;
  timeObj*  tapTimer = NULL;
  runningAvg* shake = NULL;
  runningAvg* tinkle = NULL;
};

