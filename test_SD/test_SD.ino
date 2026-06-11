
#include <ST7796_t3.h>  // The teensy display lib.
#include <SD.h>         // The SD lib.
#include <LC_SPI.h>     // My lib that supplies the LC_DC value.
#include <filePath.h>

#define SD_CS     4 //28 
#define DSP_CS    10
#define DSP_RST   26

ST7796_t3 disp(DSP_CS,LC_DC,DSP_RST);

#define PATH1 "/"
#define PATH2 "/SYSTEM/ICONSxxxxxxxx"            // Bad path
#define PATH3 "/SYSTEM/icons/STANDARD"
#define PATH4 "/SYSTEM/ICONS/STANDARD/CHUMBY"   // Missing path
#define PATH5 "/SYSTEM/imAGEs"
#define PATH6 "/SYSTEM/IMAGES/LAKE.BMP"
#define PATH7 "/SYSTEM/APPFILES"
#define PATH8 "/SYSTEM/APPFILES/BREAKOUT"
#define PATH9 "/system/APPFILES/BREAKOUT/BREAKOUT.BMP"
#define PATH10 "/SYSTEM/APPFILES/SETT"
#define PATH11 "/SYSTEM/APPFILES/SETT/SETT.BMP"


void setup() {

  Serial.begin(9600);         // Fire up serial
  delay(2000);                // Sometimes the Mac needs a smoko before starting.
  disp.init(320, 480);				// Do the init() like the example.
  disp.invertDisplay(true);   // This LCD requires colors to be inverted.
  if (!SD.begin(SD_CS)) {
    Serial.println("SD.begin() failed!");
  }
}


void testFile(char* inPath) {

  filePath  aPath;
  //char      aChar;
  
  Serial.print("Path\t");
  Serial.print(inPath);
  Serial.print("\t");

  if (aPath.setPath(inPath)) {
    switch (aPath.getPathType()) {
      case noType :
        Serial.println(" ***** Not found!! *****"); 
        //while(!Serial.available()) delay(10);
        //while(Serial.available()) aChar = Serial.read();
      break;
      case rootType    : 
        Serial.println(" ***** Root *****");
        showlist(&aPath);
        break;
      case folderType  : 
        Serial.println(" ***** Directory *****");
        showlist(&aPath);
        break;
      case fileType    : Serial.println("***** File *****"); break;
      default          : Serial.println("***** Error lands here *****");
    }
  } else {
    Serial.print("[");
    Serial.print(aPath.getPath());
    Serial.print("]");
    Serial.println("\t***** PATH WAS INVALID *****");
  }
}


void showlist(filePath* thePath) {


  pathItem*  theList;

  if (thePath->getPathType()==rootType || thePath->getPathType()==folderType) {
    thePath->refreshChildList();
    theList = thePath->getChildList();
    while (theList) {
      Serial.print("\t");
      Serial.println(theList->getName());
      theList = (pathItem*)theList->dllNext;
    }
  }
}



  /*
  aFile = SD.open(inPath,FILE_READ);
  if (aFile) {
   if (aFile.isDirectory()) Serial.println(" ***** Directory *****"); else Serial.println("***** File *****");
   aFile.close();
  } else {
    Serial.println(" ***** Failed! *****");
    while(!Serial.available()) delay(10);
    while(Serial.available()) aChar = Serial.read();
  }freeNeed : 1
  */



void loop() {

  testFile(PATH1);
  testFile(PATH2);
  testFile(PATH3);
  testFile(PATH4);
  testFile(PATH5);
  testFile(PATH6);
  testFile(PATH7);
  testFile(PATH8);
  testFile(PATH9);
  testFile(PATH10);
  testFile(PATH11);
  //tone(23, 500,35);
  delay(100);
}
