#ifndef filePath_h
#define filePath_h

#include <lists.h>


enum pathItemType { noType, rootType, folderType, fileType };


class pathItem : public dblLinkListObj {

	public:
				pathItem(void);
	virtual	~pathItem(void);
	
				pathItemType	getType(void);
	virtual	char* 			getName(void);
	virtual	int				getNumPathChars(void);
	virtual	int				getTotalPathChars(void);
	virtual	void				addNameToPath(char* path);
	virtual	pathItem*		getParent(void);
	virtual	pathItem*		fillChildList(void);
				
				pathItemType	ourType;
				char				name[13];
};


class rootItem : public	pathItem {
	
	public:
				rootItem(void);
	virtual	~rootItem(void);
	
	virtual	int	getTotalPathChars(void);
	virtual	void	addNameToPath(char* path);
};


class fileItem : public	pathItem {
	
	public:
				fileItem(char* fileName);
	virtual	~fileItem(void);
	
	virtual	pathItem*	fillChildList(void);
	virtual	void			addNextChild(pathItem* childItem);
};


class folderItem : public	pathItem {
	
	public:
				folderItem(char* folderName);
	virtual	~folderItem(void);
	
	virtual	void	addNameToPath(char* path);	
	virtual	int	getNumPathBytes(void);
};


class filePath {

	public:
				filePath(void);
	virtual	~filePath(void);
	
				void				reset(void);
				pathItemType	checkPathPlus(char* name);
				bool				setPath(char* inPath);
				char*				getPath(void);
				pathItem*		getCurrItem(void);
				char*				getCurrItemName(void);
				void				refreshChildList(void);
				int				numChildItems(void);
				pathItem*		getChildItemByName(char* name);
				bool				pushChildItemByName(char* name);
				pathItem*		getChildItemByIndex(int index);
				bool				pushChildItemByIndex(int index);
				bool				pushItem(pathItem* theNewGuy);
				void				popItem(void);
				
				pathItem*	pathList;
				pathItem*	childList;
				char*			path;
};


#endif