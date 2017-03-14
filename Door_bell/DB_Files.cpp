#include "DB_Files.h"

char currentSong[25];
byte  currentVol = 20;


void writeParamFile(void) {

  File paramFile;
  char fileName[] = PARAM_FILE;

  if (SD.exists(fileName)) {
    SD.remove(fileName);
  }
  paramFile = SD.open(fileName,FILE_WRITE);
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
      if (aChar=='\n') {
        currentSong[i-1] = '\0';   // Kill the '\r'
        done = true;
      } else {
        currentSong[i] = aChar;
        i++;
      }
    }
    while(!done);
    i = 0;
    done = false;
    do {
      char aChar = paramFile.read();
      if (aChar=='\n') {
        volStr[i] = '\0';
        done = true;
      } else {
        volStr[i] = aChar;
        i++;
      }
    }
    while(!done);
    paramFile.close();
    currentVol = atoi(volStr);
  }
}
