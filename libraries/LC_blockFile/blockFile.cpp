#include "blockFile.h"


blockFile::blockFile(char* inFilePath) {
  //tracer trace("blockFile()");

  mFileReady = false;                                 // Is the file ready? Well, not yet.
  mDelete = false;                                    // We want to delete this file? Not yet.
  mErr = BF_NO_ERR;                                   // No errors yet..
  mFilePath = (char*) malloc(strlen(inFilePath) + 1); // Try to save off file path.
  if (mFilePath) {                                    // If we got the memory..
    strcpy(mFilePath, inFilePath);                    // Lets use it.
    initBlockfile();                                  // See if we can open and use this file.
  } else {                                            // Couldn't get the memory?
    mErr = BF_MEM_ERR;                                // Flag a memory error.
  }
}


blockFile::~blockFile(void) {

  if (mFilePath) {                  // We have non null path pointer?
    if (mFileReady) {               // We have an open file?
      mFile.close();                // Close the file.
      if (mDelete && mFileReady) {  // Has the file been marked for deletion?
        SD.remove(mFilePath);       // This is where we tell the system to delete the file.
      }
    }
    free(mFilePath);                // Release the memory for the path string.
  }
}


// Hand back the ID of your first block of data. When designing your
// project, use this block to store the info you need to decode the rest.
// return of 0 means that this is an empty file.
unsigned long blockFile::readRootBlockID(void) {
//  tracer trace("readInitialBlockID()", &mErr);

  return mHeader.rootID;
}


// This is for pre-assigning new data block IDs.
// Sometimes you need them before you can store them.
unsigned long  blockFile::getNewBlockID(void) {
//  tracer trace("getNewBlockID()", &mErr);

  unsigned long ID;

  ID = mHeader.nextID;        // Save off the ID to use.
  if (!mHeader.rootID) {   // If this is the first buffer handed out..
    mHeader.rootID = ID;   // Save its ID. This'll be the one used to decode this mess.
  }
  mHeader.nextID++;           // Increment it.
  if (fOpen()) {
    writeFileHeader();          // Save it.
    fClose();
  }
  return ID;                  // We already have it so ignore any errors.
}


// Create a new file block using this buffer.
// Passing back a zero means there was an error.
unsigned long blockFile::addBlock(char* buffPtr, unsigned long bytes) {
//  tracer trace("addBlock()", &mErr);

  unsigned long ID;

  ID = getNewBlockID();                   // Grab an ID to use.
  return writeBlock(ID, buffPtr, bytes);  // Try to write it out. Return how it worked.
}


// Delete this numbered fileblock from our file.
bool blockFile::deleteBlock(unsigned long blockID) {
//  tracer trace("deleteBlock()", &mErr);

  blockHeader   tempBlock;

  if (fOpen()) {
    if (blockID != mHeader.rootID) {                    // You do NOT want to delete your initial block.
      if (findID(blockID)) {                            // Find this block in datafile.
        if (peekBlockHeader(&tempBlock)) {              // Get a copy of the header.
          if (writeBlockHeader(0, tempBlock.bytes)) {   // Change the header to to be a free block.
            return true;                                // Found it, read it, freed it, success!
          }
        }
      }
    }
  }
  fClose();
  return false;                                       // Broken, I guess.
}


// Update or create this numbered file block to match this buffer.
bool blockFile::writeBlock(unsigned long blockID, char* buffPtr, unsigned long bytes) {
//  tracer trace("writeBlock()", &mErr);

  blockHeader   tempBlock;
  unsigned long remainigBytes;
  
  if (fOpen()) {
    if (findID(blockID)) {                                                        // Find this block in datafile.
      if (peekBlockHeader(&tempBlock)) {                                          // Get a copy of the header.
        if (tempBlock.bytes == bytes) {                                           // Same size?
          if (writeBlockHeader(tempBlock.blockID, tempBlock.bytes)) {             // Moves file pointer to the data.
            if (writeBlockData(buffPtr, bytes)) {                                 // Write out the new data buffer.
              fClose();
              return true;                                                        // We are done, bail out.
            }
          }
        } else {                                                                  // Buffer size changed. Oh, ohh..
          if (writeBlockHeader(0, tempBlock.bytes)) {                             // Free the original.
            if (findFit(bytes)) {                                                 // Start over, find an empty slot.
              if (peekBlockHeader(&tempBlock)) {                                  // Get a copy of the header we have.
                if (writeBlockHeader(blockID, bytes)) {                           // Change the header to reflect what were writing.
                  if (writeBlockData(buffPtr, bytes)) {                           // Write out the new data buffer.
                    remainigBytes = tempBlock.bytes - (bytes + sizeof(blockHeader)); // Calculate what's left of the free block.
                    if (writeBlockHeader(0, remainigBytes)) {                     // Add blockHeader updating the new free size.
                      fClose();
                      return true;                                                // All done, lets go get coffee.
                    }
                  }
                }
              }
            }
          }
        }
      }
    } else if (findEnd()) {                                               // Couldn't find an empty slot, find the end.
      if (writeBlockHeader(blockID, bytes)) {                             // Change the header to reflect what were writing.
        if (writeBlockData(buffPtr, bytes)) {                             // Write out the new data buffer.
          if (writeBlockHeader(0, 0)) {                                   // Add new EOF blockHeader.
            fClose();
            return true;                                                  // Success!.
          }
        }
      }
    }
  }
  fClose();
  return false;                                       // Donno', something went wrong.
}


