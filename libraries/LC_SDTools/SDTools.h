#ifndef SDTools_h
#define SDTools_h

#include <SD.h>


// #ifdef FILE_WRITE
// #undef FILE_WRITE
// #define FILE_WRITE (O_READ | O_WRITE | O_CREAT)
// #endif


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


#endif
