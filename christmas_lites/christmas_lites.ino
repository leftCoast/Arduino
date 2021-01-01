#include "sparkles.h"

/*
I need to pull out a char from a buffer. So, I setup my readChar() method as it would like. Giving it somewhere to toss the char. The char IS ignored. Either way works. But this way I get these warnings.

The real issue is that to an un-informed user, the warning seem to come out of the blue. This is because of the standard way the compiler works. It tosses every library file it can find into the soup. So you get warnings from libraries you haven't even added to your project.

-jim lee
*/
neoPixel lites(NUM_LEDS, PIXEL_PIN);

sparkles theSparkles(&lites);

colorObj backColor;
colorObj sparkleColor[SPARKLE_COLORS];
int       maxCIndex;

void setup() {

  int i;
  
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  i=0;
  //for(int j=0;j<5;j++) sparkleColor[i++].setColor(LC_WHITE);
  
  sparkleColor[i++].setColor(LC_RED);
  sparkleColor[i++].setColor(LC_GREEN);
  sparkleColor[i++].setColor(LC_BLUE);
  sparkleColor[i++].setColor(LC_YELLOW);
  sparkleColor[i++].setColor(LC_PURPLE);
  sparkleColor[i++].setColor(LC_LAVENDER);
  sparkleColor[i++].setColor(LC_ORANGE);
  sparkleColor[i++].setColor(LC_MAGENTA);

  
  maxCIndex = i-1;
  
  backColor.setColor(&black);
  //backColor.blend(&black, DARK_PERCENT);
  
  lites.begin();
  lites.setAll(&backColor);
  lites.show();
}


void loop() {

  colorObj  aColor;

  theSparkles.erase();
  lites.setAll(&backColor);
  if (!random(0, SPARKLE_ODDS)) {
    theSparkles.addSparkle(random(0, NUM_LEDS - 1), &(sparkleColor[random(0,maxCIndex)]));
    //theSparkles.addSparkle(random(0, NUM_LEDS - 1), &red);
  }
  theSparkles.draw();
  lites.show();
}
