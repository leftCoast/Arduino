#ifndef navTest_h
#define navTest_h

#include <lilOS.h>
#include <lilParser.h>
#include <filePath.h>
#include <wpTools.h>
#include <fontLabel.h>


#define WP_LIST_X		20
#define WP_LIST_Y		50
#define WP_LIST_W		200

#define WP_ITEM_W		WP_LIST_W
#define WP_ITEM_H		22

#define WP_NUM_ITMES	10
#define WP_LIST_H		WP_ITEM_H * WP_NUM_ITMES

#define SET_WP_BTN_N	"setWP32.bmp"
#define SET_WP_BTN_X	16
#define SET_WP_BTN_Y	WP_LIST_Y + WP_LIST_H + 16

#define ADD_WP_BTN_X	240 - 32 - 16
#define ADD_WP_BTN_Y	SET_WP_BTN_Y

#define DELETE_WP_BTN_X	ADD_WP_BTN_X - 32 - 16
#define DELETE_WP_BTN_Y	SET_WP_BTN_Y

#define EDIT_WP_BTN_X	DELETE_WP_BTN_X - 32 - 16
#define EDIT_WP_BTN_Y	SET_WP_BTN_Y



class wpDispList;
class navTest;


// **************************************************
// *************** setMarkBtn ***********************
// **************************************************


class setMarkBtn	:  public iconButton {

	public:
				setMarkBtn(const char* path,navTest* inListener);
	virtual	~setMarkBtn(void);
	
	virtual	void  	doAction(void);
	
				navTest*	ourListener;
};



// **************************************************
// *************** wpListItem ***********************
// **************************************************		
	
				
class wpListItem	:  public drawGroup {

	public:
				wpListItem(wpObj* inWpObj,wpDispList* inListMgr);
	virtual	~wpListItem(void);
					
	virtual	void  doAction(void);
	virtual	void	setThisFocus(bool setLoose);
	virtual	void	drawSelf(void);
				
				wpObj*		ourWP;
				fontLabel*	wpName;
				wpDispList*	ourListMgr;
};



// **************************************************
// *************** wpDispList ***********************
// **************************************************


class wpDispList	:  public drawList {

	public:
				wpDispList(int inX,int inY,int inW,int inH);
	virtual	~wpDispList(void);
					
	virtual	void			drawSelf(void);
				void			setSelected(wpListItem* inSelect);
				wpListItem*	getSelected(void);
				
				wpListItem* selected;
};

			
// **************************************************
// ***************** navTest ************************
// **************************************************


// Prefixes people typically use for entering path paramiters.
enum pathPrefix { setRoot, upOne, upNPath, fullPath, relPath };


class navTest	:  public panel {

	public:
				navTest(int ourAppID);
	virtual	~navTest(void);
				
				bool	setupScreen(void);
				bool	setupParser(void);
				bool	setupFakeFix(void);
				bool	setupWPList(void);
				bool	fillScreenList(void);
				bool	setupLowLevel(void);
	virtual	void	setup(void);
	virtual	void	loop(void);
				void	setMark(void);
	virtual	void	handleCom(stdComs comID);
	virtual	void	closing(void);
				void	showCurs(void);
	
				
				void	doAddWP(void);
				void	doEditWP(void);
				void	doListWP(void);
				void	doDeteWP(void);
				void	doSortNames(void);
				void  doReadWPFile();
				void  doSaveWPFile();
				void 	printHelp(void);
				
				bool			checkFile(pathItem*	item);
				pathPrefix	decodePrefix(const char* param);
				void			listDirectory(void);
				void			makeDirectory(void);
				void			changeDirectory(void);
				void			deleteItem(void);
				
				lilParser*	cmdParser;
				wpList*		ourWPList;
				wpDispList*	ourWPDisplay;
				globalPos*	fakeFix;
				
				// Low level stuff.
				filePath    wd;			
};

#endif