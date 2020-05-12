

/************************************************************************************
*************************************************************************************
                                    5/11/2020 middle of pandemic.
                                    
This is a simple test/demo program for the Left Coast lilParser. Typically used as a
command line parser. This emulates a couple old UNIX file commands :

pwd   - Print working directory. 
cd    - Change directory, 
ls    - List working directory.
mkdr  - Make new directory.

In order to comile this you will need, fro the Left Coast library two folders :

LC_baseTools
LC_Parser

And hardware with an SD drive. You will need the pin number of your SD's chip select.
Other than that? 

Should work. I hope.

*************************************************************************************
*************************************************************************************/


#include <SD.h>
#include "lilParser.h"

#define SD_CS        4
#define PATH_CHARS   255

 
enum commands {   noCommand,
                  printWD,
                  changeDir,
                  listDir,
                  makeDir
                  };          // Our list of commands.

lilParser   ourParser;        // The parser object.
char        wd[PATH_CHARS];   // Allocate a space for the working directory.


void setup() {

   strcpy(wd,"/");                     // Initialize the directory at root.
   
   if (!SD.begin(SD_CS)) {             // If we can not initialze a SD drive.
      Serial.println("No SD Drive!");  // Tell the user.
      while(1);                        // Just stop here.
   }
   ourParser.addCmd(printWD,"pwd");    // Add pwd command  [ If they type "pwd" they mean printWD ]
   ourParser.addCmd(changeDir,"cd");   // Add cd command
   ourParser.addCmd(listDir,"ls");     // Add ls command
   ourParser.addCmd(makeDir,"mkdr");   // Add mkdr command
   ourParser.addCmd(makeDir,"mkdir");  // We'll take it either way.
}


// [ls] Lists all the files in the working direcotory. 
void listDirectory(void) {

  File  dir;                                                      // File handle used for the current directory.
  File  entry;                                                    // File handle used for the different entries of the current directory.
  bool  done;                                                     // A boolean used to track wehn to stop all this nonsense.

  dir = SD.open(wd);                                              // Try opening the working directory.
  if (dir) {                                                      // If we were able to open the working directory..
    dir.rewindDirectory();                                        // Rewind it to the first entry.
    done = false;                                                 // We ain't done yet.
    do {                                                          // Start looping through the entries.
      entry = dir.openNextFile();                                 // Grab an entry.
      if (entry) {                                                // If we got an entry..
        Serial.print(entry.name());                               // We print out its name.
        if (entry.isDirectory()) {                                // If the entry is a directory..
          Serial.println("/");                                    // Print out the slash afterwards meaning directory, not file.
        } else {                                                  // Else it's not a directory, IE. a file.
          Serial.print("\t"); Serial.println(entry.size(), DEC);  // Lets show them how big the file is.
        }
        entry.close();                                            // And we close the entry.
      } else {                                                    // Else, we didn't get an entry from above.
        done = true;                                              // No entry means, we are done here.
      }
    } while (!done);                                              // And we do this loop, over and over, while we are not done.
    dir.close();                                                  // Looping through entries is done, close up the original file.
  } else {                                                        // If this worked correctly, we'd know there was an error at this point.                                        
    Serial.println("Fail to open file.");                         // Sadly, instead of returning a NULL, it just crashes.
  }
}


// [mkdir] Crate a new directory in the working directory using typed in parameter.
void makeDirectory(void) {

   char* charBuff;                                 // A pointer for the folder name string.
   char  pathBuff[PATH_CHARS];                     // A buffer to hold the full path to the new folder.
   
   if (ourParser.numParams()) {                    // If they typed in somethng past the command.
      charBuff = ourParser.getParamBuff();         // We get the first parameter, assume its the new folder's name.
      strcpy(pathBuff,wd);                         // Start building up the full path. Starting with working directory.
      strcat(pathBuff,charBuff);                   // Add in the user's parameter.
      free(charBuff);                              // Dump the parameter buffer ASAP.
      if (!SD.mkdir(pathBuff)) {                   // If we can not create the folder.
         Serial.println("Can't create folder.");   // We'll send back an error.
      }
   }
}


