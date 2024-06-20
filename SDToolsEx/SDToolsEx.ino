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
  pathItem*		curItem;

  curItem = wd.getCurrItem();
  if (curItem->getType()==fileType) {
     Serial.println("Sorry, this is a file, not a directory.");
     return;
  } else {
    wd.refreshChildList();
    if (wd.numChildItems()==0) {
      Serial.println("Tmkdir dog cathis directory is empty.");
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
  pathItemType  typeResult;
  char*         pathBuff;
  int           numBytes;

  pathBuff = NULL;                                                              // Start all these at NULL.
  if (ourParser.numParams()==1) {                                               // If they typed in something past the command.
    param.setStr(ourParser.getParamBuff());                                     // We get the first parameter, assume its the new folder's name.
    typeResult = wd.checkPathPlus(param.getStr());                                 // We'll go have a look.
    switch (typeResult) {                                                       // IF we did this, what would happen?
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


// [cd] Change the working directory.
//          /  -> Set to root.
//          .. -> Go up one.
//     /name.. -> Full path.
//        name -> Local folder.

void changeDirectory(void) {

  tempStr       param;                              // TempStr, auto allocates and holds strings during function.
  tempStr       savedPath;
  pathItem*     curItem;
  int           numBytes;
  char*         pathBuff;

  pathBuff = NULL;                                                // These pointer things start at NULL.
  savedPath.setStr(wd.getPath());                                 // Save the path we have, just in case.
  curItem = wd.getCurrItem();                                     // Nice to know what we're looking at.
  if (ourParser.numParams()==1) {                                 // If they typed in a parameter (name).
    param.setStr(ourParser.getParamBuff());                       // We grab the name.
    if (!strcmp("..",param.getStr())) {                           // If they want to go back up a click..
      if (curItem->getType()==rootType) {                         // If we're at the root already..
        Serial.println("Sorry, already at root.");                // Send error. Can't one up root.
      } else {                                                    // Else, not root?
        wd.popItem();                                             // Go ahead and pop off the item.
      }                                                           //
    } else if (!strcmp("/",param.getStr())) {                     // Else if they handed us the root itself.
      wd.setPath("/");                                            // Whatever, just set it at root and be dne wih it.
    } else if (param.getStr()[0]=='/') {                          // Else, not root, but starts with /? Full path.
      if (!wd.setPath(param.getStr())) {                          // If we can't set this path..
        wd.setPath(savedPath.getStr());                           // Put it back..
        Serial.println("Path not found.");                        // Error.
      } else {                                                    // Else, we got a path, check it out.
        if (wd.getCurrItem()->getType()==fileType) {              // If this is pointing to a folder..
          wd.setPath(savedPath.getStr());                         // Put it back..
          Serial.println("Path is not a directory.");             // Working DIRECTORY can't be a file.
        }                                                         //
      }                                                           //
    } else {                                                      // Else, this is a relitive path.
      numBytes = wd.numPathBytes();                               // Num Bytes..
      numBytes = numBytes + param.numChars() + 1;                 // Plus these and one.
      if (resizeBuff(numBytes,&pathBuff)) {                       // If we can grab some RAM..
        strcpy(pathBuff,wd.getPath());                            // Stuff in our path.
        strcat(pathBuff,param.getStr());                          // Add the param text.
        if (!wd.setPath(pathBuff)) {                              // If it doesn't work..
          wd.setPath(savedPath.getStr());                         // Put it all back..
          Serial.println("Path not found.");                      // Let 'em know.
        } else {                                                  // Else we have a valid path..
          if (wd.getCurrItem()->getType()==fileType) {            // If this is pointing to a file..
            wd.setPath(savedPath.getStr());                       // Put it all back..
            Serial.println("Path is not a directory.");           // Let 'em know.
          }                                                       //
        } 
        resizeBuff(0,&pathBuff);                                  // Recycle the RAM.
      } else {                                                    //
        Serial.println("Memory error.");                          // Not enough RAM. WOW!
      }
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
