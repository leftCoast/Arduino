#ifndef blockFile_h
#define blockFile_h

#include <SD.h>

class tracer {

  public:

  tracer(char* traceText,int* inErrPtr=NULL);
  ~tracer();

  char* mText;
  int*  errPtr;
};


#define BLOCKFILE_TAG             "BLOCKFILE"
#define BLOCKFILE_TAG_SIZE        12
#define CURRENT_BLOCKFILE_VERSION 1
#define INITIAL_BLOCKFILE_ID      1

#define BF_NO_ERR         0   // Everything's fine now, ain't it?
#define BF_MEM_ERR        1   // malloc() failed.
#define BF_VERSION_ERR    2   // We have the right kind of file. But, wrong verion.
#define BF_FOPEN_ERR      3   // Tried to open from a file path but failed.
#define BF_FREAD_ERR      4   // Tried to read a buffer but failed.
#define BF_FWRITE_ERR     5   // Tried to read a buffer but failed.
#define BF_ISDIR_ERR      6   // Looking for a file, was handed path to directory.
#define BF_SEEK_ERR       7   // Trying to reposition the file pointer failed.


struct blockFileHeader {
  char          nameTag[BLOCKFILE_TAG_SIZE];  // Text of the file tag.
  unsigned long versionNum;                   // Version number. Later code may want to update this.
  unsigned long initialID;                    // The first user block of data. Can be used as key for the file.
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

  unsigned long   readInitialBlockID(void);                     // HINT : You store the information you need to decode your file in here.
  unsigned long   getNewBlockID(void);                          // The first blockID issued to you, will be saved as your initial block.
  unsigned long   addBlock(char* buffPtr,unsigned long bytes);  // Meaning from  getNewBlockID() or addBlock().
  bool            deleteBlock(unsigned long blockID);
  
  bool            writeBlock(unsigned long blockID,char* buffPtr,unsigned long bytes);
  unsigned long   getBlockSize(unsigned long blockID);
  bool            getBlock(unsigned long blockID,char* buffPtr,unsigned long bytes);
 
  void            cleanup(unsigned long allowedMs);
  void            deleteBlockfile(void);
  int             checkErr(bool clearErr=false);
  bool            isEmpty(void);
  
  protected :
  
  void            printDataBlock(blockHeader* aBlock);  // For debug.
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
