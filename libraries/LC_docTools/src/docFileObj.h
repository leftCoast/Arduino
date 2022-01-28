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

#include <SDTools.h>

#define TEMP_FOLDER		"/temp/"		// Default folder to save temporary edit files in.
#define FILE_SEARCH_MS	500			// Amount of milliseconds allowed to find a temp file name.

// #ifdef FILE_WRITE
// #undef FILE_WRITE
// #define FILE_WRITE (O_READ | O_WRITE | O_CREAT)
// #endif
	
	
class docFileObj {

	public:
					enum fileModes { fClosed, fOpenToEdit, fEdited, fOpenToRead };
	
					docFileObj(const char* filePath);
	virtual		~docFileObj(void);
	
	virtual		bool			createNewDocFile(void) = 0;
	virtual		bool			openDocFile(int openMode);
	virtual		bool			saveDocFile(const char* newFilePath=NULL);
					void			closeDocFile(void);
					bool			changeDocFile(const char* newPath);
					void			setAsAutoGen(bool trueFalse=true);
					bool			fileEdited();
					char*			getName(void);
					char*			getFolder(void);	
					byte			peek(void);
					uint32_t		position(void);
					bool			seek(uint32_t index);
					uint32_t		size(void);
					int			read(void);
					uint16_t		read(byte* buff,uint16_t numBytes);
					size_t		write(uint8_t aByte);
					size_t		write(byte* buff,size_t numBytes);
					
	protected:
					
	virtual		bool		checkDoc(File inFile);
	virtual		bool		createEditPath(void);
					
					fileModes	mode;
					bool			autoGenFile;	
					char*			docFilePath;
					char*			editFilePath;
					char*			returnBuffer;
					File			ourFile;
					
							
};


#endif