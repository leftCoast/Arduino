#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <lists.h>
#include <lilParser.h>
#include <soundCard.h>

enum commands { noCommand, cp, rm, ls, pwd, cd, mkdir, rmdir, csf, cmdPlay, cmdPause, cmdReset, vol, svol };

lilParser mParser;

char workingDir[20] = "/";
char cmdCursor[] = ">";

soundCard theSoundCard(soundCard_BREAKOUT);

void setup(void) {

  Serial.begin(9600);
  while (!Serial);
  if (!SD.begin(4)) {
    Serial.println(F("SD Drive failed to boot."));
    Serial.println(F("Shutting down."));
    while (true);
  }
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

  if (theSoundCard.begin()) {
    Serial.println(F("Sound card ready."));
  } else {
    Serial.print(F("Sound card failed with error# "));
    Serial.println((int)theSoundCard.getLastError());
  }
  Serial.print(cmdCursor);
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
                Serial.println(F("Faild to open dest. file."));
              }
            }
          } else {
            Serial.println(F("Faild to retrieve parameter."));
          }
        } else {
          Serial.println(F("Faild to open source file."));
        }
        sourceFile.close();
      } else {
        Serial.println(F("File doesn't exsist"));
      }
    } else {
      Serial.println(F("Faild to retrieve parameter."));
    }
  } else {
    Serial.println(F("Missing parameter."));
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
                Serial.println(F("Unknown file error."));
              }
            } else {
              Serial.print(filePath); Serial.println(F(" is not a file."));
            }
          } else {
            Serial.println(F("Failed to open file."));
          }
        } else {
          Serial.println(F("No such file name."));
        }
        free(filePath);
      } else {
        Serial.println(F("Failed to allocate RAM"));
      }
    } else {
      Serial.println(F("Failed to allocate RAM"));
    }
  } else {
    Serial.println(F("Missing parameter."));
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
    Serial.println(F("Fail to open file."));
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
          Serial.println(F("Already at root."));
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
                Serial.println(F("Path must be directory names, not filenames."));
              }
            } else {
              Serial.println(F("Fail to opn file."));
            }
          } else {
            Serial.println(F("No such file or directory."));
          }
          free(temp);
        } else {
          Serial.println(F("Failed to allocate RAM"));
        }
      }
      free(paramBuff);
    } else {
      Serial.println(F("Failed to allocate RAM"));
    }
    return success;
  } else {
    Serial.println(F("Missing paraeter."));
  }
  return success;
}


bool makeDirectory(void) {
  Serial.println(F("Not yet implemented."));
}

bool removeDirectory(void) {
  Serial.println(F("Not yet implemented."));
}


bool chooseMP3File(void) {

  char* paramBuff;
  char* fullPath;
  byte  nextByte;
  bool  success;

  success = false;
  Serial.print("Num params ");Serial.println(mParser.numParams());
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    Serial.print("got param ");Serial.println(paramBuff);
    if (paramBuff) {
      fullPath = makeFullpath(paramBuff);
      free(paramBuff);
      if (fullPath) {
        if (SD.exists(fullPath)) {
        if (theSoundCard.setSoundfile(fullPath)) {
          Serial.print(fullPath);
            Serial.println(" Queued and ready to play.");
            success = true;
          } else {
            Serial.print(F("setSoundfile() failed with error# "));
            Serial.println((int)theSoundCard.getLastError());
          }
        } else {
          Serial.println(F("File doesn't exsist"));
        }
        free(fullPath);
      } else {
        Serial.println(F("Failed to allocate RAM"));
      }
    } else {
      Serial.println(F("Failed to allocate RAM"));
    }
  } else {
    Serial.println(F("Missing parameter."));
  }
  return success;
}


bool playFile(void) {

  bool  success;

  success = false;
  if (theSoundCard.command(play)) {
    Serial.println(F("Playing."));
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
      Serial.println(F("Faild to retrieve parameter."));
    }
  } else {
    Serial.println(F("Missing parameter."));
  }
  return success;
}


bool getVol(void) {

  Serial.print("Volume : "); Serial.println(theSoundCard.getVolume());
  return true;
}


void loop() {

  char  inChar;
  int   command;

  idle();
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       // Cheat: Allows -1 passed back as error.
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

      default : Serial.println("???"); break;
    }
    if (command) Serial.print(cmdCursor);
  }
}


