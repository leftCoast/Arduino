#include <lilOS.h>      // From LC_lilOS library
#include <breakout.h>   // From the LCP_breakout library.
#include <rpnCalc.h>    // From the LCP_rpnCalc library.


#define  BEEP_PIN     23          // Pin numbers must match your hard=ware.
#define SYSTEM_PATH   "/system/"  // This needs to be set as it is on the SD card.

// Our list of applications. Each applicatoin comes in a folder bearing it's
// name. Inside this folder will be a 32x32 pixel icon with the same name.
// Remember, filenames in this thing are 8.3. So your applicaiton name must
// be 8 chars or less. The icon, as currently are all graphiins here, is a
// .bmp file.
enum  apps {        
  homeApp = HOME_PANEL_ID,
  breakoutApp,
  calcApp
};


// **************** ourSOClass ***************

// Our version of the OS that includes our hardware configureation info.
class ourOSClass : public lilOS {

   public:
            ourOSClass(void);
   virtual  ~ourOSClass(void);

   virtual  panel*      createPanel(int panelID);
    virtual void        beep(void);
    virtual int         getTonePin(void);
    virtual const char* getSystemFolder(void);
    virtual const char* getPanelName(int panelID);
};


// ***************** ourHome ***********

// Our verstion of a homePanel, we get to decide how it looks and functions.
class ourHome : public homePanel {

   public:
           ourHome(void);
   virtual ~ourHome(void);
  
   virtual void    setup(void);
   virtual void    loop(void);
   virtual void    drawSelf(void);

         bmpObj*  mBackImage;
};


extern ourOSClass ourOS;   // We "expose" our OS to the world here.
