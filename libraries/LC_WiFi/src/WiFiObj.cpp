#include "WiFiObj.h"
#include <timeObj.h>

extern "C" {
  #include "bsp/include/nm_bsp.h"
  #include "bsp/include/nm_bsp_arduino.h"
  #include "driver/source/nmasic.h"
  #include "driver/include/m2m_periph.h"
  #include "driver/include/m2m_wifi.h"
  #include "driver/include/m2m_ssl.h"
}

char errStr[100];
char infoStr[100];	
long aLong;
static void clrError(void) { errStr[0]='\0'; }
static void outError(void) { Serial.println(errStr); clrError(); }
static void	outLong(void)	 { Serial.println(aLong); }
static void clrInfo(void) { infoStr[0]='\0'; }
static void outInfo(void) { Serial.println(infoStr); clrInfo(); }

// We only get one. This is set up when begin() is called.
// Its used as a link for the callbacks.
WiFiObj* gWiFi = NULL;


static void wifiCallback(uint8_t u8MsgType, void* pvMsg) {

	if (gWiFi) {																	// Sanity. We have something to talk to?
		switch (u8MsgType) {
			case M2M_WIFI_RESP_DEFAULT_CONNECT   : gWiFi->handleDefaultConnect(pvMsg); break;
			case M2M_WIFI_RESP_CURRENT_RSSI      : gWiFi->handleCurrentRSSI(pvMsg); break;
			case M2M_WIFI_RESP_PROVISION_INFO    : gWiFi->handleProvisionInfo(pvMsg); break;
			case M2M_WIFI_RESP_CONN_INFO         : gWiFi->handleConnectionInfo(pvMsg); break;
			case M2M_WIFI_RESP_GET_SYS_TIME      : gWiFi->handleGetSystemTime(pvMsg); break;
			case M2M_WIFI_RESP_CON_STATE_CHANGED : gWiFi->handleConnectionChange(pvMsg); break;
			case M2M_WIFI_RESP_SCAN_DONE         : gWiFi->handleScanDone(pvMsg); break;
			case M2M_WIFI_RESP_SCAN_RESULT       : gWiFi->handleScanResult(pvMsg); break;
			case M2M_WIFI_REQ_DHCP_CONF          : gWiFi->handleDHCPConfig(pvMsg); break;
		}
  }
}


WiFiObj::WiFiObj(int cs,int irq, int rst) {


	gi8Winc1501CsPin = cs;
	gi8Winc1501IntnPin = irq;
	gi8Winc1501ResetPin = rst;
	gi8Winc1501ChipEnPin = -1;  // Wire the damn thing to 5V. 

  mode							= WF_RESET_MODE;
  status						= WF_NO_CHIP;
  remoteMACAddrPtr	= NULL;
  init = false;
}


WiFiObj::~WiFiObj(void) { }

// We can start with no hookup.
bool WiFiObj::begin(void) {

	if (!init) {					// If we ain't initialzed.
	  gWiFi = this;				// First things first. Setup our global.
  	doInit();						// Call doInit(). That sets the flag init.
  }
  return init;					// Report!
}


// Do all the stuff to get this thing online in one shot.
// It can block up to 60 sec.
bool WiFiObj::begin(char* WifiName,char* WiFiPass) {

  bool    success;
  
  success = begin();									// Go through the init stuff.
  if (success) {
  	success = connect(WifiName,WiFiPass);
  }
  return success;
} 


// Once we're running we can connect to local WiFi. Maybe? 
bool WiFiObj::connect(char* WifiName,char* WiFiPass) {

	int     result;
  timeObj timeout(60000);
  bool    success;
  
  success = false;	
  if (init) {
    if (DHCP) {           // Connect to router? Didn't we just do this?
      localIP = 0;
      submask = 0;
      gateway = 0;  
    }
    result = m2m_wifi_connect(WifiName,strlen(WifiName),M2M_WIFI_SEC_WPA_PSK,(void*)WiFiPass,M2M_WIFI_CH_ALL);
    if ( result>= 0) {
      status = WF_IDLE_STATUS;
      mode = WF_STA_MODE;
      timeout.start();
      while (!(status & WF_CONNECTED) && !(status & WF_DISCONNECTED) && !timeout.ding()) {
        m2m_wifi_handle_events(NULL); // run callbacks.
      }
      if (status & WF_CONNECTED) {
        success = true;                 // We did it! We're connected!
      } else {
        mode = WF_RESET_MODE;
        Serial.print("m2m_wifi_connect() timed out.");
      } 
      memset(ssid, 0, M2M_MAX_SSID_LEN);					// After testing, change to strcpy();
			memcpy(ssid, WifiName, strlen(WifiName));
    } else {
      status = WF_CONNECT_FAILED;
      Serial.print("m2m_wifi_connect() failed with:");Serial.println(result);
    }
  } else {
    Serial.println("Not correctly initialized");
  }
  return success;
}


