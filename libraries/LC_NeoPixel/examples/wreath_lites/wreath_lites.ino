#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>


//
// Decoration I setup for "Franklin" My 1941 Dodge Carryall WC53.
// See : https://youtu.be/JWgQ5aM2B5M
// Or for the blog : https://www.eskimo.com/~jimlee/Home/Carryall_Blog/Carryall_Blog.html
//
//

#define NUM_LEDS          150
#define PIXEL_PIN         2
 
#define PATTERN_LEN       50

neoPixel lites(NUM_LEDS, PIXEL_PIN);



void setup() {

  lites.begin();
  lites.setAll(&black);
  lites.show();
  
  //testSetup();
  normalSetup();
}

void normalSetup(void) {

  colorObj  aColor;
  
  for(int i=0;i<PATTERN_LEN;i++) {
    if(i<8) aColor.setColor(LC_WHITE);
    else if (i==8 && i<12) aColor.setColor(LC_RED);
    else if (i==12 && i<20) aColor.setColor(LC_WHITE);
    else if (i==20 && i<24) aColor.setColor(LC_GREEN);
    else if (i==24 && i<33) aColor.setColor(LC_WHITE);
    else if (i==33 && i<37) aColor.setColor(LC_BLUE);
    else if (i==37 && i<46) aColor.setColor(LC_WHITE);
    else if (i==46 && i<50) aColor.setColor(LC_MAGENTA);
    //aColor.blend(&black,75);
    lites.setPixelColor(i,&aColor);
    lites.setPixelColor(i+50,&aColor);
    lites.setPixelColor(i+100,&aColor);
  }
  lites.show();
}

void testSetup(void) {

  colorObj  aColor;
  
  aColor.setColor(LC_RED);
  for(int i=0;i<37;i++) {
    lites.setPixelColor(i,&aColor);
  }
  aColor.setColor(LC_WHITE);
  for(int i=37;i<75;i++) {
    lites.setPixelColor(i,&aColor);
  }
  aColor.setColor(LC_RED);
  for(int i=75;i<112;i++) {
    lites.setPixelColor(i,&aColor);
  }
  aColor.setColor(LC_WHITE);
  for(int i=112;i<150;i++) {
    lites.setPixelColor(i,&aColor);
  }
  lites.show();
}


void loop() {
  
  lites.roll();
  lites.show();
  delay(50);
}
