#ifndef ourOSObj_h
#define ourOSObj_h

#include <lilOS.h>

enum  apps { homeApp = HOME_PANEL_ID, calcApp };

class ourOSObj : public lilOS {

   public:
           ourOSObj(void);
   virtual ~ourOSObj(void);
  
   virtual int     begin(void);               // The hardware is online, do hookups.
   virtual panel*  createPanel(int panelID);  // We need to write our own panel creation method.
   virtual void    beep(void);                // Only WE know how to make it beep.
};

extern ourOSObj  ourOS;

#endif
