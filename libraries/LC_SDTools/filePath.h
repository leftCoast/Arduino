#ifndef filePath_h
#define filePath_h

#include <lists.h>



//****************************************************************************************
// pathItem:
//
// This will have the default actions. For all the different types pf path items. Root,
// folder, file. This pathItem is pure virtual so it'll never be able to be created.
//****************************************************************************************


enum pathItemType { noType, rootType, folderType, fileType };

class pathItem : public dblLinkListObj {

	public:
				pathItem(void);
				pathItem(pathItem* aGrandItem);
	virtual	~pathItem(void);
	
				pathItemType	getType(void);
	virtual	char* 			getName(void);
	virtual	int				getNumPathChars(void);
	virtual	void				addNameToPath(char* path) =0;
	virtual	pathItem*		getParent(void);
				pathItem*		getNext(void);
				
				pathItemType	ourType;
				char				name[13];
};



//****************************************************************************************
// rootItem:
//
// All paths start with one. Basically the SD card.
//****************************************************************************************


class rootItem : public	pathItem {
	
	public:
				rootItem(void);
	virtual	~rootItem(void);
	
	virtual	int	getNumPathChars(void);
	virtual	void	addNameToPath(char* path);
};



//****************************************************************************************
// fileItem:
//
// These are so useful. You can store stuff in 'em!
//****************************************************************************************


class fileItem : public	pathItem {
	
	public:
				fileItem(const char* fileName);
	virtual	~fileItem(void);
	
	virtual	void addNameToPath(char* path);
};



//****************************************************************************************	
// folderItem:
//
// A little more complicated than a silly old file.
//****************************************************************************************


class folderItem : public	pathItem {
	
	public:
				folderItem(const char* folderName);
	virtual	~folderItem(void);
	
	virtual	int	getNumPathChars(void);
	virtual	void	addNameToPath(char* path);
};



//****************************************************************************************
// filePath:
//
// And now we get to the actual filePath class. This is the bit the user will typically
// interact with. And the bit that will interact with the SD card's file system as well.
//
//****************************************************************************************


class filePath {

	public:
				filePath(void);
	virtual	~filePath(void);
	
	virtual	void				reset(void);
				int				numPathBytes(void);
				pathItemType	checkPathPlus(const char* name);
	virtual	bool				setPath(const char* inPath);
	virtual	char*				getPath(void);
				pathItem*		getCurrItem(void);
				char*				getCurrItemName(void);
				void				dumpChildList(void);
				void				refreshChildList(void);
				int				numChildItems(void);
				pathItem*		getChildItemByName(const char* name);
				bool				pushChildItemByName(const char* name);
	virtual	bool				pushItem(pathItem* theNewGuy);
	virtual	void				popItem(void);
				bool				deleteCurrentItem(void);
				
				pathItem*	pathList;
				pathItem*	childList;
				char*			path;
};

#endif