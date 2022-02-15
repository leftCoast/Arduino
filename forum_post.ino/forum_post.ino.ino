
#include <adafruit_1947_Obj.h>
#include <SDTools.h>
#include <stlFile.h>

char*          header[80];
char*          filename = "sillyStl";
unsigned long  numFacets = 1;
STLFacet       aFacet;
File           stlFile;

void setup(void) {
   
   Serial.begin(57600);                      // Fire up serial for debugging.
   if (!SD.begin(ADA_1947_SHIELD_SDCS)) {    // With icons, we now MUST have an SD card.
         Serial.println("NO SD CARD!");      // Tell user we have no SD card.
         Serial.flush();                     // Make sure the message gets out.
         while(true);                        // Lock the process here.
   }   
   
   for(int i=0;i<80;i++) {
      if (i<strlen(filename)) {
         header[i] = filename[i];  
      } else {
         header[i] = ' ';
      }
   }

   aFacet.normVect[0]   = 0;    // x value
   aFacet.normVect[1]   = 0;    // y value
   aFacet.normVect[2]   = -1;   // z value

   aFacet.vertex1[0]    = 10;
   aFacet.vertex1[1]    = 10;
   aFacet.vertex1[2]    = 10;

   aFacet.vertex2[0]    = 0;
   aFacet.vertex2[1]    = 0;
   aFacet.vertex2[2]    = 10;

   aFacet.vertex3[0]    = 20;
   aFacet.vertex3[1]    = 0;
   aFacet.vertex3[2]    = 10;

   aFacet.extra         = 0;
   
   stlFile = SD.open("test.stl",FILE_WRITE);
   if (stlFile) {
      stlFile.seek(0);
      stlFile.write(header,80);
      write32(numFacets,stlFile);
      stlFile.write(&aFacet,50);
      stlFile.close();
   } else {
      Serial.println("No FILE!?");
   }
   Serial.println("All done!");
}

void loop(void) { }
