 
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <lists.h>
#include <lilParser.h>
#include <soundCard.h>

// Let's try error numbers to orginase this.
//
#define F_NO_ERR    0

#define F_BOOT_ERR  1   //"SD Drive boot failure."
#define F_CMD_ERR   2   //"???"

#define F_RAM_ERR   3   //"Not enough RAM"
#define F_MPAR_ERR  4   //"Missing parameter."
#define F_RPAR_ERR  5   //"Failed to retrieve parameter."

#define F_NSF_ERR   6   //"No such file or directory."
#define F_FOF_ERR   7   //"Failed to open file."
#define F_FODF_ERR  8   //"Failed to open dest. file."
#define F_FOSF_ERR  9   //"Failed to open source file."

#define F_NOF_ERR  10   //"Not a file."
#define F_NOD_ERR  11   //"Not a directory."
#define F_ART_ERR  12   //"Already at root."
#define F_UKN_ERR  13   //"Unknown file error."

#define F_LAZY_ERR 14   //"Not yet implemented."
//
//

// Stuff for the command parser.
lilParser mParser;
enum      commands { noCommand, cp, rm, ls, pwd, cd, mkdir, rmdir, csf, cmdPlay, cmdPause, cmdReset, vol, svol, st };


char  workingDir[40] = "/";
char  cmdCursor[] = ">";
byte  lastFileError;

soundCard theSoundCard(soundCard_BREAKOUT);

void setup(void) { 

  Serial.begin(9600); while (!Serial);
  
  lastFileError = F_NO_ERR;
  if (!SD.begin(4)) { lastFileError = F_BOOT_ERR; }
  
  mParser.addCmd(cp, "cp");
  mParser.addCmd(rm, "rm");
  mParser.addCmd(ls, "ls");
  mParser.addCmd(pwd, "pwd");
  mParser.addCmd(cd, "cd");
  mParser.addCmd(mkdir, "mkdir");
  mParser.addCmd(rmdir, "rmdir");

  mParser.addCmd(csf, "csf");
  mParser.addCmd(cmdPlay, "play");
  mParser.addCmd(cmdPause, "pause");
  mParser.addCmd(cmdReset, "reset");
  mParser.addCmd(svol, "svol");
  mParser.addCmd(vol, "vol");
  mParser.addCmd(st, "st");
  //renameFile();
  if (!theSoundCard.begin()) {
    Serial.print(F("Sound card failed with error# "));
    Serial.println((int)theSoundCard.getLastError());
  }
  SDCleaner();
  Serial.print(cmdCursor);
}


// Print & clear the last file error.
void  lastErrOut(void) {

  switch(lastFileError) {
    case F_NO_ERR : break;

    case F_BOOT_ERR : Serial.println(F("SD Drive boot failure.")); break;
    case F_CMD_ERR  : Serial.println(F("???")); break;

    case F_RAM_ERR  : Serial.println(F("Not enough RAM")); break;
    case F_MPAR_ERR : Serial.println(F("Missing parameter.")); break;
    case F_RPAR_ERR : Serial.println(F("Failed to retrieve parameter.")); break;

    case F_NSF_ERR  : Serial.println(F("No such file or directory.")); break;
    case F_FOF_ERR  : Serial.println(F("Failed to open file.")); break;
    case F_FODF_ERR : Serial.println(F("Failed to open dest. file.")); break;
    case F_FOSF_ERR : Serial.println(F("Failed to open source file.")); break;

    case F_NOF_ERR  : Serial.println(F("Not a file.")); break;
    case F_NOD_ERR  : Serial.println(F("Not a directory.")); break;
    case F_ART_ERR  : Serial.println(F("Already at root.")); break;
    case F_UKN_ERR  : Serial.println(F("Unknown file error.")); break;

    case F_LAZY_ERR  : Serial.println(F("Not yet implemented.")); break;
  }
  lastFileError = F_NO_ERR;
}

void SDCleaner(void) {

// FSEVEN~1/
// TRASHE~1/
// SPOTLI~1/

  File  wd;
  File  entry;
  char  fileName[15];
  char* strPtr;
  byte  strLen;
  bool  done;
   
  wd = SD.open(workingDir);
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        strcpy(fileName,entry.name());
        entry.close();                                // Enough of you.
        if(fileName[0]=='_') {                        // Starts with a '_'
          strPtr = strstr(fileName,".MP3");
          if (strPtr) {
            strLen = strlen(fileName);
            strLen = strLen-4;
            if(strPtr==&(fileName[strLen])) {         // Ends with a .mp3
              SD.remove(fileName);                    // One of apple's files, kill it.
            }
          } 
        }
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  }
}


