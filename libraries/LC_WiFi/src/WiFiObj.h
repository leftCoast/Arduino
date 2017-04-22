#ifndef WiFiObj_h
#define WiFiObj_h

#define WIFI_FIRMWARE_LATEST_MODEL_A "19.4.4"
#define WIFI_FIRMWARE_LATEST_MODEL_B "19.5.2"

#include "driver/include/m2m_types.h"
#include <Arduino.h>
#include <IPAddress.h>
#include <lists.h>
#include <timeObj.h>


// tstrM2MConnInfo
// This is the struct layout we receive when we get connet info.
// uint8 	__PAD24__ [3]							Padding bytes for forcing 4-byte alignment.
// char 	acSSID [M2M_MAX_SSID_LEN]	AP connection SSID name.
// uint8 	au8IPAddr [4]							Connection IP address.
// uint8 	au8MACAddress [6]					MAC address of the peer Wi-Fi station.
// sint8 	s8RSSI										Connection RSSI signal.
// uint8 	u8SecType									Security type.


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


typedef enum {
  WF_NO_CHIP = 255,
  WF_IDLE_STATUS = 0,
  WF_NO_SSID_AVAIL,
  WF_SCAN_COMPLETED,
  WF_CONNECTED,
  WF_CONNECT_FAILED,
  WF_CONNECTION_LOST,
  WF_DISCONNECTED,
  WF_AP_LISTENING,
  WF_AP_CONNECTED,
  WF_AP_FAILED,
  WF_PROVISIONING,
  WF_PROVISIONING_FAILED,
  SCAN_RESULT_COMPLETE			// Added this cause its two stage. Scan & results.
} WiFiStatus;


typedef enum {
	WF_RESET_MODE = 0,
	WF_STA_MODE,
	WF_PROV_MODE,
	WF_AP_MODE
} WiFiModes;

/* Encryption modes */
enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
	ENC_TYPE_WEP  = M2M_WIFI_SEC_WEP,
	ENC_TYPE_TKIP = M2M_WIFI_SEC_WPA_PSK,
	ENC_TYPE_CCMP = M2M_WIFI_SEC_802_1X,
	/* ... except these two, 7 and 8 are reserved in 802.11-2007 */
	ENC_TYPE_NONE = M2M_WIFI_SEC_OPEN,
	ENC_TYPE_AUTO = M2M_WIFI_SEC_INVALID
};


// We want a linked list of networks. Also, since some networks 
// "flicker" we're going to give them a watchdog timer. We'll
// let them persist for awhile. Each time they are seen, we'll
// reset the timer. Once the timer expires, they'll be deleted.

#define LIST_TIME	2000	// The're allowed this much time with no signal.

class netwkObj : public dblLinkListObj,
								 public timeObj {

	public:
		netwkObj(tstrM2mWifiscanResult* inData);
		~netwkObj(void);
		
		bool		isMe(tstrM2mWifiscanResult* inData);
		void		setData(tstrM2mWifiscanResult* inData);
		char*		getSSID(void);		// name.
		byte*		getBSSID(void);		// MAC address.
		int			getRSSI(void);		// Signal strength.
		byte		getAuth(void);		// Encryption.
		byte		getChannel(void);	// Broadcast channel.
		bool		getVisable(void);	// Some try to hide.
		
	private:
		char*		SSID;
		byte		BSSID[6];
		int			RSSI;
		byte		Auth;
		byte		channel;
		bool		visable;
};


class WiFiObj {

  public:
    WiFiObj(int cs,int irq, int rst);
    ~WiFiObj(void);

		bool			begin(void);														// Start, no hookup.
		bool			connect(char* WifiName,char* WiFiPass);	// Hookup.
    bool			begin(char* WifiName,char* WiFiPass);		// Start & hookup.
		char*			getSSID(void);
		uint32_t	getLocalIP(void);
		int32_t		getRSSI(void);
		void			macAddress(uint8_t *mac);								// It wants a 6 byte buffer.
		int8_t		scanNetworks(void);											// How many we see now?
		char*			SSID(uint8_t pos);											// Name on list.
		int32_t		RSSI(uint8_t pos);											// Signal from list.
		uint8_t		encryptionType(uint8_t pos);						// Their encryption type.
		uint8_t		channel(uint8_t pos);										// Their channel.
		uint8_t*	BSSID(uint8_t pos);											// Their MAC addr.
		void			end();
		
		void			handleDefaultConnect(void* pvMsg);
		void			handleCurrentRSSI(void* pvMsg);
		void			handleProvisionInfo(void* pvMsg);
		void			handleConnectionInfo(void* pvMsg);
		void			handleGetSystemTime(void* pvMsg);
		void			handleConnectionChange(void* pvMsg);
		void			handleScanDone(void* pvMsg);
		void			handleScanResult(void* pvMsg);
		void			handleDHCPConfig(void* pvMsg);
		
	protected:
		void			doInit(void);
		void			updateList(void);
		netwkObj* getNet(int pos);
		void			countList(void);
		
		bool				init;
    WiFiStatus  status;
    byte        mode;
    tstrM2mWifiscanResult	scanData;	// Callbacks will fill this with network info.
    int					numNets;						// Number for last scan.
    netwkObj*		netList;						// Pointer to a list of the recent networks.
    int					numInList;
    uint32_t		result;							// When we ask for something, here's a spot.
    
    
    int         DHCP;
    uint32_t		localIP;
    uint32_t		submask;
    uint32_t		gateway;
		
    char 				ssid[M2M_MAX_SSID_LEN];
    char				scan_ssid[M2M_MAX_SSID_LEN];
    byte*				remoteMACAddrPtr;
    byte				scanAuth;
    byte				scanChannel;		
};

#endif