#ifndef netScanner_h
#define netScanner_h

#include "driver/include/m2m_types.h"
#include "driver/include/m2m_wifi.h"
#include <lists.h>
#include <timeObj.h>
#include <idlers.h>


// tstrM2mScanDone
// This is the struct layout we receive to return the number
// of networks (APs) that have been found. 
//
// uint8 	__PAD16__ [2]		Padding bytes for forcing 4-byte alignment.
// sint8 	s8ScanState			Scan status. M2M_SUCCESS or an error.
// uint8 	u8NumofCh				Number of found APs.


// tstrM2mWifiscanResult
// This is the struct layout we receive for a scanned network (AP) data set.
//
// uint8 	_PAD8_											Padding bytes for forcing 4-byte alignment
// uint8 	au8BSSID [6]								BSSID of the AP.
// uint8 	au8SSID [M2M_MAX_SSID_LEN]	AP SSID
// sint8 	s8rssi											AP signal strength.
// uint8 	u8AuthType									AP authentication type.
// uint8 	u8ch												AP RF channel.
// uint8 	u8index											AP index in the scan result list.



// A linked list of networks. Also, since some networks 
// "flicker" we're going to give them a watchdog timer. We'll
// let them persist for awhile. Each time they are seen, we'll
// reset the timer. Once the timer expires, they'll be deleted.
// All these times are in the .cpp file to mask their names
// from other source files.

class netwkObj;	// Forward dec.
class netScanner;	

enum scanStates {
  scannerOff,		// Scanning is off. Used as a mask for the handlers.
  waiting,      // Waiting for refresh time.
  waitOnCount,  // Wait for the scan count to arrive.
  loadInfo,     // Time to fire off info load.
  waitOnInfo    // Wait for info load to arrive.
};


class scanKicker : public idler {

	public :
					scanKicker(netScanner* inScannner);
	virtual ~scanKicker(void);
		
	virtual	void			idle(void);
	
					netScanner* scanner;
	};			
	
					
// This is the header node. It manages network list.
class netScanner :	public dblLinkListObj,
												public timeObj {
	
	public:
										netScanner(void);
	virtual 					~netScanner(void);
		
					void			scanning(bool onOff);		// Turn scanning on or off.
					void			idle(void);
          bool  		startScan(void);
          bool  		startInfo(void);
					void  		handleScanDone(void* pvMsg);
					void  		handleScanResult(void* pvMsg);
					void			trimList(void);
          void      countList(void);
          void      updateList(void);
          netwkObj* getNet(byte pos);
          
          byte			getCount(void);					// Return number in list.
          char*     getSSID(byte index);		// name.
          byte*     getBSSID(byte index);		// Sorta like a MAC address.
          int	      getRSSI(byte index);		// Signal strength.
          byte      getEncrypt(byte index);	// Encryption type.
          byte      getChannel(byte index);	// Broadcast channel.
	
	private:
		scanKicker*						ourKicker;
		timeObj								scanTimer;
		scanStates						state;				// What we're up to now.
		tstrM2mWifiscanResult	scanData;			// Holder for last callback info.
    byte									lastCount;		// Filled by callback. Count of scanned APs.
    byte                  currentIndex;	// Which AP are we getting  data for now?
    byte									numInList;
};


// This is a network node. It contains the individual network data.
class netwkObj : public dblLinkListObj,
								 public timeObj {

	public:
						netwkObj(tstrM2mWifiscanResult* inData);
	virtual 	~netwkObj(void);
		
		bool		isMe(tstrM2mWifiscanResult* inData);
		void		setData(tstrM2mWifiscanResult* inData);
		char*		getSSID(void);		// name.
		byte*		getBSSID(void);		// Sorta like a MAC address.
		int			getRSSI(void);		// Signal strength.
		byte		getEncrypt(void);	// Encryption type.
		byte		getChannel(void);	// Broadcast channel.
		
	private:
		char*		SSID;
		byte		BSSID[6];
		int			RSSI;
		byte		encrypt;
		byte		channel;
};

extern netScanner* gNetworkScanObj;


#endif
