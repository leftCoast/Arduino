#ifndef tracer_h
#define reacer.h

// OK, what the heck is a tracer?
// This is a debugging tool that outputs when its constructed and
// when its destructed. Used at the beginning of functions to tell
// you when they are called and when they go out of scope. Its like
// a stack crawl so you can get an idea where things are breaking.
//
//
// Here is a real life exmple of it being used.
/*

// This is for pre-assigning new data block IDs.
// Sometimes you need them before you can store them.
unsigned long  blockFile::getNewBlockID(void) {
  tracer trace("getNewBlockID()", &mErr);

  unsigned long ID;

  ID = mHeader.nextID;        // Save off the ID to use.
  if (!mHeader.initialID) {   // If this is the first buffer handed out..
    mHeader.initialID = ID;   // Save its ID. This'll be the one used to decode this mess.
  }
  mHeader.nextID++;           // Increment it.
  if (fOpen()) {
    writeFileHeader();          // Save it.
    fClose();
  }
  return ID;                  // We already have it so ignore any errors.
}

*/

 
class tracer {

  public:

  tracer(char* traceText,int* inErrPtr=NULL);
  ~tracer();

  char* mText;
  int*  errPtr;
};


#endif
