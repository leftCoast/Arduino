#ifndef stlList_h
#define stlList_h

#include <stlFile.h>
#include <facetList.h>


class stlList :	public stlFile,
						public facetList {

	public :
					stlList(const char* inPath);
	virtual		~stlList(void);
	
	virtual		bool			begin(void);
	virtual		bool			openList(void);
	virtual		void			closeList(void);
	virtual		long			getNumFacets(void);
	virtual		void			resetIndex(void);
					triDFacet 	readFileFacet(STLFacet* fileFacet);
	virtual		triDFacet	getTriDFacet(long inIndex);
	virtual		triDFacet	getNextTriDFacet(void);
	virtual		void			setFacet(triDFacet* facetPtr,long index);
	virtual		void			insertFacetAfter(triDFacet* facetPtr,long index);
	virtual		void			insertFacetBefore(triDFacet* facetPtr,long index);
	
					long			index;
					char*			filePath;
					bool			fileOpen;
					File			STLFile;
};


#endif