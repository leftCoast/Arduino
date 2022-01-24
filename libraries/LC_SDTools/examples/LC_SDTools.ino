/*
  pwd   - Print working directory.
  cd    - Change directory,
  ls    - List working directory.
  mkdr  - Make new directory.
*/

#include <SD.h>
#include <Adafruit_GFX.h>
#include <adafruit_1947_Obj.h>
#include <lilParser.h>
#include <screen.h>
#include <resizeBuff.h>
#include "src/SDTools.h"


#define SD_CS        4     // SD chip select pin number. Change to suit your setup.
#define PATH_CHARS   255   // Could be less, depending on how deep into the SD drive you look.


enum commands {   noCommand,  // ALWAYS start with noCommand. Or something simlar.
                  printWD,    // The rest is up to you. help would be a good one. Have it list
                  changeDir,  // What the other commands are, how they work and what they do.
                  listDir,
                  makeDir,
                  deleteFile
              };          // Our list of commands.

lilParser   ourParser;        // The parser object.
pathStr     wd;
char*       pathBuff[PATH_CHARS];


void setup() {
  if (!initScreen(ADAFRUIT_1947, ADA_1947_SHIELD_CS, PORTRAIT)) { // If we can't get the screen running..
    Serial.println("NO SCREEN!");                               // Send an error out the serial port.
    Serial.flush();                                             // Make sure it goes out!
    while (true);                                               // Lock processor here forever.
  }
  screen->fillScreen(&blue);                                    // Looks like we have a screen, fill it with ??.

  if (!SD.begin(SD_CS)) {             // If we can not initialze a SD drive.
    Serial.println("No SD Drive!");  // Tell the user.
    while (1);                       // Just stop here.
  }
  ourParser.addCmd(printWD, "pwd");   // Add pwd command  [ If they type "pwd" they mean printWD ]
  ourParser.addCmd(changeDir, "cd");  // Add cd command
  ourParser.addCmd(listDir, "ls");    // Add ls command
  ourParser.addCmd(makeDir, "mkdr");  // Add mkdr command
  ourParser.addCmd(makeDir, "mkdir"); // We'll take it either way.
  ourParser.addCmd(deleteFile, "rm"); // We'll take it either way.
}


// Your loop where it parses out all your typings.
void loop(void) {

  char  inChar;
  int   command;

  if (Serial.available()) {                                         // If serial has some data..
    inChar = Serial.read();                                        // Read out a charactor.
    Serial.print(inChar);                                          // If using development machine, echo the charactor.
    command = ourParser.addChar(inChar);                           // Try parsing what we have.
    switch (command) {                                             // Check the results.
      case noCommand    : break;                                  // Nothing to report, move along.
      case printWD      : Serial.println(wd.getPath());  break;   // Easy peasy! Just print wd out.
      case listDir      : listDirectory();               break;   // Print out a listing of the working directory.
      case makeDir      : makeDirectory();               break;   // See if we can create a directory in the working directory.
      case changeDir    : changeDirectory();             break;   // Try changing directorys.
      case deleteFile   : deleteItem();                  break;   // Delete a directory or file.
      default           : Serial.println("What?");       break;   // No idea. Try again?
    }
  }
}



/************************************************************************************
*************************************************************************************

                     Now the list of command handlers you call from
                     your main loop() when commands are parsed.

*************************************************************************************
*************************************************************************************/



// [ls] Lists all the files in the working direcotory.
void listDirectory(void) {

  dirList  aDirectory;
  dirItem* anItem;
  int      index;
  int      field;
  int      numChars;

  field = 15;
  aDirectory.readDir(wd.getPath());
  if (aDirectory.getCount()) {
    index = 0;
    anItem = aDirectory.findItem(index++);
    while (anItem) {
      Serial.print(anItem->getName());
      if (anItem->getIsDir()) {
        Serial.println("/");
      } else {
        numChars = strlen(anItem->getName());
        for (int i = 0; i < field - numChars; i++) {
          Serial.print(" ");
        }
        Serial.println(anItem->getSize());
      }
      anItem = aDirectory.findItem(index++);
    }
  }
  aDirectory.dumpList();
}


