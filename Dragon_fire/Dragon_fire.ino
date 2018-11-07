#define master

#include <Adafruit_NeoPixel.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <blinker.h>

#include <neoPixel.h>
#include <chainPixels.h>
#include <liteLine.h>

#include "fireLine.h"

#define NUM_LIGHTS    150 //120 // These two changed to run on battery box.
#define LED_PIN       2   //8
#define PATTERN_LEN   24
#define NUM_PATTERNS  10
#define FRAME_DELAY   17     // How long between frames.
#define WAIT_DELAY    2000   // How long between flames.

neoPixel lightString(NUM_LIGHTS,LED_PIN);
colorObj patternArray[PATTERN_LEN];
fireLine fireLine(&lightString,NUM_LIGHTS,patternArray,PATTERN_LEN,NUM_PATTERNS);

blinker heartbeat;

timeObj frameTimer(FRAME_DELAY);

#ifdef master

#define LIGHT_1 3
#define LIGHT_2 4
#define LIGHT_3 5
#define LIGHT_4 6
#define LIGHT_5 12

#define DELAY_0 10
#define DELAY_1 FRAME_DELAY
#define DELAY_2 FRAME_DELAY * 3

timeObj waitTimer(WAIT_DELAY);

#else

#define MASTER_PIN  6

#endif

int  r; 
int  pIndex;
int  maxIndex;
bool runningFlames;


void setupPatternArray(void) {

  colorMultiMap flameMapper;
  colorObj  aColor;

  aColor = yellow;
  aColor.blend(&black,50);
  flameMapper.addColor(0,&aColor);

  aColor = yellow;
  aColor.blend(&red,50);
  aColor.blend(&black,50);
  flameMapper.addColor(2,&aColor);

  aColor = red;
  aColor.blend(&black,70);
  flameMapper.addColor(4,&aColor);

  aColor = red;
  aColor.blend(&black,95);
  flameMapper.addColor(PATTERN_LEN-6,&aColor);
  flameMapper.addColor(PATTERN_LEN,&aColor);

  for(byte i=0;i<PATTERN_LEN;i++) {
    patternArray[i] = flameMapper.Map(i);
  }

  // For testing
  //patternArray[0] = red;
  //patternArray[1] = green;
  //patternArray[2] = blue;
  
}


void setup() {

  //Serial.begin(9600);
  heartbeat.setBlink(true);
  maxIndex = fireLine.getMaxIndex();
  lightString.begin();
  setupPatternArray();
  lightString.setAll(&black); 
  lightString.show();

  #ifdef master

    pinMode(LIGHT_1, OUTPUT);
    digitalWrite(LIGHT_1, LOW);
    
    pinMode(LIGHT_2, OUTPUT);
    digitalWrite(LIGHT_2, LOW);
    
    pinMode(LIGHT_3, OUTPUT);
    digitalWrite(LIGHT_3, LOW);
    
    pinMode(LIGHT_4, OUTPUT);
    digitalWrite(LIGHT_4, LOW);
    
    pinMode(LIGHT_5, OUTPUT);
    digitalWrite(LIGHT_5, LOW);

    waitTimer.start();
  #else

    pinMode(MASTER_PIN, INPUT);
    delay(100);                  // Wait to make sure the master has the pins shut down.
  #endif
  
  runningFlames = false;
}


void startLights(void) {

  pIndex = 0;
  lightString.setAll(&black);
  fireLine.setLights(pIndex++);
  runningFlames = true;
  frameTimer.start();
}


#ifdef master

void startAllLights(void) {

  digitalWrite(LIGHT_1, HIGH);  // Get everyone's attention.
  digitalWrite(LIGHT_2, HIGH);
  digitalWrite(LIGHT_3, HIGH);
  digitalWrite(LIGHT_4, HIGH);
  digitalWrite(LIGHT_5, HIGH);
  delay(DELAY_0);
  digitalWrite(LIGHT_2, LOW);   // Bring 'em on staggered.
  digitalWrite(LIGHT_3, LOW);
  delay(DELAY_1);
  digitalWrite(LIGHT_1, LOW);
  digitalWrite(LIGHT_4, LOW);
  delay(DELAY_2);
  startLights();                // Our own..
  digitalWrite(LIGHT_5, LOW);
}

#endif


void loop() {

  idle();           // Run stuff.
  r = random();     // Randomize the randoms.

#ifdef master

  if (!runningFlames) {
    if (waitTimer.ding()) {
       startAllLights();
    }
  }
  
#else

  if (digitalRead(MASTER_PIN)==HIGH) {
    while(digitalRead(MASTER_PIN)==HIGH); // Wait for a low
    startLights();
  }
  
#endif

  if (runningFlames) {
    if (frameTimer.ding()) {
      lightString.show();
      frameTimer.start();
      lightString.setAll(&black);
      fireLine.setLights(pIndex++);
      if (pIndex>maxIndex+1) {     // We already loaded in the last one so we need another to fire it off.
        runningFlames = false;
 #ifdef master       
        waitTimer.start();
 #endif      
      }
    }
  }
}
