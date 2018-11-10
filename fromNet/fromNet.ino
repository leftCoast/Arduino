#include <Adafruit_NeoPixel.h>
#define PIN 2  //  6
#define Pixels 60

Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);




// Set 'em all like this. Good tool to have.
void setAllPixles(byte red, byte blue,byte green) {

  for (int i=0;i<Pixels;i++) {
    strip.setPixelColor(i, red, green, blue);
  }
}


float fadeRate = 0.9;  // According to what I see, anything > 1 will blow up.
byte red;             // This is the guy that is being messed with.
int center = 30;      // Center is at pixel 30.
int cOffset;          // How far from center are we?  

void setup() {
  
  strip.begin();        // Fire it up!
  setAllPixles(0,0,0);  // Really, force it all to black.
  strip.show();         // Initialize all pixels to 'off'
  cOffset = 0;          // We start at the center.
}


byte  calculateRed(void) {

  float floatRed = red*pow(fadeRate, cOffset);
  return round(floatRed);
}


void setPixels(void) {

  byte  newRed;
  
  setAllPixles(0,0,0);    // blank it out
  newRed = calculateRed();
  if (cOffset==0) {
    strip.setPixelColor(center,newRed,0,0);
  } else {
    strip.setPixelColor(center+cOffset,newRed,0,0);
    strip.setPixelColor(center-cOffset,newRed,0,0);
  }
  strip.show();
}


void loop () {
  
  

   if (cOffset==0) {
      randomSeed(analogRead(0));  // Really, make it random.
      red = random(256);          // Start with new version of red.
    }
    setPixels();
    
    if (cOffset<30) {
      cOffset++;
    } else {
      cOffset = 0;
    }
    delay(150);
}