// [cd] This one's a bit of a pain! Too many special cases. And bug patches! Change the working directory.
void changeDirectory(void) {

   char* charBuff;                                    // A pointer for the folder name string.
   char  pathBuf[PATH_CHARS];                         // Some RAM to play with.
   File  theDir;                                      // We may need to check a directory.
   int   numChars;                                    // A place to save the lengh of a string.
   int   index;                                       // A handy index to use, messing with strings.
   
   if (ourParser.numParams()) {                       // If they typed in some parameters.
      charBuff = ourParser.getParamBuff();            // We grab the first parameter.
      if (!strcmp(charBuff,"/")) {                    // If its just '/' IE, root..
         strcpy(wd,"/");                              // Give it to them and be done with it.
      } else if (!strcmp(charBuff,"..")) {            // If we need to go up a directory from wd..
         numChars = strlen(wd);                       // Lets see how long the working directory path is.
         if (numChars>2) {                            // If more than 2 chars, Shortest possible path is 3 chars..
            index = numChars-2;                       // Back up past the '\0' and the trailing slash.
            while(wd[index]!='/') index--;            // Run backwards 'till we see the next slash.
            wd[index+1] = '\0';                       // Truncate the string at this point. Leaving the slash.
         }
      } else {                                        // Else, they typed in some sort of path..
         if (charBuff[0]!='/') {                      // If the parameter does NOT start with '/'. They're going relative!
            strcpy(pathBuf,wd);                       // Start with the working directory.
            strcat(pathBuf,charBuff);                 // add the relative path.
         } else {                                     // Else, it DID start with '/'. Absolute path!
            strcpy(pathBuf,charBuff);                 // Just use what they typed in.
         }
         
         // WAIT!! Patching bug here. If you pass name/ and it fails, it crashes! No '/' no crash.
         numChars = strlen(pathBuf);                  // Lets see how long the path is now.
         if (pathBuf[numChars-1]=='/') {              // If we have a traileing '/'..            
            pathBuf[numChars-1]='\0';                 // Clip off the trailing, and yes correct, '/'.
         }
         //
         
         if (theDir=SD.open(pathBuf,FILE_READ)) {     // If we can open the path they gave us..
            if (theDir.isDirectory()) {               // If its a directory..
               numChars = strlen(pathBuf);            // Lets see how long the path is now.
               if (pathBuf[numChars-1]!='/') {        // If the last char is NOT a '/'..
                  strcat(pathBuf,"/");                // Add one..
               }
               strcpy(wd,pathBuf);                    // Success! Change the working directory to pathBuf.
            } else {
               Serial.println("No, thats a file");    // They tried to pull a fast one on us.
            }
            theDir.close();                           // Done with the directory. Close it.
         } else {
            Serial.println("Can't find it.");         // Something went wrong.
         }
      }
      free(charBuff);                                 // Dump the parameter buffer.
   }   
}


// Your loop where it parses out all your typings.
void loop(void) {

   char  inChar;
   int   command;
   
   if (Serial.available()) {                                // If serial has some data..
      inChar = Serial.read();                               // Read out a charactor.
      Serial.print(inChar);                                 // If using development machine, echo the charactor.
      command = ourParser.addChar(inChar);                  // Try parsing what we have.
      switch (command) {                                    // Check the results.
         case noCommand : break;                            // Nothing to report, move along.
         case printWD   : Serial.println(wd);      break;   // Easy peasy! Just print wd out.
         case listDir   : listDirectory();         break;   // Print out a listing of the working directory.
         case makeDir   : makeDirectory();         break;   // See if we can create a dirrectory in the working directory.
         case changeDir : changeDirectory();       break;   // Try changing directorys
         default        : Serial.println("What?"); break;   // No idea. Try again?
      }
   }
}