char* makeFullpath(char* filePath) {

  char* fullPath;

  if (filePath[0] == '/') {
    fullPath = (char*)malloc(strlen(filePath) + 1);
    if (fullPath) {
      strcpy(fullPath, filePath);
    }
  } else {
    fullPath = (char*)malloc(strlen(filePath) + strlen(workingDir) + 1);
    if (fullPath) {
      strcpy(fullPath, workingDir);
      strcat(fullPath, filePath);
    }
  }
}

bool renameFile(void) {

  //rename("Angel.mp3","Angel2.mp3");
  //SdFat::rename("Angel.mp3", "Angel2.mp3");
}


bool copyFile(void) {

  File  sourceFile;
  File  destFile;
  char* paramBuff;
  char* pathBuff;
  bool  success;

  success = false;
  if (mParser.numParams() >= 2) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      pathBuff = makeFullpath(paramBuff);
      free(paramBuff);
      if (SD.exists(pathBuff)) {
        sourceFile = SD.open(pathBuff);
        free(pathBuff);
        if (sourceFile) {
          paramBuff = mParser.getParam();
          if (paramBuff) {
            if (!strcmp("con", paramBuff)) {
              free(paramBuff);
              while (sourceFile.available()) {
                Serial.print(sourceFile.read());
              }
              Serial.println();
              success = true;
            } else {
              pathBuff = makeFullpath(paramBuff);
              free(paramBuff);
              destFile = SD.open(pathBuff, FILE_WRITE);
              free(pathBuff);
              if (destFile) {
                destFile.seek(0);
                while (sourceFile.available()) {
                  destFile.write(sourceFile.read());
                }
                destFile.close();
                success = true;
              } else {
                lastFileError = F_FODF_ERR;
              }
            }
          } else {
            lastFileError = F_RPAR_ERR;
            //Serial.println(F("Faild to retrieve parameter."));
          }
        } else {
          lastFileError = F_FOSF_ERR;
          //Serial.println(F("Faild to open source file."));
        }
        sourceFile.close();
      } else {
        lastFileError = F_NSF_ERR;
        //Serial.println(F("File doesn't exsist"));
      }
    } else {
      lastFileError = F_RPAR_ERR;
      //Serial.println(F("Faild to retrieve parameter."));
    }
  } else {
    lastFileError = F_MPAR_ERR;
    //Serial.println(F("Missing parameter."));
  }
  return success;
}


bool removeFile(void) {

  File  theFile;
  char* paramBuff;
  char* filePath;
  bool  success;

  success = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      filePath = makeFullpath(paramBuff);
      free(paramBuff);
      if (filePath) {
        if (SD.exists(filePath)) {
          theFile = SD.open(filePath);
          if (theFile) {
            if (!theFile.isDirectory()) {
              theFile.close();
              success = SD.remove(filePath);
              if (!success) {
                lastFileError = F_UKN_ERR;
                //Serial.println(F("Unknown file error."));
              }
            } else {
              lastFileError = F_NOF_ERR;
              //Serial.print(filePath); Serial.println(F(" is not a file."));
            }
          } else {
            lastFileError = F_FOF_ERR;
            //Serial.println(F("Failed to open file."));
          }
        } else {
          lastFileError = F_NSF_ERR;
          //Serial.println(F("No such file name."));
        }
        free(filePath);
      } else {
        lastFileError = F_RAM_ERR;
        //Serial.println(F("Failed to allocate RAM"));
      }
    } else {
      lastFileError = F_RAM_ERR;
      //Serial.println(F("Failed to allocate RAM"));
    }
  } else {
    lastFileError = F_MPAR_ERR;
    //Serial.println(F("Missing parameter."));
  }
  return success;
}


