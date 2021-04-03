#ifndef appName_h
#define appName_h

#include "panel.h"
#include "lilOS.h"

// **********************************************************************
//                            newApp template. 
//
//  This is the starting point for creating a new "application" panel for
//  a handheld device.
//
//  How to use :
//  First you must pick two names. 
// 
//  Your applicatoin name
//         ** and **
//  Your application ID name. 
//
//  The application name will typically be the name used for your app's
//  .h and .cpp files. As well as your app's class name.
// 
//  The application's ID name must be different than the application's
//  name. The application's ID name is placed in your OS's apps enum.
// 
//  Substitute your application name for appName.
//  Substitute your application ID name for newAppID.
//  Once this is done, save this under your new application name as its
//  .h file. 
//
//  Lastly, go to appName.cpp and change the names there just as you did
//  here. Then save that file under your new application name as its
//  .cpp file.
//
// **********************************************************************


// **********************************************************************
// 						THIS HAS TO POINT TO YOUR OS
//
#include	"../../handheldOS.h"
//
// **********************************************************************


// **********************************************************************
//  This needs to be added to your os's .h file :
//
//  There will be a line beginning.. 
// 
//  num  apps { homeApp = HOME_PANEL_ID,
//
//  You need to add newAppID to this list.
// **********************************************************************


// **********************************************************************
//  These need to be added to your os's.cpp file :
//
//                     #include "appName.h"
//
//                   Or, if using a src folder..
//
//                 #include "src/appName/appName.h"
//
//  And in your OS's createPanel(int panelID) method, add the line..
//
//					"case newAppID : return new appName();"
//
// **********************************************************************


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



class appName	: public panel {

	public:
					appName(void);
	virtual		~appName(void);
	
	virtual void  setup(void);
	virtual void  loop(void);
	virtual void  drawSelf(void);
	virtual void  closing(void);
};

#endif