// Write these bytes into this exsisting fileblock.
// Can not overwrite the end of the fileblock.
//bool blockFile::writeSubBlock(unsigned long blockID, unsigned long stIndex, char* buffPtr, unsigned long bytes) { return false; }



// Returns the size of the file block.
// Retruns zero on error.
unsigned long  blockFile::getBlockSize(unsigned long blockID) {
//  tracer trace("getBlockSize()", &mErr);

  blockHeader   tempBlock;
  if (fOpen()) {
    if (findID(blockID)) {                              // Find this block in datafile.
      if (peekBlockHeader(&tempBlock)) {                // Get a copy of the header.
        fClose();
        return tempBlock.bytes;                         // And here's the size..
      }
    }
    fClose();
  }
  return 0;
}


// Fill buffer with contents of file block.
// Buffer can be smaller, but you'll get the end bytes truncated.
bool blockFile::getBlock(unsigned long blockID, char* buffPtr, unsigned long bytes) {
//  tracer trace("getBlock()", &mErr);

  blockHeader   tempBlock;
  if (fOpen()) {
    if (findID(blockID)) {                                        // Find this block in datafile.
      if (peekBlockHeader(&tempBlock)) {                          // Get a copy of the header.
        if (tempBlock.bytes < bytes) {                            // And here's the size..
          bytes = tempBlock.bytes;                                // Set our limit.
        }
        if (mFile.seek(mFile.position() + sizeof(blockHeader))) { // Position to start of data.
          if (mFile.read(buffPtr, bytes)) {                       // Read it out.
            fClose();
            return true;                                          // If we read it, we're good. Lets go.
          } else {                                                // Something btoke in read().
            mErr = BF_FREAD_ERR;                                  // Flag the error.
          }
        } else {                                                  // Something broke in seek().
          mErr = BF_SEEK_ERR;                                     // Flag the error.
        }
      }
    }
  }
  fClose();
  return false;                                                 // And here's the sad exit..
}


// Allocate time for maintenance tasks.
void blockFile::cleanup(unsigned long allowedMs) {
//  tracer trace("cleanup()", &mErr);

}


// Set the delete me flag. File will be killed off in destructor.
void blockFile::deleteBlockfile(void) {
  mDelete = true;
}


// Pass back the last error. Clear it if you want.
int blockFile::checkErr(bool clearErr) {
//  tracer trace("checkErr()", &mErr);

  int lastErr;

  lastErr = mErr;
  if (clearErr) mErr = BF_NO_ERR;
  return lastErr;
}


bool blockFile::isEmpty(void) {
//  tracer trace("isEmpty()", &mErr);

  blockHeader   tempBlock;
  
  if (fOpen()) {
    if (findFirst()) {
      if (peekBlockHeader(&tempBlock)) {
        fClose();
        return tempBlock.bytes == 0;
      }
    }
  }
  fClose();
  return true;
}


// *************** internal stuff..  ***************

// Have a look at a daatablock header.
void blockFile::printDataBlock(blockHeader* aBlock) {

    Serial.println("---------------------------");
    Serial.println("DATA HEADER");
    Serial.print("Block size : ");Serial.println(sizeof(blockHeader));
    Serial.print("  Block ID : ");Serial.println(aBlock->blockID);
    Serial.print("Buff bytes : ");Serial.println(aBlock->bytes);
    Serial.println("---------------------------");
}


