#ifndef regionTest_h
#define regionTest_h

#include "lilOS.h"
#include "region.h"



#include	"../../handheldOS.h"





// **********************************************************************
//  If you are going to use icons and other artwork for your application,
//  you can to use the two system folders on your SD card to store them.
//  /system/images/
//  /system/icons/
//  You should create sub folders for your images and icons so they don't
//  get mixed into all the other images and icons that might be in there.
//
//  And for lord's sakes don't call your folders "icons" an "images".
//  They need to be different than all the other image and icon folders
//  used by all the other applications.
//
//  Most likely you will need to add the full path of your application
//  icon to the source file that contains your home screen. Because it'll
//  probably need to display your application's icon for the user to
//  click on. But, how all that's accomplished is pretty much up to you.
// **********************************************************************

class LCDCharBlock	: public drawObj {

	public:	
					LCDCharBlock(int x,int y);
	virtual		~LCDCharBlock(void);
	
				void	setColors(colorObj* fCOlor,colorObj* bCOlor);
				void	setMapRow(byte row,byte bits);
				void	setMap(byte* byteArray);
				void 	clearMap(void);
				void	drawSelf(void);
				
				byte		mBits[8];
				colorObj	mFColor;
				colorObj	mBColor;
};


class regionTest	: public panel {

	public:
					regionTest(lilOS* ourOS);
	virtual		~regionTest(void);
	
	virtual void  		setup(void);
	virtual void  		loop(void);
	virtual void  		drawSelf(void);
	virtual void  		closing(void);
	
				colorObj	backColor;
				region*	mRegion;
				LCDCharBlock* aBlock;
};

#endif
