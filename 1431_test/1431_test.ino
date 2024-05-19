
#include <mask.h>
#include <baseGraphics.h>
#include <drawObj.h>
#include <region.h>
#include <displayObj.h>
#include <colorRect.h>
#include <bmpObj.h>
#include <iconButton.h>
#include <slider.h>
#include <flasher.h>
#include <lineObj.h>
#include <bmpLabel.h>
#include <label.h>
#include <fontLabel.h>
#include <liveText.h>
#include <textView.h>


#include <adafruit_1431.h>

#define OLED_CS       10
#define OLED_RST      6

label*  aLabel;



void setup() {

  Serial.begin(9600);
  screen = (displayObj*) new adafruit1431(OLED_CS,LC_DC,OLED_RST);
  if (screen) {
    screen->begin();
    screen->setRotation(INV_PORTRAIT);
  } else {
    Serial.println("NO SCREEN!");                               // Send an error out the serial port.
    Serial.flush();                                             // Make sure it goes out!
    while(true);                                                // Lock processor here forever.
  }
  //if (!SD.begin(SD_CS)) {                                        // With icons, we now MUST have an SD card.
    Serial.println("NO SD CARD!");                              // Send an error out the serial port.
    Serial.flush();                                             // Make sure it goes out!
  //}
   Serial.print("Screen W,H : ");
   Serial.print(screen->width());
   Serial.print(", ");
   Serial.println(screen->height());
   screen->fillScreen(&black);

   aLabel = new label("hey there..");
   aLabel->setColors(&white);
   aLabel->setLocation(20,20);
   viewList.addObj(aLabel);
   
}


// During loop..
void loop() {     
  
  idle();         // Idlers get their time.
}