// We don't pass back success, we set the init flag.
void WiFiObj::doInit(void) {					

  int               result;
  tstrWifiInitParam param;
  extern uint32     nmdrv_firm_ver;
  
  if (!init) {														// Once is enough!
		result = nm_bsp_init();                 // Initialize the WiFi BSP. What's a BSP?
		if (result==M2M_SUCCESS) {
			param.pfAppWifiCb = wifiCallback;         // Register the callback. But what about everything else?
			result = m2m_wifi_init(&param);              // See if it registers.
			if (result==M2M_SUCCESS) {
				if (nmdrv_firm_ver >= M2M_MAKE_VERSION(19, 5, 0)) {   // enable AES-128 and AES-256 Ciphers, if ok. Really? Whatever!
					m2m_ssl_set_active_ciphersuites(SSL_NON_ECC_CIPHERS_AES_128 | SSL_NON_ECC_CIPHERS_AES_256);
				}
				status = WF_IDLE_STATUS;
				localIP = 0;
				submask = 0;
				gateway = 0;
				DHCP = 1;
				init = true;	// flag saying init was a success!
			} else {
				Serial.print("m2m_wifi_init() failed with:");Serial.println(result);
			}
		} else {
			Serial.print("nm_bsp_init() failed with:");Serial.println(result);
		}
	}
}


char* WiFiObj::getSSID(void) {
	
	if (status == WF_CONNECTED || status == WF_AP_LISTENING || status == WF_AP_CONNECTED) {
		return ssid;
	}
	else {
		return 0;
	}
}


uint32_t WiFiObj::getLocalIP(void) { return localIP; }


int32_t WiFiObj::getRSSI(void) {
	
	timeObj	timeOut(1000);								// Create a timer.
	int32_t rssi;
	
	rssi = -1;
	if (!init) {
		m2m_wifi_handle_events(NULL);					// Clear pending events.
		resolve = 0;													// Send RSSI request.
		if (m2m_wifi_req_curr_rssi() < 0) {
			return 0;
		}
		timeOut.start();														// Set a timer and spin for answer..
		while (resolve == 0 && !timeOut.ding()) {
			m2m_wifi_handle_events(NULL);
		}
		rssi = resolve;
		resolve = 0;	
	}
	return rssi;
}
	
	
void WiFiObj::end() {
	
	if (init) {
		if (mode == WF_AP_MODE) {
			m2m_wifi_disable_ap();
		} else {
			if (mode == WF_PROV_MODE) {
				m2m_wifi_stop_provision_mode();
			}
			m2m_wifi_disconnect();
		}
		m2m_wifi_deinit(NULL);
		nm_bsp_deinit();
		mode = WF_RESET_MODE;
		status = WF_NO_CHIP;
		init = false;
	}
}


void	WiFiObj::macAddress(uint8_t *mac) {

	byte tmpMac[6];
	byte i;
	
	if (init) {
		m2m_wifi_get_mac_address(mac);		// Why?
		m2m_wifi_get_mac_address(tmpMac);
		for(i = 0; i < 6; i++) {					// I guess reverse is the new forward?
			mac[i] = tmpMac[5-i];
		}
	}
}


int8_t WiFiObj::scanNetworks(void) {

	WiFiStatus 	tmp;
	timeObj			timeout(5000);
	
	if (init) {
		tmp = status;
		if (m2m_wifi_request_scan(M2M_WIFI_CH_ALL) >= 0) { // Start scan.
			status = WF_IDLE_STATUS;				// Wait for scan result or timeout:
			timeout.start();
			while (!(status & WF_SCAN_COMPLETED) && !timeout.ding()) {
				m2m_wifi_handle_events(NULL);
			}
			status = tmp;
			return m2m_wifi_get_num_ap_found();
		} else {
			Serial.println("m2m_wifi_request_scan() failed.");
		}
	} else {
		Serial.println("Not initialized.");
	}
	return -1;	// Error flag.
}


char* WiFiObj::SSID(uint8_t pos) {

	WiFiStatus tmp;
	timeObj			timeout(2000);
	
	if (init) {
		tmp = status;																// Save status.
		memset(scan_ssid, 0, M2M_MAX_SSID_LEN);			// Clear scan_ssid.
		if (m2m_wifi_req_scan_result(pos) >= 0) {		// We want a name!
			status = WF_IDLE_STATUS;									// Wait for name or timeout.
			timeout.start();
			while (!(status & WF_SCAN_COMPLETED) && !timeout.ding()) {
				m2m_wifi_handle_events(NULL);
			}
			status = tmp;															// restore status.
			resolve = 0;															// Was resolve used? Yes, rssi is in there.
			return scan_ssid;													// All is good return pointer.
		} else {
			Serial.println("m2m_wifi_req_scan_result() failed.");
		}
	} else {
		Serial.println("Not initialized.");
	}
	return NULL;		// Errors! Return NULL
}