// [mkdir] Create a new directory in the working directory using typed in parameter.
void makeDirectory(void) {

  char*    charBuff;
  pathStr  tempPath(wd);
  char*    itemName;
  int      index;
  bool     success;
  bool     done;
  file     item;

   if (ourParser.numParams()) {                 // If they typed in something past the command.
      charBuff = ourParser.getParamBuff();      // We get the first parameter, assume its the new folder's name.
      tempPath.setPath(charBuff);               // Using the working directory copy and this string. Calculate the full path.
      pathBuff[0] = '\0';                       // Clear out the path buffer.
      index = 0;                               // Index starts at zero.
      seenFile = false;                         // Not seen a file yet.
      success = false;                          // Not a success yet.
      done = false;                             // And we are not done yet.
      do {                                      // Start looping.
            itemName = getPathItemName(index);     // Grab an item name from the list.
            if (itemName) {                        // If we got one..
                  if (seenFile) {                     // If we've already seen a file..
                     done = true;                     // This is a fail. Time to bail.
                  } else {                            // Else, We've not seen a file yet. Keep looking.
                     strcat(pathBuff, "/");           // Start with a slash
                     strcat(pathBuff, itemName);      // Add in the name.
                     if (!SD.exists(pathBuff)) {      // If this path does not exist..
                        SD.mkdir(pathBuff);           // Have a go at creating it.
                        if (!SD.exists(pathBuff)) {   // Still does not exist..
                           done = true;                  // This is a fail. Time to bail.
                        }
                     } else {                            // Else, the items exists..
                        item = SD.open(pathBuff);        // Open the item.
                        if (!item.isDirectory()) {       // If the item is NOT a directory..
                           seenFile = true;               // This is actually a file, not a directory.
                        }
                        item.close();
                     }
                  }
        } else {                               // We've run out of items.
          success = true;                     // This is the only successful end case.
          done = true;                        // And we're done looping.
        }
      }
    } while (!done);

    strcat(pathBuff, "/");                       // Add in the slash.
    strcat(pathBuff, charBuff);                  // Add in the user's parameter.
    free(charBuff);                              // Free the user's parameter.
    if (SD.mkdir(nameBuff)) {                    // If we can create the folder.
      listDirectory();                          // Lets see it.
    } else {                                     // Else, we couldn't..
      Serial.println("Can't create folder.");   // We'll send back an error.
    }
  }
}


// [cd] Change the working directory.
//          /  -> Set to root.
//          .. -> Go up one.
//     /name.. -> Full path.
//        name -> Local folder.
void changeDirectory(void) {

  char* charBuff;
  File  dir;
  int   numBytes;
  char* savedPath;

  savedPath = NULL;                                     // When using resizeBuff ALWAYS init to NULL.
  if (ourParser.numParams()) {                          // If they typed in some parameters.
    charBuff = ourParser.getParamBuff();               // We grab the first parameter.
    numBytes = wd.getPathBytes();                      // How many bytes we need here?
    if (resizeBuff(numBytes, &savedPath)) {            // If we can get RAM to store the path in..
      wd.getPath(savedPath);                          // Copy the original path.
      wd.setPath(charBuff);                           // Create the path to the new location.
      free(charBuff);                                 // Dump the parameter buffer.

      dir = SD.open(wd.getPath(), FILE_READ);         // See if the new location can be found.
      if (dir) {                                      // If we found it..
        listDirectory();                             // Lets see it.
        dir.close();                                 // And release it.
      } else {                                        // Else, we can't find it.
        Serial.println("Can't find folder.");        // We'll send back an error.
        wd.setPath(savedPath);                       // Reset the working directory.
      }
      resizeBuff(0, &savedPath);                      // Recycle the saved path.
    }
  }
}


//[rm] Delete a file or directory..
void deleteItem(void) {

  char* charBuff;

  if (ourParser.numParams()) {              // If they typed in something past the command.
    charBuff = ourParser.getParamBuff();   // We get the first parameter, assume its the new folder's name.

    SD.exists(charBuff);
  }
}
