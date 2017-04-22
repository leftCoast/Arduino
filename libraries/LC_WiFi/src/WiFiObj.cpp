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



// ************************************************
// ******************* netwkObj *******************
// ************************************************


// BEFORE creating one, make sure you have a valid
// data set with MAC address.
netwkObj::netwkObj(tstrM2mWifiscanResult* inData)
	: timeObj(LIST_TIME) {
	
	SSID = NULL; 
	setData(inData);
}


netwkObj::~netwkObj(void) { if (SSID) free(SSID); }
		

// We'll key on the MAC address, seeing its fixed and unique.	
bool netwkObj::isMe(tstrM2mWifiscanResult* inData) {

	for(byte i=0;i<6;i++) {
		if (BSSID[i]!=inData->au8BSSID[i]) {
			return false;
		}
	}
	return true;
}


void netwkObj::setData(tstrM2mWifiscanResult* inData) {
	
	for(byte i=0;i<6;i++) {
		BSSID[i]=inData->au8BSSID[i];
	}
	if (SSID) {
		free(SSID);
	}
	SSID = (char*)malloc(strlen((char*)(inData->au8SSID))+1);
	strcpy(SSID,(char*)(inData->au8SSID));
	RSSI = inData->s8rssi;
	Auth = inData->u8AuthType;
	channel = inData->u8ch;
	visable = true;
	start();	// Reset the timer.
}


char* netwkObj::getSSID(void)		{ return SSID; }
byte* netwkObj::getBSSID(void)	{ return (byte*)(&(SSID[0])); }
int netwkObj::getRSSI(void)			{ return RSSI; }
byte netwkObj::getAuth(void)		{ return Auth; }
byte netwkObj::getChannel(void)	{ return channel; }
bool netwkObj::getVisable(void) { return visable; }
		


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


// Pass back this one on the list. Or NULL if not there.
netwkObj* WiFiObj::getNet(int pos) {

	netwkObj*	trace;
	
	trace = netList;
	while(trace && pos) {
		trace = (netwkObj*)trace->next;
		pos--;
	}
	return trace;
}


// When adding or deleting list items, just recount it.
void WiFiObj::countList(void) {
	
	netwkObj* trace;
	
	numInList = 0;
	trace = netList;														// Point trace at our list again.
	while(trace) {
		trace = (netwkObj*)trace->next;
		numInList++;
	}
}
		
		
// Someone called for a list refresh. So at this time
// we have a fresh load of network data in scanData.
// Its time to update the list with it.
void WiFiObj::updateList(void) {
	
	netwkObj*	newOjb;
	netwkObj*	trace;
	int	sum;
	
	sum = 0;																			// Clear sum
	for(byte i=0;i<6;i++) {
		sum = sum + scanData.au8BSSID[i];						// Add all the bytes into sum.
	}
	if (sum) {																		// If we got a nonzero number? Then ok.
		trace = netList;														// Point trace at our list.
		if (trace==NULL) {													// Special case for empty list.
			newOjb = new netwkObj(&scanData);					// Make a fresh new one.
			netList = newOjb;													// Point the list to it. Done!
		} else {																		// Not so fast mister. If not empty..
			while(trace) {														// While trace != NULL.
				if (trace->isMe(&scanData)) {						// Aske each node, "Is this you?"
					trace->setData(&scanData);						// If it is, refresh the node.
					trace = NULL;													// And our work is done here.
				} else if (trace->next==NULL) {					// it wasn't his, and to more nodes.
					newOjb = new netwkObj(&scanData);			// Crete the new one.
					newOjb->linkAfter(trace);							// Hook in after trace.
					trace = NULL;													// And again, we are done.
				} else {																// it wasn't his, and there is more nodes? 
					trace = (netwkObj*)trace->next;				// Lets go see!
				}
			}
		}
		countList();																// We change, it's small, we recount.
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
	
	timeObj	timeOut(1000);												// Create a timer.
	
	result = 0;																		// Assume faillure.
	if (!init) {																	// Sanity.
		m2m_wifi_handle_events(NULL);								// Clear pending events.
		result = m2m_wifi_req_curr_rssi();					// Send RSSI request.
		if (result==M2M_SUCCESS) {									// We got a good call.
			result = 0;																// Clear result. The handler will fill it.
			timeOut.start();													// Set a timer and spin for answer..
			while (result == 0 && !timeOut.ding()) {	// Spin 'till resolve fills or timeout.
				m2m_wifi_handle_events(NULL);
			}
		}
	}
	return result;																// There you go.
}
	
	
void WiFiObj::macAddress(uint8_t* mac) { 

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


// This refreshes the network list.
int8_t WiFiObj::scanNetworks(void) {

	WiFiStatus 	tmp;
	timeObj			timeout(5000);
	int8_t			returnVal;
	
	returnVal = -1;																													// Assume failure.
	if (init) {
		tmp = status;																													// We save our status.
		if (m2m_wifi_request_scan(M2M_WIFI_CH_ALL) >= 0) { 										// Start scan.
			status = WF_IDLE_STATUS;																						// Wait for scan result.
			timeout.start();																										// Start the timer.
			while (status!=WF_SCAN_COMPLETED && !timeout.ding()) {							// Spin 'till done or out of time.
				m2m_wifi_handle_events(NULL);																			// Let chip do its thing.
			}
			if (numNets>0) {																										// Have at least one?
				timeout.setTime(1000);																						// Reset for quick timeout.			
				for(int i=0;i<numNets;i++) {																			// loop through all of 'em.
					m2m_wifi_req_scan_result(i);																		// Fire off query.
					status = WF_IDLE_STATUS;																				// status ready.
					timeout.start();																								// Start the timer.
					while (status!=SCAN_RESULT_COMPLETE && !timeout.ding()) {				// SCAN_RESULT_COMPLETE->delt with.
						m2m_wifi_handle_events(NULL);																	// Let chip do its thing.
					}
				}
				returnVal = numInList;																										// Success!
			}
		} else {
			Serial.println("m2m_wifi_request_scan() failed.");									// Scan failed. 
		}
		status = tmp;																													// Restore status.
	} else {
		Serial.println("Not initialized.");																		// We were not ready.
	}
	return returnVal;																												// Success or failure flag.
}


char* WiFiObj::SSID(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getSSID();
	}
	return NULL;
}


int32_t WiFiObj::RSSI(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getRSSI();
	}
	return 0;
}


uint8_t WiFiObj::encryptionType(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getAuth();
	}
	return 0;
}


uint8_t WiFiObj::channel(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getChannel();
	}
	return 0;
}


uint8_t* WiFiObj::BSSID(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getBSSID();
	}
	return 0;
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
	
	result = 0;										// Clear it, just in case..
	result = *((int8_t *)pvMsg);	// Message is only one byte.
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
	
	if (result != 0) {			// Is it carrying an address? It must be.
		memcpy((tstrSystemTime *)result, pvMsg, sizeof(tstrSystemTime));
		result = 0;
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


// Someone asked how many networks can we see. 
// This is the result of that action.
void  WiFiObj::handleScanDone(void* pvMsg) {

	numNets = m2m_wifi_get_num_ap_found();
	status = WF_SCAN_COMPLETED;
}


// The network list is being refreshed. As the data is called
// for, we refresh the list and tell whomever its been handled.
void  WiFiObj::handleScanResult(void* pvMsg) {
	
	memcpy(&scanData,pvMsg,sizeof(scanData));
	updateList();
	status = SCAN_RESULT_COMPLETE;
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

