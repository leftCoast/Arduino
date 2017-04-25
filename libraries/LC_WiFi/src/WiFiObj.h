#ifndef WiFiObj_h
#define WiFiObj_h

#define WIFI_FIRMWARE_LATEST_MODEL_A "19.4.4"
#define WIFI_FIRMWARE_LATEST_MODEL_B "19.5.2"

#include "driver/include/m2m_types.h"
#include <Arduino.h>
#include <IPAddress.h>
#include <timeObj.h>
#include "netScanner.h"

// tstrEthInitParam
// This is passed in with m2m_wifi_init() function parameter.
// uint8				__PAD8__					Padding.
// uint8*				au8ethRcvBuf			Pointer to Receive Buffer of Ethernet Packet.
// tpfAppEthCb 	pfAppEthCb				Callback for Ethernet interface.
// tpfAppWifiCb	pfAppWifiCb				Callback for wifi notifications.
// uint16				u16ethRcvBufSize	Size of Receive Buffer for Ethernet Packet.
// uint8				u8EthernetEnable	Enable Ethernet mode flag.


// tstrWifiInitParam
// We pass this into the m2m_wifi_init() function.
// tpfAppMonCb			pfAppMonCb				Callback for monitoring interface.
// tpfAppWifiCb			pfAppWifiCb				Callback for Wi-Fi notifications.
// tstrEthInitParam	strEthInitParam		Structure to hold Ethernet interface parameters.


// tstrM2MConnInfo
// This is the struct layout we receive when we get connet info.
// uint8 	__PAD24__ [3]							Padding bytes for forcing 4-byte alignment.
// char 	acSSID [M2M_MAX_SSID_LEN]	AP connection SSID name.
// uint8 	au8IPAddr [4]							Connection IP address.
// uint8 	au8MACAddress [6]					MAC address of the peer Wi-Fi station.
// sint8 	s8RSSI										Connection RSSI signal.
// uint8 	u8SecType									Security type.


// tstrM2mWifiStateChanged 
// When there is a connection state change we get one of these. Not very well documented.
// uint8 	__PAD16__ [2]		The padding.
// uint8 	u8CurrState			Current Wi-Fi connection state.
// uint8 	u8ErrCode				tenuM2mConnChangedErrcode.


// tstrM2MIPConfig
// We get this with the handleDHCPConfig handeler call.
// uint32 	u32DhcpLeaseTime		Dhcp Lease Time in sec.
// uint32 	u32DNS							IP for the DNS server.
// uint32 	u32Gateway					IP of the Default internet gateway.
// uint32 	u32StaticIP					The static IP assigned to the device.
// uint32 	u32SubnetMask				Subnet mask for the local area network.


// tstrSystemTime
// For holding a date & time.
// uint8 	__PAD8__		Padding
// uint16 u16Year			The rest are kinda' self explanatory.
// uint8 	u8Day
// uint8 	u8Hour
// uint8 	u8Minute
// uint8 	u8Month 
// uint8 	u8Second


/* Encryption modes */
enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
	ENC_TYPE_WEP  = M2M_WIFI_SEC_WEP,
	ENC_TYPE_TKIP = M2M_WIFI_SEC_WPA_PSK,
	ENC_TYPE_CCMP = M2M_WIFI_SEC_802_1X,
	/* ... except these two, 7 and 8 are reserved in 802.11-2007 */
	ENC_TYPE_NONE = M2M_WIFI_SEC_OPEN,
	ENC_TYPE_AUTO = M2M_WIFI_SEC_INVALID
};


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
  RSSI_DONE,
  SCAN_RESULT_COMPLETE			// Added this cause its two stage. Scan & results.
} WiFiStatus;


typedef enum {
	WF_RESET_MODE = 0,
	WF_STA_MODE,
	WF_PROV_MODE,
	WF_AP_MODE
} WiFiModes;



class WiFiObj {

  public:
    WiFiObj(int cs,int irq, int rst);
    ~WiFiObj(void);

		bool			begin(void);														// Start, no hookup.
    bool			begin(char* WifiName,char* WiFiPass);		// Start & hookup.
    bool			connect(char* WifiName,char* WiFiPass);	// Hookup.
    void			scanning(bool onOff);
    void			end();																	// Tears everything down, frees memory.
    char*			getSSID(void);													// name.
    byte*			getMACAddr(void);												// Our MAC address.
    byte*			getRemoteMACAddr(void);									// Their MAC address.
    uint32_t	getLocalIP(void);
		byte*			getBSSID(void);													// Sorta like a MAC address.
		int				getRSSI(void);													// Signal strength.
		byte			getEncrypt(void);													// Encryption type.
		byte			getChannel(void);												// Broadcast channel.
    void			scanNetworks(bool onOff);								// Turn scanning on or off.
    byte			getNetworkCount(void);
		char*			getSSID(int index);											// name.
		byte*			getBSSID(int index);										// Sorta like a MAC address.
		int				getRSSI(int index);											// Signal strength.
		byte			getEncrypt(int index);											// Encryption type.
		byte			getChannel(int index);									// Broadcast channel.
		
		void			handleDefaultConnect(void* pvMsg);
		void			handleCurrentRSSI(void* pvMsg);
		void			handleProvisionInfo(void* pvMsg);
		void			handleConnectionInfo(void* pvMsg);
		void			handleGetSystemTime(void* pvMsg);
		void			handleConnectionChange(void* pvMsg);
		void			handleDHCPConfig(void* pvMsg);
		void			handleReqestWPS(void* pvMsg);
		void			handleIPConflict(void* pvMsg);
		void			handleClientInfo(void* pvMsg);
		
	protected:
		void			doInit(void);
		void			updateList(void);
		netwkObj* getNet(int pos);
		void			countList(void);
		
		bool						init;
    WiFiStatus  		status;
    byte        		mode;
    tstrSystemTime	time;
    
    uint32_t				leaseTime;
		uint32_t				DNS;
    int         		DHCP;
    uint32_t				localIP;
    uint32_t				submask;
    uint32_t				gateway;

    byte		SSID[M2M_MAX_SSID_LEN+1];	// Actually SSID is a 32 byte buffer. Most use it like a string.
    byte		remoteIPAddr[4];					// IP of the AP we hooked to.
    byte		remoteMACAddr[6];					// Says its the AP MAC addr we hooked to.
    byte		MACAddr[6];								// Our MAC addr. Again, its what the documentation says.
		byte		BSSID[6];									// BSSID is used like the wireless MAC Address.
		int			RSSI;											// RSSI is the signal strength.
		byte		Auth;											// The type of encryption were using.
		byte		channel;									// The channel to broadcast/receive on.
};


#endif