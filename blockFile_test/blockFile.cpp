#include "blockFile.h"

/*
  struct blockFileHeader {
  char          nameTag[BLOCKFILE_TAG_SIZE] = BLOCKFILE_TAG;
  unsigned long versionNum = CURRENT_BLOCKFILE_VERSION;
  unsigned long numBlocks = 0;
  };
*/

blockFile::blockFile(char* inFilePath) {

  mFileReady = false;                                 // Is the file ready? Well, not yet.
  mDelete = false;                                    // We want to delete this file? Not yet.
  mErr = FB_NO_ERR;                                   // No errors yet..
  mFilePath = (char*) malloc(strlen(inFilePath + 1)); // Try to save off file path.
  if (mFilePath) {                                    // If we got the memory..
    initBlockfile();                                  // See if we can open and use this file.
  } else {                                            // Couldn't get the memory?
    mErr = FB_MEM_ERR;                                  // Flag a memory error.
  }
}


blockFile::~blockFile(void) {

  if (mFilePath) {                  // We have non null path pointer?
    if (mFileReady) {               // We have an open file?
      mFile.close();                // Close the file.
      if (mDelete&&mFileReady) {    // Has the file been marked for deletion?
        SD.remove(mFilePath);       // This is where we tell the system to delete the file.
      }
    }
    free(mFilePath);                // Release the memory for the path string.
  }
}


// Create a new file block using this buffer.
// Passing back a zero means there was an error.
unsigned long blockFile::addBlock(char* buffPtr, unsigned long bytes) {


}


// Delete this numbered fileblock from our file.
bool blockFile::deleteBlock(unsigned long blockID) {


}


// Update this numbered file block to match this buffer.
bool blockFile::writeBlock(unsigned long blockID, char* buffPtr, unsigned long bytes) {


}


// Write these bytes into this exsisting fileblock.
// Can not overwrite the end of the fileblock.
bool blockFile::writeSubBlock(unsigned long blockID, unsigned long stIndex, char* buffPtr, unsigned long bytes) {


}


// Returns the size of the file block.
// Retruns zero on error.
bool blockFile::getBlockSize(unsigned long blockID) {


}


// Fill buffer with contents of file block.
// Buffer must be large enough to fit entire file block.
bool blockFile::getBlock(unsigned long blockID, char* buffPtr, unsigned long bytes) {


}

// Fill buffer with a subset of the contents of a file block.
// Starting at stIndex.
bool blockFile::getSubBlock(unsigned long blockID, unsigned long stIndex, char* buffPtr, unsigned long bytes) {


}


// Allocate time for maintenance tasks.
void blockFile::cleanup(unsigned long allowedMs) {


}


// Set our local header to initial condition of a new file.
void blockFile::initHeader(void) {

  strcpy(mHeader.nameTag, BLOCKFILE_TAG);         // Init the header.
  mHeader.versionNum = CURRENT_BLOCKFILE_VERSION; // Set the version number.
  mHeader.numBlocks = 0;                          // No blocks.
}


// You have a file ref. Use it to see if this is a valid file or not.
bool blockFile::checkHeader() {

  
}


// Write the current header out to the file.
bool blockFile::writeHeader(void) {


}


// Refresh the current header from the saved header of the file.
bool blockFile::getHeader(void) {


}


// We have unknown filepath. See what we have and if we can use it.
// When complete, mFileReady tells us we have a valid and open mFile.
// If mFileReady is false, we have no file handle to dispose of.
void blockFile::initBlockfile(void) {

  if (!mErr) {                                  // Currently no unread error.
    mFile = SD.open(mFilePath);                 // Try opening the file.
    if (mFile) {                                // We got a file ref.
      if (!mFile.isDirectory()) {               // Not a directory.
        if (mFile.size() == 0) {                // We have an empty file.
          initHeader();                         // Set up hearder for new file.
          mFileReady = writeHeader();           // Blast it in there. Success is success.
        } else {                                // We have data.
          mFileReady = checkHeader();           // See if we have valid header.
          if (!mFileReady) {                    // Have file with bad header?
            if (mErr != FB_VERSION_ERR) {       // if its NOT a version error.
              mErr = FB_NO_ERR;                 // We overwrite. Clear the error.
              mFile.close();                    // Close the file.
              SD.remove(mFilePath);             // Delete the file.
              mFile = SD.open(mFilePath);       // Try re-opening the file.
              if (mFile) {                      // Sanity.
                initHeader();                   // Set up header for new file.
                mFileReady = writeHeader();     // Blast it in there. Success is success.
              } else {
                mErr = FB_FOPEN_ERR;            // Couldn't open the file.
              }
            }
          }
        }
      } else {
        mErr = FB_ISDIR_ERR;                    // Tried to open a directory.
      }
    } else {
      mErr = FB_FOPEN_ERR;                      // Can't open the file.
    }
  }
}