void listDirectory(void) {

  File  wd;
  File  entry;
  bool  done;

  wd = SD.open(workingDir);
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        Serial.print(entry.name());
        if (entry.isDirectory()) {
          Serial.println("/");
        } else {
          Serial.print("\t"); Serial.println(entry.size(), DEC);
        }
        entry.close();
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  } else {
    lastFileError = F_FOF_ERR;  // Sadly, instead of returning a NULL, it just crashes.
    //Serial.println(F("Fail to open file."));
  }
}


bool changeDirectory() {

  File  testFile;
  char* paramBuff;
  char* temp;
  int   trace;
  bool  success;

  success = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      if (!strcmp(paramBuff, "..")) {
        trace = strlen(workingDir) - 2;
        if (trace > 0) {
          while (workingDir[trace] != '/' && trace > 0) {
            trace--;
          }
          workingDir[trace + 1] = '\0';
        } else {      
          lastFileError = F_ART_ERR;
          //Serial.println(F("Already at root."));
        }
      } else {
        temp = makeFullpath(paramBuff);
        if (temp) {
          if (SD.exists(temp)) {
            testFile = SD.open(temp);
            if (testFile) {
              success = testFile.isDirectory();
              testFile.close();
              if (success) {
                strcpy(workingDir, temp);
              } else {
                lastFileError = F_NOD_ERR;
                //Serial.println(F("Path must be directory names, not filenames."));
              }
            } else {
              lastFileError = F_FOF_ERR;
              //Serial.println(F("Fail to opn file."));
            }
          } else {
            lastFileError = F_NSF_ERR;
            //Serial.println(F("No such file or directory."));
          }
          free(temp);
        } else {
          lastFileError = F_RAM_ERR;
          //Serial.println(F("Failed to allocate RAM"));
        }
      }
      free(paramBuff);
    } else {
      lastFileError = F_RAM_ERR;
      //Serial.println(F("Failed to allocate RAM"));
    }
    return success;
  } else {
    lastFileError = F_MPAR_ERR;
    //Serial.println(F("Missing paraeter."));
  }
  return success;
}


bool makeDirectory(void) {
  
  lastFileError = F_LAZY_ERR;
}


void clearDirectory(char* fullPath) {

  File  dir;
  File  entry;
  char  fileName[15];
  char* newPath;
  char* tempWD;
  bool  done;
  
  tempWD = malloc(strlen(workingDir)+1);
  strcpy(tempWD,workingDir);
  Serial.print("Ready to clear:");Serial.println(workingDir);
  dir = SD.open(workingDir);
  Serial.println("called open..");
  if (dir.isDirectory()) {
    dir.rewindDirectory();
    done = false;
    do {
      entry = dir.openNextFile();
      Serial.print("got entry: ");
      if (entry) {
        strcpy(fileName,entry.name());
        Serial.println(fileName);
        if (entry.isDirectory()) {
          entry.close();
          dir.close();
          strcat(fileName,"/");
          strcat(workingDir,newPath);
          Serial.print("clearing:");Serial.println(newPath);
          clearDirectory(newPath);
          Serial.print("removing:");Serial.println(newPath);
          SD.rmdir(newPath);
          free(newPath);
          dir = SD.open(fullPath);
          dir.rewindDirectory();
        } else {
          newPath = makeFullpath(fileName);
          Serial.print("deleting:");Serial.println(newPath);
          SD.remove(newPath);
          free(newPath);
        }
      } else {
        done = true;
      }
    } while(!done);
    dir.close();
  } else Serial.println("not a dir?");
  strcpy(workingDir,tempWD);
  free(tempWD);
}

bool removeDirectory(void) {

  File  theFile;
  char* paramBuff;
  char* filePath;
  bool  success;
  char* tempWD;
  
  success = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      filePath = makeFullpath(paramBuff);
      free(paramBuff);
      if (filePath) {
        if (SD.exists(filePath)) {
          theFile = SD.open(filePath);
          if (theFile) {
            if (theFile.isDirectory()) {
              theFile.close();
              clearDirectory(filePath);
              success = SD.rmdir(filePath);
              if (!success) {
                lastFileError = F_UKN_ERR;
                //Serial.println(F("Unknown file error."));
              }
            } else {
              lastFileError = F_NOD_ERR;
              //Serial.print(filePath); Serial.println(F(" is not a dir."));
            }
          } else {
            lastFileError = F_FOF_ERR;
            //Serial.println(F("Failed to open file."));
          }
        } else {
          lastFileError = F_NSF_ERR;
          //Serial.println(F("No such file name."));
        }
        free(filePath);
      } else {
        lastFileError = F_RAM_ERR;
        //Serial.println(F("Failed to allocate RAM"));
      }
    } else {
      lastFileError = F_RAM_ERR;
      //Serial.println(F("Failed to allocate RAM"));
    }
  } else {
    lastFileError = F_MPAR_ERR;
    //Serial.println(F("Missing parameter."));
  }
  return success;
}



