#ifndef filePath_h
#define filePath_h

#include <lists.h>


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


class rootItem : public	pathItem {
	
	public:
				rootItem(void);
	virtual	~rootItem(void);
	
	virtual	int	getNumPathChars(void);
	virtual	void	addNameToPath(char* path);
};


class fileItem : public	pathItem {
	
	public:
				fileItem(char* fileName);
	virtual	~fileItem(void);
	
	virtual	void addNameToPath(char* path);
};


class folderItem : public	pathItem {
	
	public:
				folderItem(char* folderName);
	virtual	~folderItem(void);
	
	virtual	int	getNumPathChars(void);
	virtual	void	addNameToPath(char* path);
};


class filePath {

	public:
				filePath(void);
	virtual	~filePath(void);
	
	virtual	void				reset(void);
				int				numPathBytes(void);
				pathItemType	checkPathPlus(char* name);
	virtual	bool				setPath(char* inPath);
	virtual	char*				getPath(void);
				pathItem*		getCurrItem(void);
				char*				getCurrItemName(void);
				void				refreshChildList(void);
				int				numChildItems(void);
				pathItem*		getChildItemByName(char* name);
				bool				pushChildItemByName(char* name);
	virtual	bool				pushItem(pathItem* theNewGuy);
	virtual	void				popItem(void);
				bool				deleteCurrentItem(void);
				
				pathItem*	pathList;
				pathItem*	childList;
				char*			path;
};

#endif