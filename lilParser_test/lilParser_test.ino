#include <SD.h>
#include <lists.h>
#include <lilParser.h>

enum commands { noCommand, cp, rm, ls, pwd, cd, mkdir, rmdir };

lilParser mParser;

char workingDir[40] = "/";
char cmdCursor[] = ">";

void setup(void) {

  Serial.begin(9600);
  while (!Serial);
  if (!SD.begin(4)) {
    Serial.println(F("SD Drive failed to boot."));
    Serial.println(F("Shutting down."));
    while(true);
  }
  mParser.addCmd(cp, "cp");
  mParser.addCmd(rm, "rm");
  mParser.addCmd(ls, "ls");
  mParser.addCmd(pwd, "pwd");
  mParser.addCmd(cd, "cd");
  mParser.addCmd(mkdir, "mkdir");
  mParser.addCmd(rmdir, "rmdir");
  Serial.print(cmdCursor);
}


void makeFullpath(char* filePath,char* fullPath) {
    
    if (filePath[0]=='/') {
      strcpy(fullPath,filePath);
    } else {
      strcpy(fullPath,workingDir);
      strcpy(fullPath,filePath);
    }
  }


 // This one needs some more error checking. 
bool copyFile(void) {

    char  filePath[20];
    char  fullPath[40];
    char  destPath[20];
    char  destFullPath[40];
    File  sourceFile;
    File  destFile;
    
    if (mParser.numParams()) {
      mParser.getParam(filePath);
      makeFullpath(filePath,fullPath);
      if (SD.exists(fullPath)) {
        if (mParser.getParam(destPath)) {
          if (!strcmp("con",destPath)) {
            sourceFile = SD.open(fullPath);
            while(sourceFile.available()) {
              Serial.print(sourceFile.read());
            }
            Serial.println();
            sourceFile.close();
          } else {
            makeFullpath(destPath,destFullPath);
            destFile = SD.open(destFullPath,FILE_WRITE);
            if(destFile) {
              sourceFile = SD.open(fullPath);
              destFile.seek(0);
              while(sourceFile.available()) {
                destFile.write(sourceFile.read());
              }
              sourceFile.close();
              destFile.close();
            } else {
              Serial.println(F("Malformed destination name"));
            }
          }
        } else {
          Serial.println(F("Missing destination name"));
        }
      } else {
        Serial.println(F("File doesn't exsist"));
      }
    } else {
      Serial.println(F("Missing file name"));
    }
  }


bool removeFile(void) {

    char fileName[20];
    char filePath[40];
    File theFile;
    bool  success;

    success = false;
    if (mParser.numParams()) {
      mParser.getParam(fileName);
      strcpy(filePath,workingDir);
      strcat(filePath,fileName);
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
          Serial.print(fileName);Serial.println(F(" is not a file."));
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
      if (!strcmp(path,"..")) {
        trace = strlen(workingDir)-2;
        if (trace>0) {
          while(workingDir[trace]!='/' && trace>0) {
            trace--;
          }
          workingDir[trace+1] = '\0';
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


  void loop() {

    char      inChar;
    commands  command;

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
        case -1 : Serial.println("???"); break;
      }
      if (command) Serial.print(cmdCursor);
    }
  }