// We need to keep better track of our datafile;
// This will be the standard opening.
bool blockFile::fOpen(void) {

  mFile = SD.open(mFilePath, FILE_WRITE);
  if (!mFile) {
    mErr = BF_FOPEN_ERR;
    return false;
  }
  return true;
}


// And the standard closing. When in doubt, keep 'em closed!
void blockFile::fClose(void) {

  if (mFile) {
    mFile.close();
  }
}


// mFile is pointing at a data block. This reads and returns the block header.
// Then, resets mFile to the beginning of that block.
bool  blockFile::peekBlockHeader(blockHeader* aBlock) {
//  tracer trace("peekBlockHeader()", &mErr);

  unsigned long bytesRead;
  blockHeader   tempBlock;
  bool          success;

  success = false;
  if (mErr == BF_NO_ERR) {                                            // Don't bother with broken file.
    bytesRead = mFile.read((char*)&tempBlock, sizeof(blockHeader));   // Make a grab at the block.
    if (bytesRead == sizeof(blockHeader)) {                           // See if we got a whole header.
      if (mFile.seek(mFile.position() - sizeof(blockHeader))) {       // Return pointer to beginning of block.
        aBlock->blockID = tempBlock.blockID;                          // Save off the bits.
        aBlock->bytes = tempBlock.bytes;
        success = true;                                               // Read it and we're still pointing at it.
      } else {
        mErr = BF_SEEK_ERR;
      }
    } else {
      mErr = BF_FREAD_ERR;
    }
  }
  return success;
}


// mFile is pointing at a data block header. Or at least where we want to put one.
// Stamp this data block into the file here.
bool blockFile::writeBlockHeader(unsigned long inBlockID, unsigned long numBytes) {
  //tracer trace("writeBlockHeader()", &mErr);

  blockHeader   tempBlock;
  unsigned long bytesWritten;

  if (mErr == BF_NO_ERR) {                                                // Ok, don't bother if its already broke.
    tempBlock.blockID = inBlockID;                                        // To make the code easier, we write it into a block.
    tempBlock.bytes = numBytes;                                           // This makes it just one big write out.
    bytesWritten = mFile.write((char*)&tempBlock, sizeof(blockHeader));   // Write the bytes.
    if (bytesWritten == sizeof(blockHeader)) {                            // All the bytes get out?
      return true;                                                        // Everything seems fine.
    } else {
      mErr = BF_FWRITE_ERR;                                               // Something broke doring the write.
    }
  }
  return false;                                                           // Tell the world.
}


// We are pointing at where we want to save this data buffer. Write it here and
// pass back if it worked. Leave the poiner at the end of the saved data.
bool blockFile::writeBlockData(char* buffPtr, unsigned long bytes) {
//  tracer trace("writeBlockData()", &mErr);

  unsigned long bytesWritten;

  if (mErr == BF_NO_ERR) {                        // Don't bother with broken file.
    bytesWritten = mFile.write(buffPtr, bytes);   // Write out the new data buffer.
    if (bytesWritten == bytes) {                  // Check to see if it was stored.
      mFile.flush();                              // Make sure they're out there.
      return true;                                // We are done, bail out.
    }
  }
  return false;                                   // Or not..
}



// mFile is pointing at a data block. We've already looked at it.
// Here's the number of bytes read from this block. Jump to the next one.
bool  blockFile::nextBlock(unsigned long numBytes) {
//  tracer trace("nextBlock()", &mErr);
  /*
    Serial.print("numBytes   : "); Serial.println(numBytes);
    Serial.print("file index : "); Serial.println(mFile.position());
    Serial.println("---------------");
  */
  unsigned long newIndex;
  bool  success;

  success = false;                                                  // Work for your successes!
  if (mErr == BF_NO_ERR && numBytes > 0) {                          // Sanity sanity!
    newIndex = mFile.position() + sizeof(blockHeader) + numBytes;   // Calculate our absolute position.
    //Serial.print("trying for index : ");Serial.println(newIndex);
    //Serial.print("File size        : ");Serial.println(mFile.size());
    if (mFile.seek(newIndex)) {                                     // Do the jump. If it works..
      success = true;                                               // Its a success!
    } else {                                                        // If it don't work..
      mErr = BF_SEEK_ERR;                                           // Thow the error.
    }
  }
  return success;                                                   // However it turned out. tell the world.
}