bool chooseMP3File(void) {

  char* paramBuff;
  char* fullPath;
  bool  success;

  success = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      fullPath = makeFullpath(paramBuff);
      free(paramBuff);
      if (fullPath) {
        if (SD.exists(fullPath)) {
          if (theSoundCard.setSoundfile(fullPath)) {
            success = true;
          } else {
            Serial.print(F("setSoundfile() failed with error# "));
            Serial.println((int)theSoundCard.getLastError());
          }
        } else {
          lastFileError = F_NSF_ERR;
          //Serial.println(F("File doesn't exsist"));
        }
        free(fullPath);
      } else {
        lastFileError = F_RAM_ERR;
        //Serial.println(F("Failed to allocate RAM"));
      }
    } else {
      lastFileError = F_RAM_ERR;
      //Serial.println(F("Failed to allocate RAM"));
    }
  } else {
    lastFileError = F_MPAR_ERR;
    //Serial.println(F("Missing parameter."));
  }
  return success;
}


bool playFile(void) {

  bool  success;

  success = false;

  if (mParser.numParams()) {
    chooseMP3File();
  }
  if (theSoundCard.command(play)) {
    success = true;
  } else {
    Serial.print(F("command(play) failed with error# "));
    Serial.println((int)theSoundCard.getLastError());
  }
  return success;
}


bool pauseFile(void) {

  bool  success;

  success = false;
  if (theSoundCard.command(pause)) {
    success = true;
  } else {
    Serial.print(F("command(pause) failed with error# "));
    Serial.println((int)theSoundCard.getLastError());
  }
  return success;
}


bool resetFile(void) {

  bool  success;

  success = false;
  if (theSoundCard.command(restart)) {
    success = true;
  } else {
    Serial.print(F("command(restart) failed with error# "));
    Serial.println((int)theSoundCard.getLastError());
  }
  return success;
}


bool setVol(void) {

  char* paramBuff;
  int   vol;
  bool  success;

  success = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      vol = atoi(paramBuff);
      free(paramBuff);
      if (vol < 0) {
        vol = 0;
      } else if (vol > 256) {
        vol = 256;
      }
      theSoundCard.setVolume(vol);
      success = true;
    } else {
      lastFileError = F_RPAR_ERR;
      //Serial.println(F("Faild to retrieve parameter."));
    }
  } else {
    lastFileError = F_MPAR_ERR;
    //Serial.println(F("Missing parameter."));
  }
  return success;
}


bool getVol(void) {

  Serial.print("Volume : "); Serial.println(theSoundCard.getVolume());
  if (mParser.numParams()) {
    setVol();
    Serial.print("New volume : "); Serial.println(theSoundCard.getVolume());
  }
  return true;
}


void setSleepTime(void) {

    char* paramBuff;
    int   timeMs;
    bool  success;

  success = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      timeMs = atoi(paramBuff);
      free(paramBuff);
      Serial.print("Setting sleep time to:");Serial.println(timeMs);
      theSoundCard.setTime(timeMs);
    }
  }
}


void loop() {

  char  inChar;
  int   command;
  
  idle();
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       
      case noCommand : break;
      case cp : copyFile(); break;
      case rm : removeFile(); break;
      case ls : listDirectory(); break;
      case pwd : Serial.println(workingDir); break;
      case cd : changeDirectory(); break;
      case mkdir : makeDirectory(); break;
      case rmdir : removeDirectory(); break;

      case csf : chooseMP3File(); break;
      case cmdPlay : playFile(); break;
      case cmdPause : pauseFile(); break;
      case cmdReset : resetFile(); break;
      case svol : setVol(); break;
      case vol : getVol(); break;
      case st : setSleepTime(); break;
      
      default : lastFileError = F_CMD_ERR; break;
    }
    if (lastFileError) lastErrOut();
    if (command) Serial.print(cmdCursor);
  }
}


