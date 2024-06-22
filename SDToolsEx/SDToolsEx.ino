/*
  pwd   - Print working directory.
  cd    - Change directory,
  ls    - List working directory.
  mkdr  - Make new directory.
*/

#include <SD.h>
#include <adafruit_1947.h>
#include <strTools.h>
#include <lilParser.h>
#include <SDTools.h>
#include <filePath.h>


#define DSP_CS      10		// We need to fire up the display because it's on the SPI bus.
#define SD_CS        4     // SD chip select pin number. Change to suit your setup.
#define PATH_CHARS   255   // Could be less, depending on how deep into the SD drive you look.


enum pathPrefix { setRoot, upOne, upNPath, fullPath, relPath };  // Prefixes people typically use for entering path paramiters.

enum commands {   noCommand,  // ALWAYS start with noCommand. Or something simlar.
                  printWD,    // The rest is up to you. help would be a good one. Have it list
                  changeDir,  // What the other commands are, how they work and what they do.
                  listDir,
                  makeDir,
                  deleteFile
              };          // Our list of commands.

lilParser   ourParser;        // The parser object.
filePath    wd;
char       pathBuff[PATH_CHARS];


void setup() {

	Serial.begin(9600);
	screen = (displayObj*)new adafruit_1947(DSP_CS,-1);
	if (!screen) { 															// If we can't get the screen running..
		Serial.println("NO SCREEN!");                         // Send an error out the serial port.
		Serial.flush();                                       // Make sure it goes out!
		while (true);                                         // Lock processor here forever.
	}
  screen->begin();
	screen->fillScreen(&green);                                    // Looks like we have a screen, fill it with ??.

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

  wd.setPath("/");
  showCurs();
}

void showCurs(void) {

  Serial.print(wd.getPath());
  Serial.print(" > ");
}

// Your loop where it parses out all your typings.
void loop(void) {
	
	char  inChar;
	int   command;
  bool  curs;

	if (Serial.available()) {                                       // If serial has some data..
    curs = true;                                                  // Show the cursor after..
		inChar = Serial.read();                                       // Read out a charactor.
		Serial.print(inChar);                                         // If using development machine, echo the charactor.
		command = ourParser.addChar(inChar);                          // Try parsing what we have.
		switch (command) {                                            // Check the results.
			case noCommand    : curs = false;                  break;   // Nothing to report, move along.
			case printWD      : Serial.println(wd.getPath());  break;   // Easy peasy! Just print wd out.
			case listDir      : listDirectory();               break;   // Print out a listing of the working directory.
			case makeDir      : makeDirectory();               break;   // See if we can create a directory in the working directory.
			case changeDir    : changeDirectory();             break;   // Try changing directorys.
			case deleteFile   : deleteItem();                  break;   // Delete a directory or file.
			default           : Serial.println("What?");       break;   // No idea. Try again?
		}
    if (curs) showCurs();
	}
}



/************************************************************************************
*************************************************************************************

                     Now the list of command handlers you call from
                     your main loop() when commands are parsed.

*************************************************************************************
*************************************************************************************/

bool checkFile(pathItem*	item) {

  if (item->name[0]=='.') return false;
  return true;
}


// [ls] Lists all the files in the working direcotory.
void listDirectory(void) {

  pathItem*		trace;

  Serial.println("list dir");
  if (wd.getPathType()==fileType) {
     Serial.println("Sorry, this is a file, not a directory.");
     return;
  } else {
    wd.refreshChildList();
    if (wd.numChildItems()==0) {
      Serial.println("This directory is empty.");
      return;
    } else {
      trace = wd.childList;																	// Grab a pointer to the first child.
      while(trace) {																							// While we have a non-NULL pointer..
        if (checkFile(trace)) {																			// Pass this child through the crucible of the user's filter function.
          Serial.print(trace->getName());
          if (trace->getType()!=fileType) {
            Serial.print("/");
          }
          Serial.println();
        }
        trace = (pathItem*)trace->dllNext;											// Jump to the next item on the list.
      }
    }
  }
}


// [mkdir] Create a new directory in the working directory using typed in parameter.
void makeDirectory(void) {

  tempStr       param;
  char*         pathBuff;
  int           numBytes;

  pathBuff = NULL;                                                              // Start all these at NULL.
  if (ourParser.numParams()==1) {                                               // If they typed in something past the command.
    param.setStr(ourParser.getParamBuff());                                     // We get the first parameter, assume its the new folder's name.
    switch (wd.getPathType()) {                                                 // IF we did this, what would happen?
      case rootType     : Serial.println("Root? Not possible.");      break;    // Not possible, insaine.
      case folderType   : Serial.println("Foler already exists.");    break;    // Can't alreadey there.
      case fileType     : Serial.println("That's a file already.");   break;    // There's a file there by that name.
      case  noType      :                                                       // Looks like we have a chance..
        numBytes = wd.numPathBytes();                                           // We need the path..
        numBytes = numBytes + param.numChars() + 1;                             // And the param + 1 bytes.
        if (resizeBuff(numBytes,&pathBuff)) {                                   // If we can get the RAM..
          strcpy(pathBuff,wd.getPath());                                        // Stuff in our path.
          strcat(pathBuff,param.getStr());                                      // Add the param text.
          SD.mkdir(pathBuff);                                                   // Have a go at creating it.
          if (!SD.exists(pathBuff)) {                                           // Still does not exist..
						Serial.println("Could not create directory.");                      // This is a fail. Time to bail.
					}                                                                     // This point is actually a success!
           resizeBuff(0,&pathBuff);                                             // Recycle the RAM.                                           
        } else {                                                                // Else, could not get the RAM..
          Serial.println("Memory error.");                                      // Not enough RAM. WOW!
        }
      break;
    }
  } else {
    Serial.print("We were looking for one param. We saw ");
    Serial.println(ourParser.numParams());
  }
}

