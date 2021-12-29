#ifndef docFileObj_h
#define docFileObj_h


// Ok what's this do and what's it NOT do..
//
// TO create a docFileObj you need a path to a document. Simple as that. Once you have
// this you can..
//
// A) Open this file to read. and it will open the file, indexed to byte 0. It then
// leaves the file open to let you read out the data as you wish. When done, you simply
// call docFileClose, and it will cleanup the mess.
//
// B) Open this file to write, or edit. In this case it will create a temporary file with
// a copy of the original file. This file will be opened and indexed 0 for you to edit as
// you wish.
// 
// C) Saving changes or cloning files.. 
//
// If you call saveDocFile() while you are editing, with no path. It will save your edit
// file over the original file.
//
// If you call saveDocFile() with a path. It will save your edited file to the file of
// that path. 
//
// If You are not editing a file and call saveDocFile() with a path. It will clone the doc
// file to the file of that path.
//
// If you are not editing and call saveDocFile() with NO path. The object will probably 
// loose respect for you and you will not receive a Christmas card next Christmas.
//
// D) Closing files. When you are done reading or editing a file you can call
// closeDocFile() and this will close the file and, if present, delete the editing file as
// well.
// 
// E) Deleting the object will have much the same effect as calling closeDocFile(). In
// addition it releases the memory used for holding the file path strings.


#define TEMP_FOLDER		"/temp/"		// Default folder to save temporary edit files in.
#define FILE_SEARCH_MS	500			// Amount of milliseconds allowed to find a temp file name.

#include <Arduino.h>
#include <SD.h>


bool createTempDir(char* dirPath);

	
class docFileObj {

	public:
					enum fileModes { fClosed, fOpenToEdit, fEdited, fOpenToRead };
	
					docFileObj(char* filePath);
	virtual		~docFileObj(void);
	
	virtual		void			setNewFileParams(void)=0;			// Set up whatever info. needed to create a new file.
	virtual		bool			newDocFile(char* folderPath)=0;	// Using the info. create a new numbered file.
	virtual		bool			openDocFile(int openMode);
	virtual		bool			saveDocFile(char* newFilePath=NULL);
					void			closeDocFile(void);
					bool			changeDocFile(char* newPath);
					bool			fileEdited();	
					byte			peek(void);
					uint32_t		position(void);
					bool			seek(uint32_t index);
					uint32_t		size(void);
					int			read(void);
					uint16_t		read(byte* buff,uint16_t numBytes);
					size_t		write(uint8_t aByte);
					size_t		write(byte* buff,size_t numBytes);
					bool			createTempFile(const char* folderPath,const char* extension);
					
	protected:
					
	virtual		bool		checkDoc(File inFile);
	virtual		bool		createEditPath(void);
					
					fileModes	mode;	
					char*			pathBuff;
					char*			docFilePath;
					char*			editFilePath;
					File			ourFile;
					
							
};


#endif