int32_t WiFiObj::RSSI(uint8_t pos) {

	WiFiStatus tmp;
	int32_t			rssi;
	timeObj			timeout(2000);
	
	if (init) {
		tmp = status;																// Save status.
		memset(scan_ssid, 0, M2M_MAX_SSID_LEN);			// Clear scan_ssid.
		if (m2m_wifi_req_scan_result(pos) >= 0) {		// We want a name!
			status = WF_IDLE_STATUS;									// Wait for name or timeout.
			timeout.start();
			while (!(status & WF_SCAN_COMPLETED) && !timeout.ding()) {
				m2m_wifi_handle_events(NULL);
			}
			status = tmp;															// restore status.
			int32_t rssi = resolve;										// It comes back in resolve!
			resolve = 0;															// Clear resolve.
			return rssi;															// All is good, return rssi.
		} else {
			Serial.println("m2m_wifi_req_scan_result() failed.");
		}
	} else {
		Serial.println("Not initialized.");
	}
	return 0;		// Errors! Return 0
}


uint8_t WiFiObj::encryptionType(uint8_t pos) {

	WiFiStatus tmp;
	timeObj			timeout(2000);
	
	if (init) {
		tmp = status;																// Save status.
		if (m2m_wifi_req_scan_result(pos) >= 0) {		// We want a name!
			status = WF_IDLE_STATUS;									// Wait for name or timeout.
			timeout.start();
			while (!(status & WF_SCAN_COMPLETED) && !timeout.ding()) {
				m2m_wifi_handle_events(NULL);
			}
			status = tmp;															// restore status.
			resolve = 0;															// Clear resolve.
			return scanAuth;													// All is good, return type.
		} else {
			Serial.println("m2m_wifi_req_scan_result() failed.");
		}
	} else {
		Serial.println("Not initialized.");
	}
	return 0;		// Errors! Return 0
}


uint8_t WiFiObj::channel(uint8_t pos) {

	WiFiStatus tmp;
	timeObj			timeout(2000);
	
	if (init) {
		tmp = status;																// Save status.
		if (m2m_wifi_req_scan_result(pos) >= 0) {		// We want a name!
			status = WF_IDLE_STATUS;									// Wait for name or timeout.
			timeout.start();
			while (!(status & WF_SCAN_COMPLETED) && !timeout.ding()) {
				m2m_wifi_handle_events(NULL);
			}
			status = tmp;															// restore status.
			resolve = 0;															// Clear resolve.
			return scanChannel;												// All is good, return type.
		} else {
			Serial.println("m2m_wifi_req_scan_result() failed.");
		}
	} else {
		Serial.println("Not initialized.");
	}
	return 0;		// Errors! Return 0
}


uint8_t* WiFiObj::remoteMacAddress(uint8_t* address) {

	timeObj	timeout(1000);
	
	remoteMACAddrPtr = address;								// Point ours at theirs.
	memset(address, 0, 6);										// Zero it out. (theirs)
	if (m2m_wifi_get_connection_info()>=0) {	// Call to fill it up.
		timeout.start();												// Wait for data or timeout.
		while (remoteMACAddrPtr != 0 && timeout.ding()) {
			m2m_wifi_handle_events(NULL);
		}
		remoteMACAddrPtr = 0;
	}
	return address;		// I guess, error or not. Return it.
}


uint8_t* WiFiObj::BSSID(uint8_t pos, uint8_t* bssid) {

	WiFiStatus tmp;
	timeObj			timeout(2000);
	
	if (init) {
		tmp = status;																// Save status.
		if (m2m_wifi_req_scan_result(pos) >= 0) {		// We want a name!
			status = WF_IDLE_STATUS;									// Wait for name or timeout.
			timeout.start();
			while (!(status & WF_SCAN_COMPLETED) && !timeout.ding()) {
				m2m_wifi_handle_events(NULL);
			}
			status = tmp;															// restore status.
			resolve = 0;															// Clear resolve.
			remoteMACAddrPtr = 0;
			return bssid;															// All is good, return bssid.
		} else {
			Serial.println("m2m_wifi_req_scan_result() failed.");
		}
	} else {
		Serial.println("Not initialized.");
	}
	return 0;		// Errors! Return 0
}

	
	
//
// *********** callback handlers ***********
//


void  WiFiObj::handleDefaultConnect(void* pvMsg) {

	tstrM2MDefaultConnResp* pstrDefaultConnResp; 
	
	pstrDefaultConnResp = (tstrM2MDefaultConnResp *)pvMsg;
	if (pstrDefaultConnResp->s8ErrorCode) {
		status = WF_DISCONNECTED;
	}
}


