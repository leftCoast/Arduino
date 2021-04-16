#ifndef docFileObj_h
#define docFileObj_h

#define TEMP_FOLDER		"/temp/"		// Default folder to save temp files in. 
#define FILE_SEARCH_MS	500			// Amount of milliseconds allowed to find a temp file name.

#include <Arduino.h>
#include <SD.h>


bool createTempDir(char* dirPath);
	
			
class docFileObj {

	public:
	
					docFileObj(void);
	virtual		~docFileObj(void);
	
	virtual		bool		newDocFile(void);
	virtual		bool		openDocFile(char* filePath);
	virtual		bool		saveDocFile(char* filePath=NULL);
					
					char*		getDocFilePath(void);
					char*		getTempFilePath(void);
	protected:
					
	virtual		bool		checkDoc(void);
	virtual		bool		createTempFile(void);
	virtual		bool		initNewTempFile(void);
	virtual		bool		copyToTempFile(void);
										
					char*			docFilePath;
					char*			tempFilePath;
					bool			haveInfo;
};

// We only access these one at a time. Proabaly, these are better set up as globals so
// there is no chance for multiples copies out there in RAM. Also, it seems that the
// File's constructor is really slow!
extern File	tempFile;
extern File	docFile;

#endif