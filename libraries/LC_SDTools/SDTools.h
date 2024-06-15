#ifndef SDTools_h
#define SDTools_h

#include <SD.h>

//****************************************************************************************
//
// Little indian, big indian?
// Some files have the 2 & 4 byte integers stored in reverse byte order than what we use
// here in Arduinoland. These routines swap 'em back. (We use little indian)
//
//****************************************************************************************

// Put this at top of function, and for the duration of the function, all intergers will
// be swapped both reading and writing to files.
#define	BYTE_SWAP	bigIndian swap;


// This sets if we flip or not.
extern bool flipBytes;								


// This is set if an big/small indian function has a file error.
extern bool	SDFileErr;


// Stack based class thata flips the byte order for the calls while it's in scope.
// Automatically flips them back when going out of scope. NOT reentrant!
class bigIndian {

	public:
				bigIndian(void);
	virtual	~bigIndian(void);
};


// For reading two byte numbers.
bool read16(uint16_t* result,File f);


// For writing two byte numbers.
bool write16(uint16_t val, File f);


// For reading four byte numbers.
bool read32(uint32_t* result,File f);
  
  
// For writing four byte numbers.  
bool write32(uint32_t val, File f);




// Returns true if this folderPath can be found, or created.
bool createFolder(const char* folderPath);

// Given a path, baseName and extension this hands back a string with a path to an unused
// numbered file. For example "/docs/NoName5.doc". IF it can not allocated this file it
// will return NULL. IF THIS IS A SUCCESS, YOU MUST COPY THE RETURNED STRING. It won't last
// very long before being recycled.
char* numberedFilePath(const char* folderPath,const char* baseName,const char* extension);


// TOTAL HACK TO GET AROUND NO FILE truncate() CALL..
File TRUNCATE_FILE(const char* path);


// fcpy() : The file version of strcpy(). The dest file must be open for writing. The src
// file must be, at least, open for reading. (Writing is ok too) The dest file index is
// left pointing to the end of the file. The src file index is not changed.
void fcpy(File dest,File src);


// fcat() : The file version of strcat(). The dest file must be open for writing. The src
// file must be, at least, open for reading. (Writing is ok too) The dest file index is
// left pointing to the end of the file. The src file index is not changed.
void fcat(File dest,File src);


// Pass in your extension and a file path. returns if the file extension matches.
bool extensionMatch(const char* extension,const char* filePath);

#endif
