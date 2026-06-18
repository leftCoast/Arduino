
//#include <ST7796_t3.h>  // The teensy display lib.
#include <SD.h>         // The SD lib.
#include <LC_SPI.h>     // My lib that supplies the LC_DC value.
#include <filePath.h>
#include <baseImage.h>
#include <bmpObj.h>
#include <strTools.h>
#include <SDTools.h>
#include <MSP3526_T.h>

#define SD_CS     4 //28 
#define DSP_CS    10
#define DSP_RST   26

//ST7796_t3 disp(DSP_CS,LC_DC,DSP_RST);

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
#define PATH12 "/SYSTEM/APPFILES/slider/"
#define PATH13 "/SYSTEM/APPFILES/slider/pix/"


int x;
int y;


void setup() {

  bool  haveScreen;

  Serial.begin(9600);         // Fire up serial
  delay(2000);                // Sometimes the Mac needs a smoko before starting.
  x = 1;
  y = 1;
  haveScreen = false;
  screen =  (displayObj*) new MSP3526_T(DSP_CS,DSP_RST);
  if (screen) {
    if (screen->begin()) {
      screen->setRotation(PORTRAIT);
      haveScreen = true;
    }
  }
  if (!haveScreen) {
    Serial.println("SScreen failed!");
    while(1)delay(10);
  }
  if (!SD.begin(SD_CS)) {
    Serial.println("SD.begin() failed!");
    while(1)delay(10);
  }
  ourEventMgr.begin(); 
}


void checkBMPFile(filePath* aPath) {

  int         w;
  int         h;
  char*       pathStr;
  File        tempFile;

  if (aPath) {                                                                  // Path?
    pathStr = NULL;                                                             // Init string.
    if (heapStr(&pathStr,aPath->getPathName())) {                               // If we can copy the name of the file or folder.
      if (pathStr[0]!=0&&pathStr[0]!='.' ) {                                    // If we got chars and not starting with a '.'.
        if (heapStr(&pathStr,aPath->getPath())) {                               // If we can now grab the actual path string.
          if (extensionMatch(".bmp",pathStr)) {                                 // If we have a .bmp file..                                 
            bmpObj aBmpObj = bmpObj(1,1,w,h,pathStr);                           // Create the bmpObj.
            if (aBmpObj.checkDoc(tempFile)) {                                 // See if we can read the file. Is it ok?
            w = aBmpObj.getWidth();                                         // Grab width.
            h = aBmpObj.getHeight();
              if (anImage.checkDoc(tempFile)) {                                 // See if we can read the file. Is it ok?
                w = anImage.getWidth();                                         // Grab width.
                h = anImage.getHeight();                                        // Height.
                bmpObj aBmpObj = bmpObj(1,1,w,h,pathStr);                       // Create the bmpObj.
                Serial.println("***** Looks like we opend a .BMP file. *****"); // Look at us!
                Serial.print  ("***** WIDTH  : ");Serial.println(w);            // See? Did we get values?
                Serial.print  ("***** HEIGHT : ");Serial.println(h);            //
                x++;
                y++;
                if (x>125) x = 1;
                if (y>240) y = 1;
                aBmpObj.setLocation(x,y);
                aBmpObj.drawSelf();
                delay(100);
              }                                                                 //
              tempFile.close();                                                 // Recycle the file.
            }                                                                   //
          }
        }                                                                     //
      }                                                                         //
      freeStr(&pathStr);                                                        //
    }                                                                           //
  }
}


/*
void checkBMPFile(filePath* aPath) {

  int         w;
  int         h;
  char*       pathStr;
  File        tempFile;

  if (aPath) {                                                                  // Path?
    pathStr = NULL;                                                             // Init string.
    if (heapStr(&pathStr,aPath->getPathName())) {                               // Try for a path string.
      if (pathStr[0]!=0&&pathStr[0]!='.' ) {                                    // Got chars and not starting with a '.'?
        if (heapStr(&pathStr,aPath->getPath())) {                               // Ok, grab the actual path string.
          if (extensionMatch(".bmp",pathStr)) {                                 // If we have a .bmp file..                                 
            bmpImage anImage = bmpImage(pathStr);                               // Read the image..
            tempFile = SD.open(pathStr,FILE_READ);                              // Try to open the file.
            if (tempFile) {                                                     // If we got the file open..
              if (anImage.checkDoc(tempFile)) {                                 // See if we can read the file. Is it ok?
                w = anImage.getWidth();                                         // Grab width.
                h = anImage.getHeight();                                        // Height.
                bmpObj aBmpObj = bmpObj(1,1,w,h,pathStr);                       // Create the bmpObj.
                Serial.println("***** Looks like we opend a .BMP file. *****"); // Look at us!
                Serial.print  ("***** WIDTH  : ");Serial.println(w);            // See? Did we get values?
                Serial.print  ("***** HEIGHT : ");Serial.println(h);            //
                x++;
                y++;
                if (x>125) x = 1;
                if (y>240) y = 1;
                aBmpObj.setLocation(x,y);
                aBmpObj.drawSelf();
                delay(100);
              }                                                                 //
              tempFile.close();                                                 // Recycle the file.
            }                                                                   //
          }
        }                                                                     //
      }                                                                         //
      freeStr(&pathStr);                                                        //
    }                                                                           //
  }
}
*/

void testFile(char* inPath) {

  filePath    aPath;
  int         w;
  int         h;

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
      case fileType    : 
        Serial.println("***** File *****");
        checkBMPFile(&aPath);
      break;
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
      if (theList->getType()==fileType) {
        thePath->pushChildItemByName(theList->getName());
        testFile(thePath->getPath());
        thePath->popItem();
      }
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


  idle();
  testFile(PATH1);
  idle();
  testFile(PATH2);
  idle();
  testFile(PATH3);
  idle();
  testFile(PATH4);
  idle();
  testFile(PATH5);
  idle();
  testFile(PATH6);
  idle();
  testFile(PATH7);
  idle();
  testFile(PATH8);
  idle();
  testFile(PATH9);
  idle();
  testFile(PATH10);
  idle();
  testFile(PATH11);
  idle();
  testFile(PATH12);
  idle();
  testFile(PATH13);
  idle();
  //tone(23, 500,35);
  delay(100);
}
