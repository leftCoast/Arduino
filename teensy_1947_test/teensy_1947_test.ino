#include <SD.h>
#include <ILI9341_t3.h>
//#include <Adafruit_ILI9341.h>

#define DSP_CS     10
#define DSP_RST    -1
#define SD_CS      4

#define SPLASH_SCR_BMP        "/system/images/sunset.bmp"


ILI9341_t3        disp(DSP_CS,9,DSP_RST);
//Adafruit_ILI9341  disp(DSP_CS,9,DSP_RST);
void setup() {

  Serial.begin(9600);

    disp.begin();
    disp.fillScreen(ILI9341_GREEN);
  if (!SD.begin(SD_CS)) {                                        // With icons, we now MUST have an SD card.
    Serial.println("NO SD CARD!");                              // Send an error out the serial port.
    Serial.flush();                                             // Make sure it goes out!
   }
   Serial.print("Screen W,H : ");
   Serial.print(disp.width());
   Serial.print(", ");
   Serial.println(disp.height());
   

   //bmpObj* mBackImage = new bmpObj(0,24,240,280,SPLASH_SCR_BMP);
   //viewList.addObj(mBackImage);
    
   File imageFile;
   imageFile = SD.open(SPLASH_SCR_BMP);
   if (imageFile) {
      Serial.println("Good, opend the file.");
   } else {
      Serial.println("Oh no, failed to open the damn file.");
   }
}


// During loop..
void loop() {     
  
  //idle();         // Idlers get their time.
}