// Set the file pointer to the location after the file header.
bool  blockFile::findFirst(void) {
//  tracer trace("findFirst()", &mErr);

  if (mErr == BF_NO_ERR) {                        // As long aas we ain't busted already.
    if (mFile.seek(sizeof(blockFileHeader))) {    // Easy, its the location right after the header.
      return true;                                // If the jump worked, were golden.
    } else {                                      // If the jump didn't work?
      mErr = BF_SEEK_ERR;                         // Throw an error.
    }
  }
  return false;                                   // Sorry boys, didnt work out.
}


// Find a "hole" in the list big enough to put in this byte amount.
// Return false if it does not find one, true if pointing at one.
// False may or may not mean an error. Better check that.
bool  blockFile::findFit(unsigned long numBytes) {
//  tracer trace("findFit()", &mErr);

  blockHeader   tempBlock;    // A place to read blocks into.
  unsigned long minBuff;      // Enough room to split a free block.

  if (mErr == BF_NO_ERR && findFirst()) {                                     // Reset file pointer. Don't bother if its broken.
    minBuff = numBytes + sizeof(blockHeader) + 1;                             // We need enuogh to put in new header & some data.
    while (true) {                                                            // Start looping.
      if (peekBlockHeader(&tempBlock)) {                                      // If we get a header.
        if (tempBlock.bytes > 0) {                                            // Bytes == 0 is EOF marker.
          if (tempBlock.blockID == 0) {                                       // BlockID == 0 is a free block.
            if (tempBlock.bytes == numBytes || tempBlock.bytes >= minBuff) {  // This free block fit?
              return true;                                                    // Found one and we're pointing at it.
            }
          }
        } else {                                                              // Oh dear, so many holes to drop out of.
          return false;
        }
      } else {
        return false;
      }
      if (!nextBlock(tempBlock.bytes)) {
        return false;
      }
    }
  }
  return false;
}


// Looking for something? This guy will point you to that ID.
// If its in there.
bool  blockFile::findID(unsigned long blockID) {
//  tracer trace("findID()", &mErr);

  blockHeader   tempBlock;    // A place to read blocks into.
  bool          done;         // Used to tell when we can stop lookin'.
  bool          success;      // Did this work or not?

  success = false;                                                    // Can't succeed unless you try.
  if (mErr == BF_NO_ERR && findFirst()) {                             // Reset file pointer. Don't bother if its broken.
    done = false;                                                     // Set up for looping.
    while (!done) {                                                   // Start looping.
      if (peekBlockHeader(&tempBlock)) {                              // Lets have a look at the header.
        if (tempBlock.bytes == 0) {                                   // CHECK FIRST! Bytes == 0 is EOF marker.
          done = true;                                                // And that's it, we're done.
        } else if (tempBlock.blockID == blockID) {                    // Not EOF, check ID.
          success = true;                                             // Found it!
          done = true;                                                // And we're done.
        }
      } else {                                                        // Couldn't peek at the block?
        done = true;                                                  // Jump out on errors.
      }
      if (!done) {                                                    // If we're still running..
        done = !nextBlock(tempBlock.bytes);                           // Try for the next block.
      }
    }
  }
  return success;
}


// Want to add data? This'll point you at the EOF header.
bool  blockFile::findEnd(void) {
//  tracer trace("findEnd()", &mErr);

  blockHeader   tempBlock;								// A place to read blocks into.

  if (mErr == BF_NO_ERR && findFirst()) {       // Reset file pointer. Don't bother if its broken.
    while (true) {                              // Start looping.
      if (peekBlockHeader(&tempBlock)) {        // See if we can have a look at the header.
        if (tempBlock.bytes == 0) {             // Bytes == 0 is EOF marker.
          return true;                          // Found it!
        }
      } else {                                  // Couldn't peek at the block?
        return false;                           // Jump out on errors.
      }
      if (!nextBlock(tempBlock.bytes)) {        // Try for the next block.
        return false;
      }
    }
  }
  return false;											// I *think* this is best err out. 12/30/2018
}


// Set our local header to initial condition of a new file.
void blockFile::initFileHeader(void) {
//  tracer trace("initFileHeader()", &mErr);

  strcpy((char*)mHeader.nameTag, BLOCKFILE_TAG);         // Init the header.
  mHeader.versionNum = CURRENT_BLOCKFILE_VERSION; // Set the version number.
  mHeader.rootID = 0;                             // Zero means not set yet.
  mHeader.nextID = INITIAL_BLOCKFILE_ID;          // Set the next ID value.
}


