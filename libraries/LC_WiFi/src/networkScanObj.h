#ifndef networkScanObj_h
#define networkScanObj

#include <lists.h>
#include <timeObj.h>


/* Encryption modes */
enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
	ENC_TYPE_WEP  = M2M_WIFI_SEC_WEP,
	ENC_TYPE_TKIP = M2M_WIFI_SEC_WPA_PSK,
	ENC_TYPE_CCMP = M2M_WIFI_SEC_802_1X,
	/* ... except these two, 7 and 8 are reserved in 802.11-2007 */
	ENC_TYPE_NONE = M2M_WIFI_SEC_OPEN,
	ENC_TYPE_AUTO = M2M_WIFI_SEC_INVALID
};


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

#define LIST_TIME   5000	// The're allowed this much time with no signal.
#define SLEEP_TIME  1000  // This much time between scans.


class netwkObj;	// Forward dec.

extern enum scanStates;


// This is the header node. It manages network list.
class networkScanObj :	public dblLinkListObj,
												public timeObj,
												public idler {
	
	public:
			networkScanObj(void);
			~networkScanObj(void);
		
					void		scanning(bool onOff);		// Turn scanning on or off.
  virtual	void	idle(void);
          bool  startScan(void);
          bool  startInfo(void);
					void  handleScanDone(void* pvMsg);
					void  handleScanResult(void* pvMsg);
          
          void      countList(void);
          void      updateList(void);
          netwkObj* getNet(byte pos);
          char*     getSSID(int index);			// name.
          byte*     getBSSID(int index);		// Sorta like a MAC address.
          byte      getRSSI(int index);     // Signal strength.
          byte      getEncrypt(int index);	// Encryption type.
          byte      getChannel(int index);	// Broadcast channel.
	
	private:
    bool                  areScanning;
		scanStates						state;				// What we're up to now.
		tstrM2mWifiscanResult	scanData;		// Holder for last callback info.
    byte									lastCount;	// Also filled by callback.
    byte                  currentIndex;
};


// This is a network node. It contains the individual network data.
class netwkObj : public dblLinkListObj,
								 public timeObj {

	public:
		netwkObj(tstrM2mWifiscanResult* inData);
		~netwkObj(void);
		
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

extern networkScanObj* gNetworkScanObj;


#endif
