#ifndef fOpenPanel_h
#define fOpenPanel_h

#include <lilOS.h>



// **************************************************************
// ********************* dynamicPath  stuff *********************
// **************************************************************


enum pathItemType { noType, rootType, folderType, fileType };


class pathItem : public dblLinkListObj {

	public:
				pathItem(pathItemType itemType)
	virtual	~pathItem(void);
	
				pathItemType	getType(void);
	virtual	char* 			getName(void);
	virtual	int				getNumPathChars(void);
	virtual	int				getTotalPathChars(void);
	virtual	void				addNameToPath(char* path);
	virtual	pathItem*		getParent(void);
	virtual	void				fillChildList(void);
				
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
				fileItem(void);
	virtual	~fileItem(void);
	
	virtual	void	fillChildList(void);
	virtual	void	addNextChild(pathItem* childItem);
};


class folderItem : public	pathItem {
	
	public:
				folderItem(void);
	virtual	~folderItem(void);
	
	virtual	void	addNameToPath(char* path);	
	virtual	int	getNumPathBytes(void);
};


class dynamicPath {

	public:
				dynamicPath(void);
	virtual	~dynamicPath(void);
	
				void			reset(void);
				bool			pathToList(char* inPath);
				char*			listToPath(void);
				bool			nextItemByIndex(int index);
				bool			pushItem(fileItem* theNewGuy);
				bool			popItem(void);
				
				pathItem*	pathList;
				pathItem*	childList;
				char*			path;
};



// **************************************************************
// ********************** fOpenPanel stuff **********************
// **************************************************************


class fOpenPanel :	public drawObj,
							public dynamicPath {

	public:
				fOpenPanel(lilOS* ourOS,char* inPath);
	virtual	~fOpenPane(void);
	
	virtual	void	setup(void);
				void	setFilterCallback(bool(*funct)(char*));	// Use a callback to filter what you see.
				void	setResultCallback(void(*funct)(bool));		// Get a call back with the result.
				
				bool	(*filterFx)(char*);
				void	(*resultFx)(bool*);
				
				dynamicPath*	ourPath;
};