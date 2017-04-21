#ifndef WiFiObj_h
#define WiFiObj_h

#define WIFI_FIRMWARE_LATEST_MODEL_A "19.4.4"
#define WIFI_FIRMWARE_LATEST_MODEL_B "19.5.2"

#include "driver/include/m2m_types.h"
#include <Arduino.h>
#include <IPAddress.h>

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
  WF_PROVISIONING_FAILED
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
		uint8_t*	BSSID(uint8_t pos, uint8_t* bssid);			// Their.. thing.
		uint8_t*	remoteMacAddress(uint8_t* address);			// Their Mac address.
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
		
		bool				init;
    WiFiStatus  status;
    byte        mode;
    int         DHCP;
    uint32_t		localIP;
    uint32_t		submask;
    uint32_t		gateway;
		uint32_t		resolve;
    char 				ssid[M2M_MAX_SSID_LEN];
    char				scan_ssid[M2M_MAX_SSID_LEN];
    byte*				remoteMACAddrPtr;
    byte				scanAuth;
    byte				scanChannel;		
};

#endif