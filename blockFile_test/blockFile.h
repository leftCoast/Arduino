#ifndef blockFile_h
#define blockFile_h

#include <SD.h>

#define BLOCKFILE_TAG             "BLOCKFILE"
#define BLOCKFILE_TAG_SIZE        10
#define CURRENT_BLOCKFILE_VERSION 1

#define FB_NO_ERR         0
#define FB_MEM_ERR        1
#define FB_VERSION_ERR    2
#define FB_FOPEN_ERR      3
#define FB_ISDIR_ERR      4


struct blockFileHeader {
  char          nameTag[BLOCKFILE_TAG_SIZE] = BLOCKFILE_TAG;
  unsigned long versionNum = CURRENT_BLOCKFILE_VERSION;
  unsigned long numBlocks = 0;
};


class blockFile {

  public:
          blockFile(char* inFilePath);
  virtual ~blockFile(void);

  unsigned long   addBlock(char* buffPtr,unsigned long bytes);
  bool            deleteBlock(unsigned long blockID);
  
  bool            writeBlock(unsigned long blockID,char* buffPtr,unsigned long bytes);
  bool            writeSubBlock(unsigned long blockID,unsigned long stIndex,char* buffPtr,unsigned long bytes);
  
  bool            getBlockSize(unsigned long blockID);
  bool            getBlock(unsigned long blockID,char* buffPtr,unsigned long bytes);
  bool            getSubBlock(unsigned long blockID,unsigned long stIndex, char* buffPtr,unsigned long bytes);  
 
  void            cleanup(unsigned long allowedMs);
  void            deleteBlockfile(void);
  int             clearErr(void);
    
protected :

  void            initHeader(void);
  bool            checkHeader();
  bool            getHeader(void);
  bool            writeHeader(void);
  void            initBlockfile(void);

  
  blockFileHeader mHeader;
  char*           mFilePath;
  File            mFile;
  bool            mFileReady;
  bool            mDelete;
  int             mErr;
};

#endif
