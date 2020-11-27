#include <idlers.h>
#include <mechButton.h>



// ************ oneShot class decleration ************


class oneShot  : public idler  {

   public:   
                enum states { preInit, preFire, startFire, firing };

                oneShot(int pinNum,float delayMs=0,float pulseMs=20);
   virtual    ~oneShot(void);

                void   setDelay(float inDelayMs);
                void   setPulse(float inPulseMs);
                void   fire(void);
                void   idle(void);

                int        mPinNum;
                timeObj   delayTimer;
                timeObj   pulseTimer;
                int           mState;
};


// ************ oneShot class code ************


// Constructor. PUts together the inittial stuff, sets defaults.
oneShot::oneShot(int pinNum,float delayMs=0,float pulseMs=20) {

   mPinNum = pinNum;
   setDelay(delayMs);
   setPulse(pulseMs);
   mState = preInit;
}


// Destructor has really nothing to do. Drinks a lot.
oneShot::~oneShot(void) {  }


// Change the delay timer. Resets the state of the oneSHot. 
void oneShot::setDelay(float inDelayMs) {
   delayTimer.setTime(inDelayMs,false);
   if (mState!=preInit) {
      digitalWrite(mPinNum, LOW);
      mState = preFire;
   }
}


// Change the pulse timer. Resets the state of the oneSHot. 
void oneShot::setPulse(float inPulseMs) {
   pulseTimer.setTime(inPulseMs,false);
   if (mState!=preInit) {
      digitalWrite(mPinNum, LOW);
      mState = preFire;
   }
}


// The trigger. Click this, and the sequence starts.
void oneShot::fire(void) {

   delayTimer.start();
   if (mState == preInit) {
      hookup();
      pinMode(mPinNum, OUTPUT);
   }
   mState = startFire;
}


// Idle() this is where all the real work happens. Kinda' like our own slice of loop().
void oneShot::idle(void) {

   switch(mState) {
      case preInit   :
      case preFire   : break;
      case startFire :
         if (delayTimer.ding()) {
            digitalWrite(mPinNum, HIGH);
            pulseTimer.start();
            mState = firing;
         }
      break;
      case firing    :
         if (pulseTimer.ding()) {
            digitalWrite(mPinNum, LOW);
            mState = preFire;
         }
      break;
   }
}



// ************ PROGRAM STARTS HERE ************


oneShot     ourOneShot(13,500,100); // OUr oneShot. You can have as many as you like.
mechButton  ourButton(8);           // Demounced button.


// Not much to do during setup..
void setup(void) {

   Serial.begin(9600);     // If you want serial, start it here.
   pinMode(13, OUTPUT);    // If you -really- must have the oneShot pin off to start, you can do this.
   digitalWrite(13,LOW);   // Otherwise, it won't be initialized until the first time you call shot.
}


// Our loop(). Call idle() to let everyting run. Check the button for firing.
void loop(void) {

   idle();                                         // Give all the idlers time to do their thing.
   if (!ourButton.trueFalse()) {                   // If the button has been grounded..
      ourOneShot.fire();                           // Fire the oneShot.
      Serial.println("bang!");                     // Send out a message.
      while(!ourButton.trueFalse()) {              // While the button is still grounded..
         sleep(10);                                // Run the background tasks. ( Like a friendly delay() that works. )
      }
   }
}
