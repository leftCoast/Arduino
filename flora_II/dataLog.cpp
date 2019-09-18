#include "dataLog.h"
#include "globals.h"
#include "debug.h"

dataLog::dataLog(void) {
  
  mHaveSD = false;
  clearFlags();
}


dataLog::~dataLog(void) {  }


bool dataLog::isLogging(void) { return ourParamObj.getRunNum()>0; }


void dataLog::begin(bool haveSD) {

  int runNum;
  
  if (haveSD) {                         // If this is even worth trying..
    mHaveSD = true;                     // They say the hardware is up and runnig.
    runNum = ourParamObj.getRunNum();   // Waking up from staysis, grab copy of last run number.
    if (runNum) {                       // If we were running..
      ourParamObj.setRunNum(0);         // Its a new life, we're not running yet.
      if (setLogging(true)) {           // If we can restart..
        runNum++;                       // Bump up our saved run number.
        ourParamObj.setRunNum(runNum);  // Save it to "disk".
      } else {
        Serial.println("setLogging() fail?!");
      }
    }
  }
}


void dataLog::readHeader(logHeader* aHeader) {
  
  unsigned long bytesRead;
  
  bytesRead = logFile.read((char*)aHeader, sizeof(logHeader));   // Make a grab at the block.
  //debugger.trace("Read #bytes    :",bytesRead,false);
  //debugger.trace("Read numLines  :",(int)(aHeader->numLines),false);
  //debugger.trace("Read numWLines :",(int)(aHeader->numWetLines),false);
}


void dataLog::writeHeader(logHeader* aHeader) {

  //debugger.trace("Writing numLines  :",(int)(aHeader->numLines),false);
  //debugger.trace("Writing numWLines :",(int)(aHeader->numWetLines),false);
  logFile.seek(0);                                    // Point at the start of the file..
  logFile.write((char*)aHeader, sizeof(logHeader)); // Write the bytes.
}


// Returns not success, but the actual resulting state of logging.
bool dataLog::setLogging(bool onOff) {

  if (!mHaveSD) return false;                   // Sanity, no hardware? Bail.
  if (onOff) {                                  // If we want to start..
    if (ourParamObj.getRunNum()==0) {           // If we're NOT already running?
      logFile = SD.open(LOG_FILE, FILE_WRITE);  // Lets try to open/create a logfile.
      if (logFile) {                            // If we had success..
        logFile.close();                        // Close the file.
        ourParamObj.setRunNum(1);               // Flag we're running
      } else {
        ourParamObj.setRunNum(0);               // In any case, we're not running.
      }
    }
  } else {
    ourParamObj.setRunNum(0);                   // Shut it down!
  }
  return ourParamObj.getRunNum()>0;             // Return the result.
}


void dataLog::deleteLog(void) {

  if (!mHaveSD) return;                         // Sanity, no hardware? Bail.
  logFile = SD.open(LOG_FILE, FILE_WRITE);
  if (logFile) {
    logFile.close();
    SD.remove(LOG_FILE);
  }
}


void dataLog::saveDataRecord(void) {

  logHeader aHeader;
  
  
  if (!mHaveSD) return;                                       // Sanity, no hardware? Bail.
  if (isLogging()) {                                          // If we are actually runing..
    logFile = SD.open(LOG_FILE,FILE_READ);                    // Attempt opening the log file.
    if (logFile) {
      readHeader(&aHeader);
      logFile.close();
      logFile = SD.open(LOG_FILE,FILE_WRITE);
      if (logFile) {                                            // If we were able to open the log file..
        if (logFile.size()==0) {                                // Check the size to see if this is a new file.
          aHeader.numLines = 0;                                 // Zero out a header..
          aHeader.numWetLines = 0;
          writeHeader(&aHeader);
          logFile.print("Run #");       logFile.print('\t');    // New file's get a header line in human.
          logFile.print("Mode");        logFile.print('\t');
          logFile.print("Limit %");     logFile.print('\t');
          logFile.print("Water s");  logFile.print('\t');
          logFile.print("Soak s");   logFile.print('\t');
          logFile.print("Raw Cap");    logFile.print('\t');
          logFile.print("Raw Tmp");   logFile.print('\t'); 
          logFile.print("Ave Cap");    logFile.print('\t');     
          logFile.print("Ave Tmp");   logFile.print('\t');
          logFile.print("Moisture");    logFile.println();
        }
        logFile.print(ourParamObj.getRunNum()); logFile.print('\t');   // Create the data line. Un-updated values are set as a blank char.
        if (modeSet) { logFile.print(mMode); } else { logFile.print(" "); } logFile.print('\t');
        if (limitSet) { logFile.print(mLimit); } else { logFile.print(" "); } logFile.print('\t');
        if (wTimeSet) { logFile.print(mWTime); } else { logFile.print(" "); } logFile.print('\t');
        if (sTimeSet) { logFile.print(mSTime); } else { logFile.print(" "); } logFile.print('\t');
        if (rCapSet) { logFile.print(mRCap); } else { logFile.print(" "); } logFile.print('\t');
        if (rTempSet) { logFile.print(mRTemp); } else { logFile.print(" "); } logFile.print('\t'); 
        if (aCapSet) { logFile.print(mACap); } else { logFile.print(" "); } logFile.print('\t');     
        if (aTempSet) { logFile.print(mATtemp); } else { logFile.print(" "); } logFile.print('\t');
        if (moistureSet) { logFile.print(mMoisture); } else { logFile.print(" "); } logFile.println();
        aHeader.numLines++;
        if (mMode==watering) { aHeader.numWetLines++; }
        writeHeader(&aHeader);
        logFile.close();          // Close the file. (Always leave files closed!)
        clearFlags();             // Clear out the updating flags.
        return;                   // That's everything, lets go.
      }
    }                   // Getting here means there's troubles.
    setLogging(false);  // Shut down logging.
    mHaveSD = false;    // Can't open the file? Hardware error.
  }                                                         
}


