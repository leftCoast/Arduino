#include "WiFiObj.h"

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

// We only get one of each. These are set up when begin() is called.
// They're used as a links for the callback function.
WiFiObj* gWiFi 									= NULL;
networkScanObj* gNetworkScanObj = NULL;

static void wifiCallback(uint8_t u8MsgType, void* pvMsg) {

	if (gWiFi) {																	// Sanity. We have something to talk to?
		switch (u8MsgType) {
			case M2M_WIFI_RESP_DEFAULT_CONNECT		: gWiFi->handleDefaultConnect(pvMsg); break;
			case M2M_WIFI_RESP_CURRENT_RSSI				: gWiFi->handleCurrentRSSI(pvMsg); break;
			case M2M_WIFI_RESP_PROVISION_INFO			: gWiFi->handleProvisionInfo(pvMsg); break;
			case M2M_WIFI_RESP_CONN_INFO					: gWiFi->handleConnectionInfo(pvMsg); break;
			case M2M_WIFI_RESP_GET_SYS_TIME				: gWiFi->handleGetSystemTime(pvMsg); break;
			case M2M_WIFI_RESP_CON_STATE_CHANGED	: gWiFi->handleConnectionChange(pvMsg); break;
			case M2M_WIFI_RESP_IP_CONFLICT				: gWiFi->handleIPConflict(pvMsg); break;
			case M2M_WIFI_RESP_CLIENT_INFO				: gWiFi->handleClientInfo(pvMsg); break;
			case M2M_WIFI_REQ_DHCP_CONF						: gWiFi->handleDHCPConfig(pvMsg); break;
			case M2M_WIFI_REQ_WPS									: gWiFi->handleReqestWPS(pvMsg); break;
			
			case M2M_WIFI_RESP_SCAN_DONE					: gNetworkScanObj->handleScanDone(pvMsg); break;
			case M2M_WIFI_RESP_SCAN_RESULT				: gNetworkScanObj->handleScanResult(pvMsg); break;
			
		}
  }
}


// ************************************************
// ******************* WiFiObj ********************
// ************************************************


WiFiObj::WiFiObj(int cs,int irq, int rst) {


	gi8Winc1501CsPin = cs;
	gi8Winc1501IntnPin = irq;
	gi8Winc1501ResetPin = rst;
	gi8Winc1501ChipEnPin = -1;  // Wire the damn thing to 5V. 

  mode							= WF_RESET_MODE;
  status						= WF_NO_CHIP;
  remoteMACAddrPtr	= NULL;
  netList = NULL;
  numInList = 0;
  init = false;
}


WiFiObj::~WiFiObj(void) {  }


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
      while (status != WF_CONNECTED && status != WF_DISCONNECTED && !timeout.ding()) {
        m2m_wifi_handle_events(NULL); // run callbacks.
      }
      if (status == WF_CONNECTED) {
        success = true;                 // We did it! We are amazing!
      } else {
        mode = WF_RESET_MODE;
        Serial.print("m2m_wifi_connect() timed out.");
      } 
      memset(SSID, 0, M2M_MAX_SSID_LEN);					// Can't use strcpy() here because SSID is not actually a string.
			memcpy(SSID, WifiName, strlen(WifiName));		// I'ts just a buffer people typically use as a string. No '\0'.
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
  
  if (!init) {																								// Once is enough!
		result = nm_bsp_init();                 									// Initialize the WiFi "Basic Service Provider."
		if (result==M2M_SUCCESS) {
			param.pfAppWifiCb = wifiCallback;         							// Register the callback.
			result = m2m_wifi_init(&param);              						// See if it registers.
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
		return SSID;
	}
	else {
		return 0;
	}
}


uint32_t WiFiObj::getLocalIP(void) { return localIP; }


