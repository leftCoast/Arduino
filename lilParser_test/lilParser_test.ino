#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <lists.h>
#include <lilParser.h>
#include <soundCard.h>

enum commands { noCommand, cp, rm, ls, pwd, cd, mkdir, rmdir, csf, cmdPlay, cmdPause };

lilParser mParser;

char workingDir[20] = "/";
char cmdCursor[] = ">";

soundCard theSoundCard(soundCard_SHIELD);
byte vol;

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
  
  vol = 20;
  if (theSoundCard.begin()) {
    Serial.println(F("Sound card ready."));
  } else {
    Serial.print(F("Sound card failed with error# "));
    Serial.println((int)theSoundCard.getLastError());
  }
  Serial.print(cmdCursor);
}


void makeFullpath(char* filePath, char* fullPath) {

  if (filePath[0] == '/') {
    strcpy(fullPath, filePath);
  } else {
    strcpy(fullPath, workingDir);
    strcpy(fullPath, filePath);
  }
}


bool copyFile(void) {

  File  sourceFile;
  File  destFile;
  char  charBuff[80];
  byte  nextByte;
  bool  success;

  success = false;
  if (mParser.numParams() >= 2) {
    if (mParser.getParam(&(charBuff[0]))) {
      nextByte = strlen(&(charBuff[0])) + 2;
      makeFullpath(&(charBuff[0]), &(charBuff[nextByte]));
      if (SD.exists(&(charBuff[nextByte]))) {
        sourceFile = SD.open(&(charBuff[nextByte]));
        if (sourceFile) {
          if (mParser.getParam(&(charBuff[0]))) {
            if (!strcmp("con", &(charBuff[0]))) {
              while (sourceFile.available()) {
                Serial.print(sourceFile.read());
              }
              Serial.println();
              success = true;
            } else {
              nextByte = strlen(&(charBuff[0])) + 2;
              makeFullpath(&(charBuff[0]), &(charBuff[nextByte]));
              destFile = SD.open(&(charBuff[nextByte]), FILE_WRITE);
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

  char fileName[20];
  char filePath[40];
  File theFile;
  bool  success;

  success = false;
  if (mParser.numParams()) {
    mParser.getParam(fileName);
    strcpy(filePath, workingDir);
    strcat(filePath, fileName);
    if (SD.exists(filePath)) {
      theFile = SD.open(filePath);
      if (!theFile.isDirectory()) {
        theFile.close();
        success = SD.remove(filePath);
        if (!success) {
          Serial.println(F("Unknown file error."));
        }
      } else {
        theFile.close();
        Serial.print(fileName); Serial.println(F(" is not a file."));
      }
    } else {
      Serial.println(F("No such file name."));
    }
  } else {
    Serial.println(F("Missing file name."));
  }
  return success;
}


void listDirectory(void) {

  File  wd;
  File  entry;
  bool  done;

  wd = SD.open(workingDir);
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
}


bool changeDirectory() {

  File  testFile;
  bool  success;
  char  path[20];
  char  temp[40];
  int  trace;

  success = false;
  if (mParser.numParams()) {
    mParser.getParam(path);
    if (!strcmp(path, "..")) {
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
      strcpy(temp, workingDir);
      strcat(temp, path);
      if (SD.exists(temp)) {
        testFile = SD.open(temp);
        success = testFile.isDirectory();
        if (success) {
          strcpy(workingDir, temp);
        } else {
          Serial.println(F("Path must be directory names, not filenames."));
        }
      } else {
        Serial.println(F("No such directory."));
      }
    }
  } else {
    Serial.println(F("Missing directory name."));
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

  char  charBuff[20];
  byte  nextByte;
  bool  success;

  success = false;
  if (mParser.numParams()) {
    if (mParser.getParam(&(charBuff[0]))) {
      nextByte = strlen(&(charBuff[0])) + 2;
      makeFullpath(&(charBuff[0]), &(charBuff[nextByte]));
      if (SD.exists(&(charBuff[nextByte]))) {
        if (theSoundCard.setSoundfile(&(charBuff[nextByte]))) {
        //if (theSoundCard.setSoundfile("RADIOACT.MP3")) {
          Serial.print(&(charBuff[nextByte]));
          Serial.println(" Queued and ready to play.");
          success = true;
        } else {
          Serial.print(F("setSoundfile() failed with error# "));
          Serial.println((int)theSoundCard.getLastError());
        }
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


void loop() {

  char      inChar;
  commands  command;

  idle();
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch ((int)command) {                       // Cheat: Allows -1 passed back as error.
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
      
      case -1 : Serial.println("???"); break;
    }
    if (command) Serial.print(cmdCursor);
  }
}