// Alll these are pretty much the same. Set a value, set the flag that its been set.         
void dataLog::addMode(int mode) { mMode = mode; modeSet = true; }
void dataLog::addLimit(int limit) { mLimit = limit; limitSet = true; }
void dataLog::addWaterSec(int wTime) { mWTime = wTime; wTimeSet = true; }
void dataLog::addSoakSec(int sTime) { mSTime = sTime; sTimeSet = true; }
              
void dataLog::addRawCap(uint16_t rCap) { mRCap = rCap; rCapSet = true; }
void dataLog::addRawTemp(float rTemp) { mRTemp = rTemp; rTempSet = true; }
void dataLog::addAveCap(uint16_t aCap) { mACap = aCap; aCapSet = true; }
void dataLog::addAveTemp(float aTemp) { mATtemp = aTemp; aTempSet = true; }
void dataLog::addMoisture(float moisture) { mMoisture = moisture; moistureSet = true; }


// And, like it says, this clears all the "I been set" flags in one mighty swoop.
void dataLog::clearFlags(void) {

  modeSet     = false;
  limitSet    = false;
  wTimeSet    = false;
  sTimeSet    = false;
            
  rCapSet     = false;
  rTempSet    = false;
  aCapSet     = false;
  aTempSet    = false;
  moistureSet = false;
}

// Print the log file to the monitor.
void dataLog::showLogfile(void) {

  if (mHaveSD) {
    logFile = SD.open(LOG_FILE, FILE_READ);                    // Attempt opening the log file.
      if (logFile) {
        if (logFile.size()==0) {                                  // Check the size to see if this is a new file.
          Serial.println("Nothing to show.");
        } else {
          unsigned long i = sizeof(logHeader);
          while(i<logFile.size()) {
            logFile.seek(i);
            Serial.print((char)logFile.read());
            i++;
          }
        }
        logFile.close();
      } else {
        Serial.println("Not able to open the file.");
      }
  } else {
    Serial.println("Not seeig any SD drive.");
  }
}


// Print the log file size to the monitor.
void dataLog::showLogLines(void) {

  logHeader aHeader;
  int count;
  if (mHaveSD) {
    logFile = SD.open(LOG_FILE, FILE_READ);                    // Attempt opening the log file.
      if (logFile) {
        readHeader(&aHeader);
        logFile.close();
        count = aHeader.numLines;
        Serial.print("total lines : ");Serial.println(count);
      } else {
        Serial.println("Can't open log file.");
      }
  } else {
    Serial.println("Not seeig any SD drive.");
  }
}


// Return the number of bytes in the logFile. Zero on errors.
unsigned long dataLog::getFileSize(void) {

  unsigned long fileSize;
  
  if (mHaveSD) {
    logFile = SD.open(LOG_FILE, FILE_READ);  // Attempt opening the log file.
    if (logFile) {
        fileSize = logFile.size();
        logFile.close();
        return fileSize;
    }
  }
  return 0;
}


unsigned long dataLog::getFileNumLines(void) {
  
  logHeader aHeader;

  aHeader.numLines = 0;                  // Just in case..
  logFile = SD.open(LOG_FILE, FILE_READ); // Attempt opening the log file.
  if (logFile) {
    readHeader(&aHeader);
    logFile.close();
  }
  return aHeader.numLines;
}


unsigned long dataLog::getFileNumWLines(void) {
  
  logHeader aHeader;

  aHeader.numWetLines = 0;                // Just in case..
  logFile = SD.open(LOG_FILE, FILE_READ); // Attempt opening the log file.
  if (logFile) {
    readHeader(&aHeader);
    logFile.close();
  }
  return aHeader.numWetLines;
}
 
// Here's a buffer of buffBytes size. Fill it as far as you can starting at index.
unsigned long dataLog::getFileBuff(unsigned long index,unsigned long buffBytes,byte* buff) {

  unsigned long numBytes;

  numBytes = 0;
  if (mHaveSD) {
    logFile = SD.open(LOG_FILE, FILE_READ);  // Attempt opening the log file.
    if (logFile) {
      while(index<logFile.size()&&numBytes<buffBytes) {
        buff[index] = logFile.read();
        index++;
        numBytes++;
      }
      logFile.close();
      return numBytes;
    }
  }
  return 0;
}
 