void  WiFiObj::handleCurrentRSSI(void* pvMsg) {
	
	resolve = *((int8_t *)pvMsg);		// Isn't this the same as resolve = *pvMsg; ?
}


// Do we need this now? It's for incomprehensible things.
// It's not complete.
void  WiFiObj::handleProvisionInfo(void* pvMsg) {
	
	tstrM2MProvisionInfo* msg;
	
	Serial.println("Provisioning has been called. It was never completed!");
	msg = (tstrM2MProvisionInfo *)pvMsg;
	if (msg->u8Status == M2M_SUCCESS) {
		memset(ssid, 0, M2M_MAX_SSID_LEN);
		memcpy(ssid, (char *)msg->au8SSID, strlen((char *)msg->au8SSID));
		mode = WF_STA_MODE;
		localIP = 0;
		submask = 0;
		gateway = 0;
		m2m_wifi_connect((char *)msg->au8SSID, strlen((char *)msg->au8SSID),msg->u8SecType, msg->au8Password, M2M_WIFI_CH_ALL);
	} else {
		status = WF_PROVISIONING_FAILED;
		//beginProvision();	-- Needs to be completed.
	}
}


void  WiFiObj::handleConnectionInfo(void* pvMsg) {
	
	tstrM2MConnInfo	*msg = (tstrM2MConnInfo*)pvMsg;

	if (remoteMACAddrPtr) {												// If we're pointing at an address.
		for(int i = 0; i < 6; i++) {								// reverse copy the remote MAC
			remoteMACAddrPtr[i] = msg->au8MACAddress[5-i];
		}
		remoteMACAddrPtr = NULL;
	}
	strcpy((char *)ssid, msg->acSSID);	// I guess this bit's for us?
}


void  WiFiObj::handleGetSystemTime(void* pvMsg) {
	
	if (resolve != 0) {			// Is it carrying an address? It must be.
		memcpy((tstrSystemTime *)resolve, pvMsg, sizeof(tstrSystemTime));
		resolve = 0;
	}
}


// This needs closeSockets() written and added to it.
void  WiFiObj::handleConnectionChange(void* pvMsg) {
	
	tstrM2mWifiStateChanged* pstrWifiState;
	
	pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
	if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {	// Looks like we're in!
		if (mode == WF_STA_MODE && !DHCP) {										// I don't really undestand this bit..
			status = WF_CONNECTED;
		} else if (mode == WF_AP_MODE) {
			status = WF_AP_CONNECTED;
		}
	} else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {	// Disconnected?!
		if (mode == WF_STA_MODE) {
			status = WF_DISCONNECTED;
			if (DHCP) {
				localIP = 0;
				submask = 0;
				gateway = 0;
			}
			// Close sockets..
			// Need to add that bit here.
		} else if (mode == WF_AP_MODE) {
			status = WF_AP_LISTENING;
		}
	}
}


void  WiFiObj::handleScanDone(void* pvMsg) {
	
	tstrM2mScanDone* pstrInfo;
	
	pstrInfo = (tstrM2mScanDone *)pvMsg;
	if (pstrInfo->u8NumofCh >= 1) {
		status = WF_SCAN_COMPLETED;
	}
}


void  WiFiObj::handleScanResult(void* pvMsg) {

	tstrM2mWifiscanResult*	msg;
	uint16_t								scan_ssid_len;
	
	msg = (tstrM2mWifiscanResult *)pvMsg;
	scan_ssid_len = strlen((const char *)msg->au8SSID);
	memset(scan_ssid, 0, M2M_MAX_SSID_LEN);
	if (scan_ssid_len) {
		memcpy(scan_ssid, (const char *)msg->au8SSID, scan_ssid_len);
	}
	if (remoteMACAddrPtr) {												// We pointing to a remote address?
		for(int i = 0; i < 6; i++) {								// reverse copy the remote MAC
			remoteMACAddrPtr[i] = msg->au8BSSID[5-i];
		}
	} else {
		//Serial.println("No remote address");
	}
	resolve = msg->s8rssi;
	scanAuth = msg->u8AuthType;
	scanChannel = msg->u8ch;
	status = WF_SCAN_COMPLETED;
}


void  WiFiObj::handleDHCPConfig(void* pvMsg) {
	
	tstrM2MIPConfig* pstrIPCfg;
	
	pstrIPCfg = (tstrM2MIPConfig *)pvMsg;
	if (mode == WF_STA_MODE) {
		localIP = pstrIPCfg->u32StaticIP;
		submask = pstrIPCfg->u32SubnetMask;
		gateway = pstrIPCfg->u32Gateway;
		status = WF_CONNECTED;
	}
}

