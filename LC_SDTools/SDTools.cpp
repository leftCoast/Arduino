#include "SDTools.h>
#include <resizeBuff.h>

char  nameBuff[20];  // Use it as you please. Its shared between all.


// Groups like Apple will fill the SD drive with all sorts of junk we do't need
// to see. This function's job is to filter it out. As best as it can.
bool filterFile(char* fileName) {

   if (!strstr(fileName,"~1."))  return false;  // No Apple clones.
   if (fileName[0]=='.')         return false;  // No invisables.
   return true;
}



// *******************************************************
//                      dirItem class
// *******************************************************


dirItem::dirItem(File inItem)
   : linkListObj() {
      
   int numBytes;
   
   mName = NULL;                          // Pointers start at NULL here.
   strcpy(nameBuff,inItem.name());        // Grab the name to work with.
   numBytes = strlen(nameBuff)+1;         // See how long it is..
   if (resizeBuff(numBytes,&mName)) {     // If we can get RAM to store the name in..
      strcpy(mName,nameBuff);             // Stuff in the name.
   }
   mIsDir = inItem.isDirectory();         // Save off if its a directory or not.
   if (mIsDir) {                          // If it is a diectory..
      mNumBytes = 0;                      // It has zero bytes.
   } else {                               // Else, its not a directory..
      mNumBytes = inItem.size();          // Save off the file size.
   }
}

  
dirItem::~dirItem(void) { resizeBuff(0,&mName); }

   
char* dirItem::getName(void) { return mName; }


unsigned long dirItem::getSize(void) { return mNumBytes; }


bool* dirItem::getIsDir(void) { return mIsDir; }



// *******************************************************
//                      dirList class
// *******************************************************


dirList::dirList(void)
   : linkList() { 
   
   mLastIndex = -1;
   mLastItem = NULL; 
}
   
   
dirList::~dirList(void) {  }
   
  
void dirList::readDir(char* path) {

   File     dir;                                // File handle used for the current directory.
   File     entry;                              // File handle used for the different entries of the current directory.
   dirItem* newItem;                            // Its what we create.
   bool     done;                               // A boolean used to track when to stop all this nonsense.

   dumpList();                                  // Clear the list for a new set.
   mLastIndex = -1;                             // Flag for no search yet.
   mLastItem = NULL;                            // Flag nothing found yet.
   dir = SD.open(path);                         // Try opening the directory.
   if (dir) {                                   // If we were able to open the working directory..
      dir.rewindDirectory();                    // Rewind it to the first entry.
      done = false;                             // We ain't done yet.
      do {                                      // Start looping through the entries.
         entry = dir.openNextFile();            // Grab an entry.
         if (entry) {                           // If we got an entry..
            strcpy(fileName,entry.name());      // Grab the name.
            if (filterFile(fileName)) {         // filter out Apple nonsense.
               newItem = new dirItem(entry);    // Create the list item.
               addToTop(newItem);               // Pop it in the list.
               entry.close();                   // And we close the entry.
            }
         } else {                               // Else, we didn't get an entry from above.
            done = true;                        // No entry means, we are done here.
         }
      } while (!done);                          // And we do this loop, over and over, while we are not done.
      dir.close();                              // Looping through entries is done, close up the original file.
   } else {                                     // If this worked correctly, we'd know there was an error at this point.                                        
      Serial.println("Fail to open file.");     // Sadly, instead of returning a NULL, it just crashes.
   }
}


dirItem* findItem(int index) {

   dirItem* anItem;
   
   if (index == mLastIndex) {
      anItem = mLastItem; 
   } else {
      anItem = getByIndex(index);
      mLastIndex = index;
      mLastItem = anItem;
   }
   return anItem;
}


char* dirList::getItemName(int index) {

   dirItem* anItem;
   
   anItem = findItem(index);
   if (anItem) {
      return anItem->getName();
   }
   return NULL;
}


unsigned long dirList::getItemSize(int index) {

   dirItem* anItem;
   
   anItem = findItem(index);
   if (anItem) {
      return anItem->getName();
   }
   return NULL;
}


bool dirList::getItemIsDir(int index) {

   dirItem* anItem;
   
   anItem = findItem(index);
   if (anItem) {
      return anItem->getIsDir();
   }
   return false;
}
