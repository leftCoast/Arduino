#include <SD.h>

#define PARAM_FILE  "/dbos/CONFIG.SYS"

extern char currentSong[];
extern byte  currentVol;

void writeParamFile(void);
void readParamFile(void);
void fillFileList(const char* workingDir);

