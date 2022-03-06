#ifndef stlFile_h
#define stlFile_h

#include <triDbase.h>
#include <triDVector.h>
#include <SD.h>

struct STLFacet {

	float		normVect[3];	// 12 bytes
	float		vertex1[3];		// 12 bytes
	float		vertex2[3];		// 12 bytes
	float		vertex3[3];		// 12 bytes
	int16_t	extra;			// 2 bytes
};


	
class stlFile {

	public:
				stlFile(const char* stlPath=NULL);
	virtual	~stlFile(void);
	
				void			setPath(const char* stlPath);
				bool			checkFile(void);
				char*			getSavedFileName(void);
				uint32_t		getNumFacets(void);
				bool			openForBatchRead(void);
	virtual	STLFacet		getFacet(int32_t index);
				void			closeBatchRead(void);	
				void			printSTLFile(void);
				void			printFacet(STLFacet* inFacet,int32_t index);
				
				char*			path;
				bool			isValid;
				bool			fileOpen;
				File			fileObj;
				uint32_t		numFacets;
};

#endif