int32_t WiFiObj::getRSSI(void) {
	
	timeObj	timeOut(1000);
	int			savedStat;
	int			res;
	
	savedStat = status;																	// Save status.
	RSSI = 0;																						// Assume faillure.
	if (!init) {																				// Sanity.
		m2m_wifi_handle_events(NULL);											// Clear pending events.
		status = 0;																				// Clear status. The handler will fill it.
		res = m2m_wifi_req_curr_rssi();										// Send RSSI request.
		if (res==M2M_SUCCESS) {														// We sent a good call?
			timeOut.start();																// Set a timer and..
			while (status!=RSSI_DONE && !timeOut.ding()) {	// Spin 'till RSSI fills or timeout.
				m2m_wifi_handle_events(NULL);
			}
		}
		status = savedStat;
	}
	return RSSI;																// There you go.
}
	
	
void WiFiObj::getMACAddr(uint8_t* mac) { 

	m2m_wifi_get_mac_address(mac);
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



//
// *********** callback handlers ***********
//


void  WiFiObj::handleDefaultConnect(void* pvMsg) {

	tstrM2MDefaultConnResp* msg; 
	
	msg = (tstrM2MDefaultConnResp *)pvMsg;	// Look through the template.
	if (msg->s8ErrorCode) {									// I guess if we get anything in s8ErrorCode, its disconnected.
		status = WF_DISCONNECTED;
	}
}


void  WiFiObj::handleCurrentRSSI(void* pvMsg) {
	
	RSSI = *((int8_t *)pvMsg);	// Message is only one byte.
	status = RSSI_DONE;
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
	
	tstrM2MConnInfo* msg;
	
	msg = (tstrM2MConnInfo*)pvMsg;
	memcpy(SSID,msg->acSSID,M2M_MAX_SSID_LEN);			// There we gt it.
	memcpy(connectIPAddr,msg->au8IPAddr,4);					// Uh huh, and that.
	memcpy(remoteMACAddr,msg->au8MACAddress[i],6);	// That too.
	RSSI = msg->s8RSSI;															// It was just laying there.
	Auth = msg->u8SecType;													// What? Leave some? no..	
}


void  WiFiObj::handleGetSystemTime(void* pvMsg) {
	
	if (result != 0) {			// Is it carrying an address? It must be.
		memcpy((tstrSystemTime *)result, pvMsg, sizeof(tstrSystemTime));
		result = 0;
	}
}

// uint8 	__PAD16__ [2]		The padding.
// uint8 	u8CurrState			Current Wi-Fi connection state
// uint8 	u8ErrCode				Error type review tenuM2mConnChangedErrcode


// This needs closeSockets() written and added to it.
void  WiFiObj::handleConnectionChange(void* pvMsg) {
	
	tstrM2mWifiStateChanged* msg;
	
	msg = (tstrM2mWifiStateChanged *)pvMsg;
	if (msg->u8CurrState == M2M_WIFI_CONNECTED) {	// Looks like we're in!
		if (mode == WF_STA_MODE && !DHCP) {					// I don't really undestand this bit..
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

// uint32 	u32DhcpLeaseTime
// uint32 	u32DNS
// uint32 	u32Gateway
// uint32 	u32StaticIP
// uint32 	u32SubnetMask

void  WiFiObj::handleDHCPConfig(void* pvMsg) {
	
	tstrM2MIPConfig* pstrIPCfg;
	
	pstrIPCfg = (tstrM2MIPConfig *)pvMsg;
	if (mode == WF_STA_MODE) {
		leaseTime = pstrIPCfg->u32DhcpLeaseTime;
		DNS				= pstrIPCfg->u32DNS;
		localIP = pstrIPCfg->u32StaticIP;
		submask = pstrIPCfg->u32SubnetMask;
		gateway = pstrIPCfg->u32Gateway;
		status = WF_CONNECTED;
	}
}


// Not sure. I think if we want a WPS connection, they get our WPS info this way.
void  WiFiObj::handleReqestWPS(void* pvMsg) { Serial.println("There sayin' they want WPS info. What do I do?"); }

// Typically from a failed call to m2m_wifi_set_static_ip().
// Setting yourself to a known address.
void  WiFiObj::handleIPConflict(void* pvMsg); { Serial.println("There sayin' there's an IP conflict. What do I do?"); }

// Documentation says this ain't supported yet.
void  WiFiObj::handleClientInfo(void* pvMsg); { Serial.println("There sayin' they want client info. What do I do?"); }


