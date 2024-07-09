#include "SDFile.h"
#include <strTools.h>



SDFile::SDFile(const char* inName) {

  heapStr(&fName,inName);
  status = 
}



bool SDFile::openRoot(SDVol* volume) {

  if (isOpen()) {
    return false;
  }
  if (volume->fatType() == 16) {
    type_ = FAT_FILE_TYPE_ROOT16;
    firstCluster_ = 0;
    fileSize = 32 * volume->rootDirEntryCount();
  } else if (volume->fatType() == 32) {
    type_ = FAT_FILE_TYPE_ROOT32;
    firstCluster_ = vol->rootDirStart();
    if (!vol->chainSize(firstCluster_, &fileSize)) {
      return false;
    }
  } else {
    // volume is not initialized or FAT12
    return false;
  }
  vol_ = vol;
  // read only
  flags_ = O_READ;

  // set to start of file
  curCluster_ = 0;
  curPosition_ = 0;

  // root has no directory entry
  dirBlock_ = 0;
  dirIndex_ = 0;
  return true;
}
  
}


void SDFile::close(void) {

  
}
