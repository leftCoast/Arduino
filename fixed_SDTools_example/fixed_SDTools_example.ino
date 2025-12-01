/*
  pwd   - Print working directory.
  cd    - Change directory,
  ls    - List working directory.
  mkdr  - Make new directory.
  rm    - Delete file/directory.
*/

#include <SD.h>
#include <adafruit_1947.h>
#include <strTools.h>
#include <lilParser.h>
#include <SDTools.h>
#include <filePath.h>


#define DSP_CS      10		// We need to fire up the display because it's on the SPI bus.
#define SD_CS        4     // SD chip select pin number. Change to suit your setup.


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


void setup() {

	Serial.begin(9600);
	screen = (displayObj*)new adafruit_1947(DSP_CS,-1);
	if (!screen) {                                      // If we can't get the screen running..
		Serial.println("NO SCREEN!");                     // Send an error out the serial port.
		Serial.flush();                                   // Make sure it goes out!
		while (true);                                     // Lock processor here forever.
	}                                                   //
  screen->begin();                                    //
	screen->fillScreen(&black);                         // Looks like we have a screen.
                                                      //
	if (!SD.begin(SD_CS)) {                             // If we can not initialze a SD drive.
		Serial.println("No SD Drive!");                   // Tell the user.
		while (1);                                        // Just stop here.
	}
	ourParser.addCmd(printWD, "pwd");                   // Add pwd command  [ If they type "pwd" they mean printWD ]
	ourParser.addCmd(changeDir, "cd");                  // Add cd command
	ourParser.addCmd(listDir, "ls");                    // Add ls command
	ourParser.addCmd(makeDir, "mkdr");                  // Add mkdr command
	ourParser.addCmd(makeDir, "mkdir");                 // We'll take it either way.
	ourParser.addCmd(deleteFile, "rm");                 // Add rm command.
                                                      //
  showComs();                                         // Lets see the list of commands.
  wd.setPath("/");                                    // Initial path setting.
  showCurs();                                         // Default curser setting.
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
			default           : showComs();                    break;   // No idea. Show them the list.
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

void showComs(void) {

  Serial.println("Possible commands..");
  Serial.println("pwd           show working directory.");
  Serial.println("ls            list items in the working directory.");
  Serial.println("cd *path*     Can be fullpath starting at '/' or");
  Serial.println("              reletive path startig with just a");
  Serial.println("              directoty. Or .. to go back to");
  Serial.println("              parent directory.");
  Serial.println("mkdir *path*  Can be fullpath starting at '/' or"); 
  Serial.println("              reletive path startig with just a");
  Serial.println("              directoty.");
  Serial.println("rm *path*     Can be fullpath starting at '/' or"); 
  Serial.println("              reletive path starting with just a");
  Serial.println("              directoty. Will delete files or");
  Serial.println("              directories. Directories do NOT need");
  Serial.println("              to be empty, this will empty them.");
  Serial.println("              NOTE: Be careful with this. Emptying");
  Serial.println("              directories can eat a lot of RAM if");
  Serial.println("              they are nested deeply.");
}


bool checkFile(pathItem*	item) {

  if (item->name[0]=='.') return false;
  return true;
}


//enum pathPrefix { setRoot, upOne, upNPath, fullPath, relPath };
//
//      /         ->  Set to root.
//      ..        ->  Go up one.
//      ..path    ->  Go up, then relative path.
//      /path     ->  Full path.
//      path      ->  Relative folder.
//
pathPrefix decodePrefix(const char* param) {

  delay(3);                                                 // Dash said to put this in to fix wicked bug. I did, it worked. No clue!
  if (!strcmp(param,"/")) return setRoot;                   // Just "/" alone means root. setRoot.
  else if (param[0]=='/') return fullPath;                  // Starts with '/' + more..   fullPath.
  else if (!strcmp(param,"..")) return upOne;               // Just ".." means go up one. upOne.
  else if (param[0]=='.' && param[1]=='.') return upNPath;  // Starts with ".." + more..  upNPath.
  else return relPath;                                      // Default, relitive path.    relPath.
}


// [ls] Lists all the files in the working direcotory.
void listDirectory(void) {

  pathItem*		trace;

  if (ourParser.numParams()) {                                  // If they typed in something past the command.
    Serial.println("Sorry, ls takes no params. Ignoring..");    // Let 'em know..
  }                                                             //
  if (wd.getPathType()==fileType) {                             // If this is pointing to a file..? 
    Serial.println("Sorry, this is a file, not a directory.");  // Just wrong, we can't list a file.
    return;                                                     // Just walk away..
  } else {                                                      // Else, not a file. Good!
    wd.refreshChildList();                                      // Make sure we have an up to date child list..
    if (wd.numChildItems()==0) {                                // Wait, no children to list?
      Serial.println("This directory is empty.");               // Tell the user.
      return;                                                   // And walk away.
    } else {                                                    // Else, we DO have kids to list.
      trace = wd.getChildList();										    // Grab a pointer to the first child.
      while(trace) {															 // While we have a non-NULL pointer..
        if (checkFile(trace)) {											 // Pass this child through the crucible of the user's filter function.
          Serial.print(trace->getName());                       // If passed, print it's name.
          if (trace->getType()!=fileType) {                     // If it's a directory..
            Serial.print("/");                                  // Add the "I'm a direcotry" slash to it.
          }                                                     // 
          Serial.println();                                     // Line feed..
        }                                                       //
        trace = (pathItem*)trace->dllNext;											// Jump to the next item on the list.
      }
    }
  }
}


// [mkdir] Create a new directory in the working directory using typed in parameter.
void makeDirectory(void) {

  tempStr param;
  int     numBytes;
  char*   pathBuff;
  Serial.println("Create directory?");
  pathBuff = NULL;                                // Make SURE these start at NULL.
  if (ourParser.numParams()==1) {                 // If they typed in something past the command.
    param.setStr(ourParser.getParamBuff());       // We get the first parameter. Should be the new folder's name/path.
    switch (decodePrefix(param.getStr())) {       // Let's see what they are asking for..
      case setRoot  :                             // Create root..
      case upOne    :                             // Pop up a direcoty..
      case upNPath  :                             // Pop up and go elsewhere..
        Serial.println("Illegal command");        // Not going to deal with any of those for now.
      break;                                      // And we're off.
      case fullPath :                             // Full path, we can do that.
        if(!SD.mkdir(param.getStr())) {           // If we can't make the direcotry..
          Serial.println("File error.");          // Tell 'em..
        }                                         //
      break;                                      // In any case, we're off.
      case relPath  :                             // Relitive path..
        numBytes = wd.numPathBytes();             // We need our wd path..
        numBytes = numBytes + param.numChars();   // And the param .
        if (resizeBuff(numBytes,&pathBuff)) {     // If we can get the RAM..
          strcpy(pathBuff,wd.getPath());          // Stuff in our wd path.
          strcat(pathBuff,param.getStr());        // Add the param text.
          if(!SD.mkdir(pathBuff)) {               // If we can't make the direcotry..
            Serial.println("File error.");        // Tell 'em..
          }                                       //
          resizeBuff(0,&pathBuff);                // recycle the RAM.
        }                                         //
      break;                                      // In any case, we're off.
    }
  }
}


// [cd] Change the working directory. (See prefix commands above.)
void changeDirectory(void) {

  tempStr       param;
  tempStr       savedPath;
 
  savedPath.setStr(wd.getPath());                           // Save the path we have, just in case.
  if (ourParser.numParams()==1) {                           // If they typed in a parameter (name/path).
    param.setStr(ourParser.getParamBuff());                 // We grab the name.
    switch(decodePrefix(param.getStr())) {                  // Decide on how the param is prefixed..
      case setRoot  :                                       // Just set to root..
        //Serial.println("Setting root");
        wd.setPath("/");                                    // Easy peasy!
      break;                                                //
      case upOne    :                                       // Want to go up one deirectory..
        //Serial.println("upOne");
        if (wd.getPathType()==rootType) {                   // If we're at the root already..
          Serial.println("Sorry, already at root.");        // Send error. Can't one up more root.
        } else {                                            // Else, not root?
          wd.popItem();                                     // Go ahead and pop off the item.
        }                                                   //
      break;                                                //
      case upNPath  :                                       // Up then, relitive path..
        //Serial.println("upNPath");
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
        //Serial.println("fullPath");
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
         //Serial.println("relPath");
        if (wd.addPath(param.getStr())) {                   // If we can add this relitive path bit..
          if (wd.getCurrItem()->getType()==fileType) {      // If this is pointing to a file..
            wd.setPath(savedPath.getStr());                 // Put it all back..
            Serial.println("Path is not a directory.");     // Let 'em know.
          }                                                 //
          //Serial.print("Added:");
          Serial.println(param.getStr());
        } else {                                            // Else, couldin't find the file.
          wd.setPath(savedPath.getStr());                   // Put it all back..
          Serial.println("Path not found.");                // Let 'em know.
        }                                                   //
      break;                                                // We're off..
      default : Serial.println("Should never get here!");
    }
  }
}


//[rm] Delete a file or directory..
void deleteItem(void) {

  tempStr param;
  tempStr savedPath;
 
  savedPath.setStr(wd.getPath());             // Save the path we have, just in case.
  if (ourParser.numParams()==1) {             // If they typed in a parameter (name).
    param.setStr(ourParser.getParamBuff());   // We grab the name.
    switch(decodePrefix(param.getStr())) {    // Decide on how the param is prefixed..
      case setRoot  :                         // Just set to root..
      case upOne    :                         // Want to go up one deirectory..
      case upNPath  :                         // Up then, relitive path..
      case fullPath :                         // Full path..
        Serial.println("Illegal command.");   // We don't do those with delete.
      break;                                  // Skipping off..
      case relPath  :                         // Relitive path..
        if (wd.addPath(param.getStr())) {     // If we can add this relitive path bit..
          if(!wd.deleteCurrentItem()) {       // If we failed to delete the item..
            Serial.println("File error.");    // Something went wrong.
          }                                   //
        } else {                              // Else, couldin't find the file.
          Serial.println("Path not found.");  // Let 'em know.
        }                                     //
        wd.setPath(savedPath.getStr());       // Put it all back..
      break;                                  // Enough!
    }
  }
}
