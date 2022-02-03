#ifndef stlFile_h
#define stlFile_h

#include <triDbase.h>


struct STLFacet {

	float	normVect[3];	// 12 bytes
	float	vertex1[3];		// 12 bytes
	float	vertex2[3];		// 12 bytes
	float	vertex3[3];		// 12 bytes
	int8_t	extra[2];	// 2 bytes
};


class stlFile : {

	public:
				stlFile(const char* stlPath);
	virtual	~stlObj(void);
	
				bool		checkFile(void);
				char*		getSavedFileName(void);
				long		getNumFacets(void);
				bool		openForBatchRead(void);
				STLFacet	getFacet(long index);
				void		closeBatchRead(void);	
				void		printSTLFile(void);
				void		printFacet(STLFacet* inFacet);
				
				char*	path;
				bool	isValid;
				bool	fileOpen;
				File	fileObj;
};

#endif