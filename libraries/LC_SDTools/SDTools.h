#ifndef SDTools_h
#define SDTools_h

#include <SD.h>

//****************************************************************************************
// little indian integers:
// Many files have the 2 & 4 byte integers stored in reverse byte order than what we use
// here in Arduinoland. These routines swap 'em back.
//
//****************************************************************************************


// For reading two byte numbers.
uint16_t read16(File f);


// For writing two byte numbers.
void write16(uint16_t val, File f);


// For reading four byte numbers.
uint32_t read32(File f);
  
  
// For writing four byte numbers.  
void write32(uint32_t val, File f);




// Returns true if this folderPath can be found, or created.
bool createFolder(const char* folderPath);

// Given a path, baseName and extension this hands back a string with a path to an unused
// numbered file. For example "/docs/NoName5.doc". IF it can not allocated this file it
// will return NULL. IF THIS IS A SUCCESS, YOU MUST COPY THE RETURNED STRING. I won't last
// very long before being recycled.
char* numberedFilePath(const char* folderPath,const char* baseName,const char* extension);


// TOTAL HACK TO GET AROUND NO FILE truncate() CALL..
File TRUNCATE_FILE(char* path);


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