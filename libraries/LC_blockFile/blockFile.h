#ifndef blockFile_h
#define blockFile_h

#include <SD.h>

// blockFile.. This is an object that will manage a file on an SD card much like dynamic
// memory. You ask for a file block ID. From that you can store, retrieve, delete or
// resize your block of data, as your whim takes you. Files can have pretty near an
// unlimited number of numbered blocks.
//
// Now, when you start up your program, the big problem is... In what block is anything
// stored? There are no guarantees as to the order of the block ID allocation. What to do?
// Here is how it this problem is addressed. The first block allocated is known as the
// "root" block ID. You use this file block to hold whatever information you need to
// decode the rest of your file.
//
// Something to remember. You can only read and write complete blocks. Therefore, make
// them small enough that this won't be an issue. You can have as many as you want, so
// don't design your code around big multi use blocks. Break things up in smaller single
// use blocks.


#define BLOCKFILE_TAG             "BLOCKFILE"
#define BLOCKFILE_TAG_SIZE        12
#define CURRENT_BLOCKFILE_VERSION 1
#define INITIAL_BLOCKFILE_ID      1

#define BF_NO_ERR         0   // Everything's fine now, ain't it?
#define BF_MEM_ERR        1   // malloc() failed.
#define BF_VERSION_ERR    2   // We have the right kind of file. But, wrong version.
#define BF_FOPEN_ERR      3   // Tried to open from a file path but failed.
#define BF_FREAD_ERR      4   // Tried to read a buffer but failed.
#define BF_FWRITE_ERR     5   // Tried to write a buffer but failed.
#define BF_ISDIR_ERR      6   // Looking for a file, was handed path to directory.
#define BF_SEEK_ERR       7   // Trying to reposition the file pointer failed.


struct blockFileHeader {
  char          nameTag[BLOCKFILE_TAG_SIZE];  // Text of the file tag.
  unsigned long versionNum;                   // Version number. Later code may want to update this.
  unsigned long rootID;								 // The first user block of data. Can be used as key for the file.
  unsigned long nextID;                       // When creating data blocks, this is the next block ID to be handed out.
};


struct blockHeader {
  unsigned long blockID;  // Set to zero as free block flag.
  unsigned long bytes;    // Set to zero as EOF marker.
};


class blockFile {

  public:
          blockFile(char* inFilePath);
  virtual ~blockFile(void);

			  unsigned long   readRootBlockID(void);                        // HINT : You store the information you need to decode your file in here.
			  unsigned long   getNewBlockID(void);                          // The first blockID issued to you, will be saved as your initial block.
			  unsigned long   addBlock(char* buffPtr,unsigned long bytes);  // That can come from  getNewBlockID() or addBlock().
			  bool            deleteBlock(unsigned long blockID);
  
			  bool            writeBlock(unsigned long blockID,char* buffPtr,unsigned long bytes);  // I have my ID, put this buffer in there.
			  unsigned long   getBlockSize(unsigned long blockID);                                  // How much did I store in there? I forget..
			  bool            getBlock(unsigned long blockID,char* buffPtr,unsigned long bytes);    // Here's my ID and a buffer, give me my data.
 
			  void            cleanup(unsigned long allowedMs);                                     // Not written yet.
			  void            deleteBlockfile(void);                                                // Mark file to be erased when object is deleted.
			  int             checkErr(bool clearErr=false);                                        // Lets see what the last error was?
			  bool            isEmpty(void);                                                        // No data buffers.
  
			  //protected :
  
			  void            printDataBlock(blockHeader* aBlock);	// For debug.
			  void				printFile(void);								// For debug.
			  bool            fOpen(void);
			  void            fClose(void);
			  bool            peekBlockHeader(blockHeader* aBlock);
			  bool            writeBlockHeader(unsigned long inBlockID,unsigned long numBytes);
			  bool            writeBlockData(char* buffPtr,unsigned long bytes);

			  bool            nextBlock(unsigned long numBytes);
			  bool            findFirst(void);
			  bool            findFit(unsigned long numBytes);
			  bool            findID(unsigned long blockID);
			  bool            findEnd(void);
  
			  void            initFileHeader(void);
			  bool            writeFileHeader(void);
			  bool            getFileHeader(void);
			  void            initBlockfile(void);
  
			  blockFileHeader mHeader;
			  char*           mFilePath;
			  File            mFile;
			  bool            mFileReady;
			  bool            mDelete;
			  int             mErr;
};

#endif
