#include <lilParser.h>
#include <SDTools.h>
#include <soundCard.h>
#include <strTools.h>
#include <mapper.h>
#include <cardIndex.h>

#define SOUND_CS     20
#define SOUND_SDCS   21
#define SOUND_DRQ    1
#define SOUND_R      15

lilParser   ourParser;
soundCard*  player;
filePath    wd;
filePath    autoPath;
char*       songPath;
bool        goingAuto;
cardIndex   ourPlayList(1);

enum cmds { noCmd,      // ALWAYS start with no Command. Or something simlar.
            playFile,   // The rest is up to you. help would be a good one. Have it list
            stopPlay,   // what the other commands are, how they work and what they do.
            playVol,
            shuffle,
            autoPlay,
            hwRestart,
            printWD,    
            changeDir,
            listDir,
            makeDir,
            deleteFile
            };          
            // Our list of commands.
            
enum pathPrefix { setRoot, upOne, upNPath, fullPath, relPath };              

void setup() {

   songPath = NULL;
   wd.setPath("/");
   goingAuto = false;
   if (SD.begin(SOUND_SDCS)) {
      Serial.println("SD seems good.");
   } else {
      Serial.println(F("File syem FAIL."));
      while(1);
   }
   player = new soundCard(soundCard_BREAKOUT,SOUND_CS,SOUND_DRQ,SOUND_R);
   if (player) {
      player->begin();
      Serial.println("Player seems good.");
   } else {
       Serial.println(F("Player FAIL."));
       while(1);
   }

   ourParser.addCmd(playFile,"play");     // Add play command.
   ourParser.addCmd(stopPlay,"stop");     // Add stop command.
   ourParser.addCmd(stopPlay,"pause");    // Add other stop command.
   ourParser.addCmd(playVol,"volume");    // Add volume command.
   ourParser.addCmd(playVol,"vol");       // Add other volume command.
   ourParser.addCmd(autoPlay,"auto");     // Add Toggle auto play command.
   ourParser.addCmd(hwRestart,"restart"); // Add reset command. Stops current song and restartes it.
   ourParser.addCmd(printWD, "pwd");      // Add pwd command. Print working directory.
   ourParser.addCmd(changeDir,"cd");      // Add cd command. Change directory.
   ourParser.addCmd(listDir,"ls");        // Add ls command. List directory.
   ourParser.addCmd(makeDir,"mkdr");      // Add mkdr command. Make directory.
   ourParser.addCmd(makeDir,"mkdir");     // We'll take it either way.
   ourParser.addCmd(deleteFile,"rm");     // Add rm command. Remove item.
}


void showCurs(void) {

  Serial.print(wd.getPath());
  Serial.print(" > ");
}

/*
void selectSong(int index) {

   autoPath
}



void checAuto(void) {

   int   songIndex;
   
   if (goingAuto) {
      if (!player->isPlaying()) {
         songIndex = ourPlayList.dealCard();
         if (!songINdex) {
            ourPlayList.shuffle();
            songIndex = ourPlayList.dealCard();
            selectSong(songIndex);     
         }  
      }
   }
}
*/

// Your loop where it parses out all your typings.
void loop(void) {
   
   char  inChar;
   int   command;
   bool  curs;
  
   idle();
   if (Serial.available()) {                                       // If serial has some data..
    curs = true;                                                  // Show the cursor after..
      inChar = Serial.read();                                       // Read out a charactor.
      Serial.print(inChar);                                         // If using development machine, echo the charactor.
      command = ourParser.addChar(inChar);                          // Try parsing what we have.
      switch (command) {                                            // Check the results.
         case noCmd        : curs = false;                  break;   // Nothing to report, move along.
         case playFile     : doPlay();                      break;
         case stopPlay     : doStop();                      break;
         case playVol      : doVolume();                    break;
         case autoPlay     : doAutoPlay();                  break;
         case hwRestart    : doRestart();                   break;
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
  Serial.println("play          With path, play that file. No path, last file played.");
  Serial.println("stop          Stops playing.");
  Serial.println("volume 0..100 Sets valume level.");
  //Serial.println("auto          Auto play random somgs..");
  Serial.println("restart       Stops current song and restarts it from beginning.");
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


void doPlay() {

   char* childName;

   childName = NULL;
   if (ourParser.numParams()) {
      heapStr(&childName,ourParser.getNextParam());
      wd.pushChildItemByName(childName);
      freeStr(&childName);
      heapStr(&songPath,wd.getPath());
      wd.popItem();
   }
   if (songPath) {
      Serial.print("Going to play : ");
      Serial.println(songPath);
      player->setSoundfile(songPath);
      player->command(play);
   } 
}


void doStop() { 
   
   player->command(fullStop);
   goingAuto = false;
}


void doVolume(void) {

   float volume;
   
   if (ourParser.numParams()) {                    // If we vot a param..
      volume = atof(ourParser.getNextParam());    // Grab it and decode it as a percent.
      Serial.print("Setting volume : ");
      Serial.println(volume);
      player->setVolume(volume);                   // Send it to the machine.
   }
}


void doAutoPlay() {
   
   //set auto path.
   //count song files.
   //setup card list.
   goingAuto = true;
}              


void doRestart() {

   if (player->isPlaying()) {
      player->command(restart);
   }
}


bool checkFile(pathItem* item) {

  if (item->name[0]=='.') return false;
  return true;
}


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

  pathItem*    trace;

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
      trace = wd.getChildList();                                // Grab a pointer to the first child.
      while(trace) {                                            // While we have a non-NULL pointer..
        if (checkFile(trace)) {                                 // Pass this child through the crucible of the user's filter function.
          Serial.print(trace->getName());                       // If passed, print it's name.
          if (trace->getType()!=fileType) {                     // If it's a directory..
            Serial.print("/");                                  // Add the "I'm a direcotry" slash to it.
          }                                                     // 
          Serial.println();                                     // Line feed..
        }                                                       //
        trace = (pathItem*)trace->dllNext;                                 // Jump to the next item on the list.
      }
    }
  }
}


// [mkdir] Create a new directory in the working directory using typed in parameter.
void makeDirectory(void) {

  tempStr param;
  int     numBytes;
  char*   pathBuff;
  
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
