#ifndef IDnameBuff_h
#define IDnameBuff_h

#include <fileBuff.h>
#include <blockFile.h>


class IDnameBuff :  public fileBuff {

  public:
          IDnameBuff(blockFile* inFile,unsigned long blockID,char* recName);
          IDnameBuff(blockFile* inFile,unsigned long blockID,char* recName,unsigned long numBytes,uint8_t* recBuff);
          IDnameBuff(blockFile* inFile,unsigned long blockID);
  virtual ~IDnameBuff(void);

  virtual unsigned long   calculateBuffSize(void);
  virtual void            writeToBuff(char* buffPtr,unsigned long maxBytes);
  virtual unsigned long   loadFromBuff(char* buffPtr,unsigned long maxBytes);
          void            setName(char* recName);
          char*           getName(void);
          void            setRecBuff(unsigned long numBytes,uint8_t* buff);
          bool            resizeName(int numBytes);  
          bool            resizeRecBuff(unsigned long numBytes);
          
          char*         mName;
          unsigned long mNumBytes;
          uint8_t*      mBuff;     
};


#endif
