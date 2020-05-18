#ifndef listServer_h
#define listServer_h

#include "blockFile.h"
#include "fileBuff.h"
#include "lilParser.h"


class bfListObj :	public fileBuff {
  
	public:
    			bfListObj(void);
	virtual	~bfListObj(void);
    
	virtual	void 				linkAfter(unsigned long present);		// Given a pointer to a node, link yourself after it.
	virtual	void				linkToEnd(unsigned long present);		// Given a pointer to a node, link yourself after the last in the chain.
	virtual	unsigned long	getNext(void);									// Pass back the next pointer.
	virtual	void				setNext(unsigned long ID);					// Point somewhere else.
	virtual	void				deleteTail(void);								// Call delete on everyone hooked to us.
	virtual	bool				isGreaterThan(unsigned long compObj);	// Are we greater than the obj being passed in? Primary sorting function.
	virtual	bool				isLessThan(unsigned long compObj);		// Are we less than the obj being passed in? Primary sorting function.
	
	virtual	unsigned	long	calculateBuffSize(void);
	virtual	void				writeToBuff(char* buffPtr,unsigned long maxBytes);
	virtual	unsigned	long	loadFromBuff(char* buffPtr,unsigned long maxBytes);
	
	protected : 	
				unsigned long next;
};
 
 
// This is the manager for a list.
class bfLinkList :	public blockFile {
  
	public:
				bfLinkList(char* bfPath);
	virtual	~bfLinkList(void);
    
	virtual	void         	addToTop(unsigned long newObj);
	virtual	void         	addToEnd(unsigned long newObj);
	virtual	void				unlinkTop(void);						// Push off the first one.
	virtual	void         	unlinkObj(unsigned long oldObj);	// Find it and push it off.
	virtual	void		 		dumpList(void);						// Call delete on everyone.
	virtual	bool				isEmpty(void);
	virtual	unsigned long	getFirst(void);
	virtual	unsigned long	getLast(void);
	virtual	unsigned long	findMax(linkListObj* present);
	virtual	unsigned long	findMin(linkListObj* present);
	virtual	void				sort(bool decending);
    
    			unsigned	long	getCount(void);
    			unsigned long	getByIndex(unsigned long index);										
 
	protected :           
				unsigned long theList;
};


class blockList
class listServer :	public lilParser {

	enum { noCommand, addItems, clearList, deleteItems, saveItems, showList };
	
	public:
				listServer();
	virtual	~listServer(void);
	
	virtual	bool	begin(char* fPath);
	virtual	bool	parseText(char* inStr);
	virtual	void	reply(void);
	
				
};
	
#endif