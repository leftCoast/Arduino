//#include <adafruit_2050.h>
//#include <displayObj.h>
//#include <idlers.h>

#define SD_CS        4      // wht
#define SD_Detect    5     // grn
#define LC_DC        9     // Data/command   - YELLOW WIRE 30AWG
#define SCREEN_RST   14    // red
#define SCREEN_LED   15    // blu
#define BEEP_PIN     23    // none yet
#define SCREEN_CS    25    // blk

void setup() {
   Serial.begin(9600);
   Serial1.begin(9600);
   ///screen = (displayObj*)new adafruit_2050(SCREEN_CS,LC_DC,SCREEN_RST);
   //screen->begin();
   //screen->fillScreen(&green);

}


void resetBuff(void) {
   
    buffIndex = 0;
    buff[buffIndex] = '/0';
}


void   checkSum(void) {

   int   i;
   byte  chkSumVal;
   char  sumStr[4];
   byte  sumStrVal;
   
   if (strchr(buff,'*')) {
      i=0;
      chkSumVal = 0;
      while(buff[i]!='*') {
         chkSumVal ^= buff[i];             //xor the incoming char
         i++;
      }
      i++;  
      sumStr[0] = buff[i++];
      sumStr[1] = buff[i];
      sumStr[2] = '\0';
      sumStrVal = (byte)strtol(sumStr, NULL, 16);
      if (chkSumVal!=sumStrVal) {
         Serial.println("***** Checksum failed! *****");
         Serial.print("The message [");
         Serial.print(buff);
         Serial.println("]");
         Serial.print("Msg length : ");
         Serial.println(strlen(buff));
         Serial.println(sumStrVal);
         Serial.println(chkSumVal,HEX);
      } else {
         Serial.print("We see ");
         Serial.print(sumStrVal,HEX);
         Serial.print(" ");
         Serial.print(chkSumVal,HEX);
         Serial.print(" ");
         Serial.println("It's ok");
      }
   }  else {
      Serial.println("***** Missing Checksum! *****");
   }  
}

char  buff[200];
int   buffIndex;
bool  inSynk = false;

void loop() {

   char  aChar;

   if (Serial1.available()) {
      Serial.print((char)Serial1.read());
   }
   /*
   if (Serial1.available()) {
      Serial.print(Serial1.available());
      Serial.print(" ");
      aChar = Serial1.read();
      if (aChar=='$') {
         Serial.print("Synk!");
         Serial.print(" ");
         resetBuff();
      } else if (aChar=='\n') {
         Serial.println(buff);
         checkSum();
         resetBuff();
      } else {
         buff[buffIndex++] = aChar;
         buff[buffIndex] = '/0';
      }
   }
   */
}
