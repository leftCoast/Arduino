#ifndef ourOSObj_h
#define ourOSObj_h

#include <lilOS.h>

enum  apps { homeApp = HOME_PANEL_ID, calcApp, breakoutApp };

class ourOSObj : public lilOS {

   public:
           ourOSObj(void);
   virtual ~ourOSObj(void);
  
   virtual  int      begin(void);                     // The hardware is online, do hookups.
   virtual  panel*   createPanel(int panelID);        // We need to write our own panel creation method.
   virtual  void     beep(void);                      // Only WE know how to make it beep.
   virtual  int      getTonePin(void);
   virtual  void     setBrightness(byte brightness);  // 0 for full bright 255 for off.
   virtual  char*    panelFolder(int panelID);

            char     pathBuff[100];                   // Workspace for building up file paths.
};

extern ourOSObj  ourOS;

#endif