// Write the current header out to the file.
bool blockFile::writeFileHeader(void) {
//  tracer trace("writeFileHeader()", &mErr);

  int bytesWritten;

  if (mErr == BF_NO_ERR) {                                                      // Don't bother if its broken.
    if (mFile.seek(0)) {                                                      // Set to the start of the file.
      bytesWritten = mFile.write((char*)&mHeader, sizeof(blockFileHeader));   // Write the bytes here.
      if (bytesWritten == sizeof(blockFileHeader)) {                          // All the bytes get out?
        return true;                                                          // Everything seems fine.
      } else {
        mErr = BF_FWRITE_ERR;                                                 // Something broke doring the write.
      }
    } else {
      mErr = BF_SEEK_ERR;                                                     // Seek() failed for some reason.
    }
  }
  return false;
}


// Refresh the current header from the saved header of the file.
bool blockFile::getFileHeader(void) {
//  tracer trace("getFileHeader()", &mErr);

  blockFileHeader tempHeader;
  int             bytesread;
  bool            success;

  success = false;                                                    // Can't succeed unless you try.
  if (mErr == BF_NO_ERR) {                                            // Don't bother if its broken.
    if (mFile.seek(0)) {                                              // Set to the start of the file.
      if (mFile.available() >= sizeof(blockFileHeader)) {             // We have enough bytes to read..
        bytesread = mFile.read(&tempHeader, sizeof(blockFileHeader)); // Make a grab for it.
        if (bytesread == sizeof(blockFileHeader)) {                   // Make sure we got the complete set.
          if (!strcmp(tempHeader.nameTag, BLOCKFILE_TAG)) {           // We get a matching nametag?
            if (tempHeader.versionNum == CURRENT_BLOCKFILE_VERSION) { // Is the version number ok?
              strcpy(mHeader.nameTag, tempHeader.nameTag);           // Everything seems ok, copy the data.
              mHeader.versionNum = tempHeader.versionNum;
              mHeader.rootID = tempHeader.rootID;
              mHeader.nextID = tempHeader.nextID;
              success = true;                                         // And we're a success.
            } else {
              mErr = BF_VERSION_ERR;
            }
          }                                                           // No real error, just not our filetype.
        } else {
          mErr = BF_FREAD_ERR;
        }
      }                                                               // Again, no real error. Just not enough bytes.
    } else {
      mErr = BF_SEEK_ERR;
    }
  }
  return success;
}


// We have unknown filepath. See what we have and if we can use it.
// When complete, mFileReady tells us we have a valid and open mFile.
// If mFileReady is false, we have no file handle to dispose of.
void blockFile::initBlockfile(void) {
//  tracer trace("initBlockfile()", &mErr);

  mFileReady = false;                                 // Ain't ready yet.
  if (mErr == BF_NO_ERR) {                            // Currently no unread error?
    if (fOpen()) {                                    // We got a file ref.
      if (!mFile.isDirectory()) {                     // Not a directory.
        if (mFile.size() == 0) {                      // We have an empty file.
          initFileHeader();                           // Set up header for new file.
          if (writeFileHeader()) {                    // Blast it in there.
            mFileReady = writeBlockHeader(0, 0);      // End of data marker.
          }                                           // Write methods set the errors for us here.
        } else {                                      // We have data.
          mFileReady = getFileHeader();               // See if we have valid header.
          if (!mFileReady) {                          // Have file with bad header?
            if (mErr != BF_VERSION_ERR) {             // if its NOT a version error.
              checkErr(true);                         // We overwrite. Clear the error.
              mFile.close();                          // Close the file.
              SD.remove(mFilePath);                   // Delete the file.
              mFile = SD.open(mFilePath);             // Try re-opening the file.
              if (mFile) {                            // Sanity.
                initFileHeader();                     // Set up header for new file.
                if (writeFileHeader()) {              // Blast it in there.
                  mFileReady = writeBlockHeader(0, 0); // End of data marker.
                }                                     // Again, write methods set the errors for us here.
              } else {
                mErr = BF_FOPEN_ERR;                  // Couldn't open the file.
              }
            }
          }
        }
      } else {
        mErr = BF_ISDIR_ERR;                          // Tried to open a directory.
      }
    } else {
      mErr = BF_FOPEN_ERR;                            // Can't open the file.
    }
  }
  fClose();
}
