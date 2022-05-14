#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <neoPixel.h>
#include <lilParser.h>
#include <colorObj.h>


#define NUM_COLORS  24

#define NUM_LEDS    2
#define LED_PIN     3
#define HOUR_LED    1
#define DECMIN_LED  0


enum        mainComs  { noCommand,
                        help,
                        setHour,
                        setMin,
                        setSec,
                        reset,
                        cEdit,
                        gColor,
                        sColor
                      };
lilParser   mParser;
RTC_PCF8523 rtc;
DateTime    drawtime;

colorObj    colors[NUM_COLORS];
neoPixel    theNeoPixel(NUM_LEDS,LED_PIN);


void setup() {
  
  Serial.begin(57600);  
  while (!Serial);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  
  theNeoPixel.begin();
  theNeoPixel.setPixelColor(0,&red);
  theNeoPixel.setPixelColor(1,&blue);
  theNeoPixel.show();

  mParser.addCmd(help,"?");
  mParser.addCmd(setHour,"hour");
  mParser.addCmd(setMin,"minute");
  mParser.addCmd(setSec,"sec");
  mParser.addCmd(reset,"reset");
  mParser.addCmd(cEdit,"cedit");
  mParser.addCmd(gColor,"gcolor");
  mParser.addCmd(sColor,"scolor");
}


// **********************************************************************
// Param handleing
// **********************************************************************


void readParams(void) {

  RGBpack paramBlock[24];
  
  EEPROM.get(0,paramBlock);
  for (int i=0;i<24;i++) {
    colors[i].setColor(&(paramBlock[i]));
  }
}


void saveParams(void)  {
  
  RGBpack paramBlock[24];

  for (int i=0;i<24;i++) {
    paramBlock[i] = colors[i].packColor();
  }
  EEPROM.put(0,paramBlock);
}



// **********************************************************************
// Checking time
// **********************************************************************


void checkTime(void) {
  
  DateTime  now;
  int       index;
  
  now = rtc.now();
  if((now.minute()!=drawtime.minute())||(now.hour()!=drawtime.hour())) {  // If times have changed..
    index = now.hour();
    theNeoPixel.setPixelColor(HOUR_LED,&(colors[index]));
    theNeoPixel.setPixelColor(DECMIN_LED,&black);
    drawtime = now;
  }
}


// **********************************************************************
// Command parsing stuff
// **********************************************************************


void doSetHour() {

  byte  newVal;
  
  if (mParser.numParams()) {
    mParser.getParam();
    newVal = atoi (returnStr);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),newVal,now.minute(),now.second()));
  }
}


void doSetMin() {

  byte  newVal;
  
  if (mParser.numParams()) {
    mParser.getParam();
    newVal = atoi (returnStr);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),newVal,now.second()));
  }
}


void doSetSec() {
  
  byte  newVal;
  
  if (mParser.numParams()) {
    mParser.getParam();
    newVal = atoi (returnStr);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),newVal));
  }
}


void doReset() {

  DateTime  now;

  colors[0].setColor(LC_RED);
  colors[1].setColor(LC_ORANGE);
  colors[2].setColor(LC_YELLOW);
  colors[3].setColor(LC_GREEN);
  colors[4].setColor(LC_BLUE);
  colors[5].setColor(100,100,0);
  colors[6].setColor(LC_RED);
  colors[7].setColor(LC_ORANGE);
  colors[8].setColor(LC_YELLOW);
  colors[9].setColor(LC_GREEN);
  colors[10].setColor(LC_BLUE);
  colors[11].setColor(100,100,0);   // Brown.
  for(int i=0;i<12;i++) {
    colors[12+i].setColor(&(colors[i]));
  }
  saveParams();
  //Wire.beginTransmission(0x68);   // address DS3231
  //Wire.write(0x0E);               // select register
  //Wire.write(0b00011100);         // write register bitmap, bit 7 is /EOSC
  //Wire.endTransmission();
  now = rtc.now();
}


void doGetColor(void) {

  int   hourVal;
  
  if (mParser.numParams()==1) {
    mParser.getParam();
    hourVal = atoi (returnStr);
  } else {
    hourVal = drawtime.hour();
  }
  if (hourVal>=0&&hourVal<24) {
    Serial.print("Hour ");Serial.print(hourVal);Serial.print(" RGB Value : ");
    Serial.print(colors[hourVal].getRed());
    Serial.print(" ");Serial.print(colors[hourVal].getGreen());
    Serial.print(" ");Serial.println(colors[hourVal].getBlue());
  } else {
    Serial.print("Hour is out of bounds : ");Serial.println(hourVal);
  }
}


void doSetColor(void) {

  int   hourVal;
  int   redVal;
  int   greenVal;
  int   blueVal;
  
  if (mParser.numParams()==4) {
    mParser.getParam();
    hourVal = atoi (returnStr);
    
    if (hourVal>=0 && hourVal<24) {
      mParser.getParam();
      redVal = atoi (returnStr);
      
  
      mParser.getParam();
      greenVal = atoi (returnStr);
      
  
      mParser.getParam();
      blueVal = atoi (returnStr);
      
      colors[hourVal].setColor(redVal,greenVal,blueVal);
      ourOS.saveParams();
      if (hourVal==drawtime.hour()) {
        //mBacground->setColor(&(colors[hourVal]));
        //We'll need to set the LEDs here.
      }
    } else {
      Serial.println("No, values between 0..23 please.");
    }
  }
}



void checkParse(void) {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       
      case noCommand      : break;
      case help           : 
        Serial.println("Commands:");
        Serial.println("   hour followed by a number 0..23. Sets the hour.");
        Serial.println("   minute followed by a number 0..59. Sets the minutes.");
        Serial.println("   sec followed by a number 0..59. Sets the seconds.");
        Serial.println("   cedit takes you to the color editor.");
        Serial.println("   reset will reset all colors to defaults.");
        Serial.println("   gcolor reads back the current hour's color.");
        Serial.println("   scolor followed by 4 numbers hour,R,G,B sets that hour's color.");
      break;
      case setHour  : doSetHour();    break;
      case setMin   : doSetMin();     break;
      case setSec   : doSetSec();     break;
      case cEdit    : nextPanel = colorEditApp; break;
      case reset    : doReset();      break;
      case gColor   : doGetColor();   break;
      case sColor   : doSetColor();   break;
      default       : 
        Serial.println("I really don't know what your looking for.");
        Serial.println("Try typing ? for a list of commands.");
      break;
    }
  } 
}



// **********************************************************************
// Main loop()
// **********************************************************************


void loop() {     // During loop..
   
   checkTime();
   checkParse();
}
