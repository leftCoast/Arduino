#ifndef NMEAHandlers_h
#define NMEAHandlers_h


#include <llama2000.h>
#include <GPSHandlers.h>

/*
 
// ***************************************************************************************
//                               ----- msgHandler -----
// ***************************************************************************************


// Base class for handling and creation of SAE J1939 network messages. Inherit this create
// your handler object and add them using the netObj call addMsgHandler().
class msgHandler :   public linkListObj { 

   public:
            msgHandler(netObj* inNetObj);
            ~msgHandler(void);
            
            int   getNumBytes(void);            // These only set the value. No buffer here.
            void  setNumBytes(int inNumBytes);  // The message class reads  and uses this.
   virtual  bool  handleMsg(message* inMsg);    // Fill in to handle messages.
   virtual  void  newMsg(void);                 // Fill in to create messages.
   virtual  void  sendMsg(message* inMsg);      // This one just sends messages on their way.
   
            void  setSendInterval(float inMs);  // Used for broadcasting.
            float getSendInterval(void);        //
   virtual  void  idleTime(void);               // Same as idle, but called by the netObj.
            
            netObj*  ourNetObj;                 // Pointer to our boss!
            timeObj  intervaTimer;              // If broadcasting, how often do we broadcast? (Ms)
};

*/

// These are the sources of GPS data.
extern GPVTG   trackMadeGood;  
extern GPGGA   fixData; 
extern GPGSA   activeSatellites; 
extern GPGSV   SatellitesInView; 
extern GPRMC   minTransData;
extern byte    sid;



// ****************************************************
// PGN 0x1F801 - Position, Rapid Update
// ****************************************************


class PGN0x1F801Handler :  public msgHandler {

    public:
            PGN0x1F801Handler(netObj* inNetObj);
            ~PGN0x1F801Handler(void);
            
   virtual  void  newMsg(void);                 // Fill in to create messages.
};



// ****************************************************
// PGN 0x1F802: PGN 129026 - COG & SOG.
// ****************************************************


class PGN0x1F802Handler :  public msgHandler {

    public:
            PGN0x1F802Handler(netObj* inNetObj);
            ~PGN0x1F802Handler(void);
            
   virtual  void  newMsg(void);                 // Fill in to create messages.
};



// ****************************************************
// PGN 0x1F805: PGN 129029 - GNSS Position Data
// ****************************************************


class PGN0x1F805Handler :  public msgHandler {

    public:
            PGN0x1F805Handler(netObj* inNetObj);
            ~PGN0x1F805Handler(void);
            
   virtual  void  newMsg(void);                 // Fill in to create messages.
};




#endif
