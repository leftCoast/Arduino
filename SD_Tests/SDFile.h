#include <filePath.h>

enum fileStat   { fileUnknown, fileMissing, fileClosed, openRead, openWrite }; 
enum volumeType { fat16, fat32 };

    
class SDFile {

  public:

    
    
          SDFile(const char* inName);
  virtual ~SDFile(void);

          bool  openRoot(volume);
          void  close(void);

  protected:
 
          pathItemType  fileType;
          fileStat      status;
          uint32_t      cluster;      // cluster for current file position
          uint32_t      position;     // current file position in bytes from beginning
          uint32_t      dirBlock;     // SD block that contains directory entry for file
          uint8_t       dirIndex;     // index of entry in dirBlock 0 <= dirIndex_ <= 0XF
          uint32_t      fileSize;     // file size in bytes
          uint32_t      firstCluster; // first cluster of file
          SDVol*        volume;       // volume where file is located
}
