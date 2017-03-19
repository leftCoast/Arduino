#include "DB_Files.h"
#include "fList.h"

        char        currentSong[25];
        byte        currentVol = 20;
extern  fList       fileList;
        
void writeParamFile(void) {

  File paramFile;
  char fileName[] = PARAM_FILE;

  if (SD.exists(fileName)) {
    SD.remove(fileName);
  }
  paramFile = SD.open(fileName, FILE_WRITE);
  paramFile.println(currentSong);
  paramFile.println(currentVol);
  paramFile.close();
}


void readParamFile(void) {

  File paramFile;
  char fileName[] = PARAM_FILE;
  byte i;
  char volStr[10];
  bool  done;

  paramFile = SD.open(fileName);
  if (paramFile) {
    i = 0;
    done = false;
    do {
      char aChar = paramFile.read();
      if (aChar == '\n') {
        currentSong[i - 1] = '\0'; // Kill the '\r'
        done = true;
      } else {
        currentSong[i] = aChar;
        i++;
      }
    }
    while (!done);
    i = 0;
    done = false;
    do {
      char aChar = paramFile.read();
      if (aChar == '\n') {
        volStr[i] = '\0';
        done = true;
      } else {
        volStr[i] = aChar;
        i++;
      }
    }
    while (!done);
    paramFile.close();
    currentVol = atoi(volStr);
  }
}


void fillFileList(const char* workingDir) {

  File        wd;
  File        entry;
  bool        done;
  fListItem*  newItem;

  wd = SD.open(workingDir);
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        if (!entry.isDirectory()) {
          newItem = new fListItem(entry.name());
          fileList.addObj(newItem);
        }
        entry.close();
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  } else {
    //lastFileError = F_FOF_ERR;  // Sadly, instead of returning a NULL, it just crashes.
  }
}



