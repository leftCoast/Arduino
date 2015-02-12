#include <PinChangeInt.h>
#include <RCReciver.h>

#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>

#include <Adafruit_NeoPixel.h>

#include <colorObj.h>
#include <liteLine.h>
#include <neoPixel.h>


// Single LED that runs in background.
class sparkle : public idler {
  
  public:
    sparkle(neoPixel* inLights,word inLoc,float inTime=5);
    ~sparkle(void);
    
    void     setColor(colorObj* inColor);
    void     setTime(float inTime);
    boolean  getState(void);
    void     flash(void);
    void     fade(void);
     
  protected:
    neoPixel* lights;
    word      loc;
    colorObj  color;
    colorObj  savedColor;
    timeObj   onTime;
    boolean   lightOn;
};


sparkle::sparkle(neoPixel* inLights,word inLoc,float inTime) {
   
   lights = inLights;
   loc    = inLoc;
   onTime.setTime(inTime);
   lightOn = false; 
 }
  
 
sparkle::~sparkle(void) {  }


void sparkle::setColor(colorObj* inColor) {

  if (lightOn)
    fade();
  color.setColor(inColor);
}


void sparkle::setTime(float inTime) {

  if (lightOn)
    fade();
  onTime.setTime(inTime);
}
  

boolean sparkle::getState(void) { return lightOn; }


void sparkle::flash(void) { 

  savedColor = lights->getPixelColor(loc);
  lights->setPixelColor(loc,&color);
  onTime.start();
  lightOn = true;
}


void sparkle::fade(void) { 

  lights->setPixelColor(loc,&savedColor);
  lightOn = false;
}



// ************************************



class randomFlash : public sparkle {
  
  public:
  randomFlash(neoPixel* inLights,word inLoc);
  
  virtual void idle(void);
};


randomFlash::randomFlash(neoPixel* inLights,word inLoc) : sparkle(inLights,inLoc) { }
  
  
void randomFlash::idle(void) {

  if (lightOn && onTime.ding()) {
    fade();
  } else if (!lightOn && (random(50)==12)) {
    flash();
  }
}


#define LIGHTSTICK_LEDS  8
#define LIGHTSTICK_PIN   3
#define AUX1_PIN         4
#define MIN_PIN          1000
#define MID_PIN          1500
#define MAX_PIN          2000
#define PIN_TOLERANCE    170

reciverPin aux1(AUX1_PIN);

neoPixel lightStick(LIGHTSTICK_LEDS,LIGHTSTICK_PIN);

colorObj red(RED);      // FUll red & blue
colorObj blue(BLUE);
colorObj white(WHITE);
colorObj black(BLACK);

randomFlash* bulbs[8];

boolean lightState = false;

colorObj chooseColor(void) {
  
  long val = random(0,9);
  switch (val) {
    case 0 : 
    case 1 :
    case 2 :
    case 3 : return red; 
    case 4 :
    case 5 :
    case 6 :
    case 7 : return blue;
    case 8 : return white;
  }
}


  
void setup(void) {
  
  //Serial.begin(9600);
  
  for (int i=0;i<LIGHTSTICK_LEDS;i++) {
    bulbs[i] = new randomFlash(&lightStick,i);
    bulbs[i]->setColor(&black);
    bulbs[i]->hookup();
  }
  lightStick.begin();
  lightState = false;
}


void swapColors(void) {
  
  if (random(0,300)==10) {
    int index = random(0,9);
    if (!bulbs[index]->getState()) {
       int time  = random(10,50);
       bulbs[index]->setColor(&(chooseColor()));
       bulbs[index]->setTime(time);
    }
  }
}
  
  
  
boolean checkState(void) {
  
  unsigned long aux1Val = aux1.pinResult();
  
  //Serial.print("Val : ");Serial.print(aux1Val);Serial.print(" ");
  if (aux1Val>=(MID_PIN-PIN_TOLERANCE) && aux1Val<=(MID_PIN+PIN_TOLERANCE)) {  // Switch is on.
  //if (true) {
    //Serial.println("Switch On");
    if (!lightState) {                                                         // Lights are not on.
     for (int i=0;i<LIGHTSTICK_LEDS;i++) {                                     // Fire 'em up!
        bulbs[i]->setColor(&(chooseColor()));
     }
     lightState = true;                                                        // On now!
    }
  } else {                                                                     // Switch is off.
    //Serial.println("Switch Off");
    if (lightState) {                                                          // But the lights are on..
      for (int i=0;i<LIGHTSTICK_LEDS;i++) {                                    // Shut them off!
        bulbs[i]->setColor(&black);
      }
      lightState = false;                                                      // Off now! 
    }      
  }
  return lightState;                                                           // Return the resulting state.
}

  
void loop(void) {
  
 
  if (checkState()) {                        // Lights on?
    swapColors();                            // Allow color changes.
    idle();                                  // Give 'em some time to run.
  }
  lightStick.show();                        // In any case, make sure we see it.
}


