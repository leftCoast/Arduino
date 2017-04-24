#include "networkScanObj.h"


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



/*
// Knock out old dead networks.
void WiFiObj::cleanList(void) {

	netwkObj* trace;
	netwkObj* death;
	
	if (netList) {
		trace = (netwkObj*)netList->getLast();			// Point to end of list.
		while(trace) {
			trace->ding();
			death = trace;
			if (trace==netList) 
		}
	}
}
*/

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
				if (trace->isMe(&scanData)) {						// Ask each node, "Is this you?"
					trace->setData(&scanData);						// If it is, refresh the node.
					trace = NULL;													// And our work is done here.
				} else if (trace->next==NULL) {					// it wasn't his, and to more nodes.
					newOjb = new netwkObj(&scanData);			// Create the new one.
					newOjb->linkAfter(trace);							// Hook in after trace.
					trace = NULL;													// And again, we are done.
				} else {																// It wasn't his, and there is more nodes? 
					trace = (netwkObj*)trace->next;				// Lets go see!
				}
			}
		}
		//cleanList();
		countList();																// We change, it's small, we recount.
	}
}


// This refreshes the network list.
int8_t networkScanObj::scanNetworks(void) {

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


char* networkScanObj::SSID(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getSSID();
	}
	return NULL;
}


int32_t networkScanObj::RSSI(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getRSSI();
	}
	return 0;
}


uint8_t networkScanObj::encryptionType(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getAuth();
	}
	return 0;
}


uint8_t networkScanObj::channel(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getChannel();
	}
	return 0;
}


uint8_t* networkScanObj::BSSID(uint8_t pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getBSSID();
	}
	return 0;
}


// Someone asked how many networks can we see. 
// This is the result of that action.
void  networkScanObj::handleScanDone(void* pvMsg) {

	lastCount = m2m_wifi_get_num_ap_found();
	status = WF_SCAN_COMPLETED;
}


// The network list is being refreshed. As the data is called
// for, we refresh the list and tell whomever its been handled.
void  networkScanObj::handleScanResult(void* pvMsg) {
	
	memcpy(&scanData,pvMsg,sizeof(scanData));
	updateList();
	task = loadInfo;
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
byte* netwkObj::getBSSID(void)	{ return (byte*)(&(BSSID[0])); }
int netwkObj::getRSSI(void)			{ return RSSI; }
byte netwkObj::getAuth(void)		{ return Auth; }
byte netwkObj::getChannel(void)	{ return channel; }
bool netwkObj::getVisable(void) { return visable; }
		
