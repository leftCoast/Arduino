#ifndef wpTools_h
#define wpTools_h

#include <globalPos.h>
#include <lists.h>


class wpList;



//*********************************************
//		wayPoint
//*********************************************

class wayPoint :	public globalPos {

	public:
				wayPoint(void);
	virtual	~wayPoint(void);
				
	virtual	void	setName(const char* inName);
	virtual	char*	getName(void);
			
				bool	sameName(wayPoint* inWP);
				char*	formatFileStr(void);
				bool	readFileStr(const char* aStr);
				
				char*	ourName;
};



//*********************************************
//		wpObj
//*********************************************


enum sortOrder {
	
	byName,
	byDistance,
	byBearing
};


class wpObj :	public wayPoint,
					public linkListObj {
		
		public:
				wpObj(wpList* inMgr,const char* inName=NULL);
	virtual	~wpObj(void);
				
    virtual bool			isGreaterThan(linkListObj* compObj);	// Are we greater than the obj being passed in? Primary sorting function.
    virtual bool			isLessThan(linkListObj* compObj);		// Are we less than the obj being passed in? Primary sorting function.

				wpList*		ourMgr;
};



//*********************************************
//		wpList
//*********************************************


class wpList :	public linkList {
		
		public:
				wpList(globalPos* inFix);
	virtual	~wpList(void);
	
	virtual	void			addWP(wayPoint* inWP);
	virtual	void			outputList(void);
				void			readList(const char* filePath);
				void			saveList(const char* filePath);
				wpObj*		findWPByName(const char* inName);
				void			setSortOrder(sortOrder inOrder);
				sortOrder	getSortOrder(void);
				globalPos*	getFix(void);
				
				sortOrder	ourSortOrder;
				globalPos*	ourFix;	
};
	
					
#endif