//enum pathPrefix { setRoot, upOne, upNPath, fullPath, relPath };


//      /  -> Set to root.
//      .. -> Go up one.
//      /name.. -> Full path.
//      name -> Local folder.
pathPrefix decodePrefix(const char* param) {

  if (!strcmp(param,"/")) return setRoot;                   // Just "/" alone means root. setRoot.
  else if (param[0]=='/') return fullPath;                  // Starts with '/' + more..   fillPath.
  else if (!strcmp(param,"..")) return upOne;               // Just ".." means go up one. upOne.
  else if (param[0]=='.' && param[1]=='.') return upNPath;  // Starts with ".." + more..  upNPath.
  else return relPath;                                      // Default, relitive path.    relPath.
}


// [cd] Change the working directory. (See prefix commands above.)
void changeDirectory(void) {

  tempStr       param;                                      // TempStr, auto allocates and holds strings during function.
  tempStr       savedPath;
 
  savedPath.setStr(wd.getPath());                           // Save the path we have, just in case.
  if (ourParser.numParams()==1) {                           // If they typed in a parameter (name/path).
    param.setStr(ourParser.getParamBuff());                 // We grab the name.
    switch(decodePrefix(param.getStr())) {                  // Decide on how the param is prefixed..
      case setRoot  :                                       // Just set to root..
        Serial.println("setRoot");
        wd.setPath("/");                                    // Easy peasy!
      break;                                                //
      case upOne    :                                       // Want to go up one deirectory..
        Serial.println("upOne");
        if (wd.getPathType()==rootType) {                   // If we're at the root already..
          Serial.println("Sorry, already at root.");        // Send error. Can't one up more root.
        } else {                                            // Else, not root?
          Serial.println("Popping");
          wd.popItem();                                     // Go ahead and pop off the item.
        }                                                   //
      break;                                                //
      case upNPath  :                                       // Up then, relitive path..
        Serial.println("upNPath");
        if (wd.getPathType()==rootType) {                   // If we're at the root already..
          Serial.println("Sorry, already at root.");        // Send error. Can't one up more root.
        } else {                                            // Else, not root?
          wd.popItem();                                     // Go ahead and pop off the item.
          if (!wd.addPath(&(param.getStr()[2]))) {          // If we can't set this concatinated path..
            wd.setPath(savedPath.getStr());                 // Put it all back..
            Serial.println("Path not found.");              // Error.
          } else {                                          // Else, we got a path. Check it out.
            if (wd.getPathType()==fileType) {               // If this is pointing to a file..
              wd.setPath(savedPath.getStr());               // Put it all back..
              Serial.println("Path is not a directory.");   // Working DIRECTORY can't be a file.
            }                                               //
          }                                                 //
        }                                                   //
      break;                                                // Done!
      case fullPath :                                       // Full path..
         Serial.println("fullPath");
        if (!wd.setPath(param.getStr())) {                  // If we can't set this path..
          wd.setPath(savedPath.getStr());                   // Put it all back..
          Serial.println("Path not found.");                // Error.
        } else {                                            // Else, we got a path, check it out.
          if (wd.getPathType()==fileType) {                 // If this is pointing to a file..
            wd.setPath(savedPath.getStr());                 // Put it all back..
            Serial.println("Path is not a directory.");     // Working DIRECTORY can't be a file.
          }                                                 //
        }                                                   //
      break;                                                // Skipping off..
      case relPath  :                                       // Relitive path..
        Serial.println("relPath");
        if (wd.addPath(param.getStr())) {                   // If we can add this relitive path bit..
          if (wd.getCurrItem()->getType()==fileType) {      // If this is pointing to a file..
            wd.setPath(savedPath.getStr());                 // Put it all back..
            Serial.println("Path is not a directory.");     // Let 'em know.
          }                                                 //
          Serial.println("success!");
        } else {                                            // Else, couldin't find the file.
          wd.setPath(savedPath.getStr());                   // Put it all back..
          Serial.println("Path not found.");                // Let 'em know.
        }                                                   //
      break;                                                // We're off..
    }
  }
}


//[rm] Delete a file or directory..
void deleteItem(void) {

  tempStr     param;
  filePath    condemmed;

  if (ourParser.numParams()==1) {                 // If they typed in a parameter (name).
    param.setStr(ourParser.getParamBuff());       // We grab the name.
    
    if (condemmed.setPath(wd.getPath())) {
      if (!condemmed.deleteCurrentItem()) {
        Serial.println("File error deleting item.");
      }
    } else {
      Serial.println("File/Folder not found.");
    }
  } else {
    Serial.print("We were looking for one param. We saw ");
    Serial.println(ourParser.numParams());
  }
}
