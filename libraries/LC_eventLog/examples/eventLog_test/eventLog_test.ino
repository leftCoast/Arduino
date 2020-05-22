
#include "SD.h"

// **********************************************************************
// These are just for my hardware. You will need to replace them to match
// your hardware setup.

#include "adafruit_1947_Obj.h"
#include "screen.h"
// **********************************************************************


// **********************************************************************
// For the eventLog to compile, you will also need the LC_baseTools
// library from the Left coast repository. Go find it.

#include "eventLog.h"
// **********************************************************************


#define SD_CS     4           // My chip select choice was 4, that's prett typical, but yours may be different.
#define MAX_BYTES 200         // A good size for a text buffer.

char  dataBuff[MAX_BYTES+1];  // Text  uffer to store the stuff Mrs user will type in.
int   i = 0;                  // Pointer for keeping track of inputted text.

eventLog ourEventLog;         // The eventlog object. Remeber this was for testing eventLog?

void setup() {
   
   Serial.begin(9600);

   // **********************************************************************
   // If there is any hardware sharing the SPI bus, it needs to be initiated.
   // In my case there is an Adafruit display that the SD card reader is
   // attached to. Your's will probably be different. So this should be
   // replaced with however your hardware is initilized.
   if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
    Serial.println("NO SCREEN!");
    Serial.flush();
    while(true);
   }
   screen->fillScreen(&black);
   // **********************************************************************

   
   if (!SD.begin(SD_CS)) {
      Serial.println("NO SD CARD!");
      while(1);
   }
   
   if (!ourEventLog.begin("/logTest.txt")) {
      Serial.println("Event log error.");
      while(1); 
   }
   ourEventLog.setLogging(true);
   
   Serial.println("Type in a string to log it.");
   Serial.println("Type ? for list of commands.");
}


// Before doing the logging thing, lets check to see if its actually a command
// first. IF so, handle the command. If not? We can let them log it.
bool checkCommand(void) {

   File  logFile;
   int   numBytes;
   char  aChar;
   
   if (!strcmp(dataBuff,"?")) {
      Serial.println("Well, first is \"?\" to print this.");
      Serial.println("Type \"show\" to print the datafile to the monitor.");
      Serial.println("Type \"stop\" to shut down logging.");
      Serial.println("Type \"start\" to shut down logging.");
      Serial.println("Type \"clear\" to clear out the logfile.");
      return true;
   } else if (!strcmp(dataBuff,"show")) {
      Serial.println("Reading log file.");
      logFile = SD.open(ourEventLog.getPath(),FILE_READ);
      if (logFile) {
         Serial.println();
         numBytes = logFile.size();
         for (int i=0;i<numBytes;i++) {
            Serial.print((char)logFile.read());
         }
          Serial.println();
      }
      Serial.println("Complete.");
      return true;
   } else if (!strcmp(dataBuff,"stop")) {
      ourEventLog.setLogging(false);
      Serial.println("logging has been turned off.");
      return true;
   } else if (!strcmp(dataBuff,"start")) {
      ourEventLog.setLogging(true);
      Serial.println("logging has been resumed.");
      return true;
   } else if (!strcmp(dataBuff,"clear")) {
      ourEventLog.deleteFile();
      Serial.println("Logfile has been deleted.");
      return true;
   }
   return false;
}


// loop through reading user text 'till an end of string is seen. Then
// add the resulting string to the log.
void loop() {

  char   inChar;

  idle();                                                // Always call idler() first.
  if (Serial.available()) {                              // If there is text to be read..   
      inChar = Serial.read();                            // We grab a character.     
      if (inChar=='\n') {                                // If its a newline? We have a complete string..
         dataBuff[i] = '\0';                             // Stamp in the ending char of our c string.
         if (!checkCommand()) {                          // If we can't pase a comamnd out of it..
            if (ourEventLog.addEvent(dataBuff)) {        // Send the string off to be logged.
               Serial.print(dataBuff);                   // Show us what we logged.
               Serial.println(", added to event log.");  // And that we logged it.
            } else {                                     // Else..
               Serial.println("No data added.");         // Show that we couldn't log it for some reason.
            }
         }
         i = 0;                                          // In any case this string is done, setup for the next one.
      } else if (i<MAX_BYTES) {                          // Else (Not end of string) and if we have room in the buffer..
         dataBuff[i] = inChar;                           // Pop the character into the buffer.
         i++;                                            // Bump up our buffer pointer.
      }
   }
}
