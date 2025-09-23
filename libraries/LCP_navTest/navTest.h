#ifndef navTest_h
#define navTest_h

#include <lilOS.h>
#include <lilParser.h>
#include <filePath.h>
#include <wpTools.h>


		
				
// **************************************************
// ***************** navTest ************************
// **************************************************


// Prefixes people typically use for entering path paramiters.
enum pathPrefix { setRoot, upOne, upNPath, fullPath, relPath };


class navTest	:  public panel {

	public:
				navTest(int ourAppID);
				~navTest(void);
				
				bool	setupScreen(void);
				bool	setupParser(void);
				bool	setupFakeFix(void);
				bool	setupWPList(void);
				bool	setupLowLevel(void);
	virtual	void	setup(void);
	virtual	void	loop(void);
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
				globalPos*	fakeFix;
				
				// Low level stuff.
				filePath    wd;			
};

#endif