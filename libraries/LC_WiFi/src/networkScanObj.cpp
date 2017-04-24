#include "networkScanObj.h"


enum scanStates {
  
  scannerOff,
  waiting,      // Wating for refresh time.
  waitOnCount,  // Wait for the scan count to arrive.
  loadInfo,     // Time to fire off info load.
  waitOnInfo    // Wait for info load to arrive.
};


networkScanObj* gNetworkScanObj = NULL;



networkScanObj::networkScanObj(void) {
  : timeObj(SLEEP_TIME) {
    
    lastCount = 0;
    scanning(false);
  }
  
  
networkScanObj::~networkScanObj(void) { }
  
  
// Turn scanning on or off.
void networkScanObj::scanning(bool onOff) {
  
    if (onOff) {              // We turning on?
      hookup();               // May be first time, call hookup();
      startScan();            // Don't want to waste time on starup. Kick in the scanner.
      state = waitOnCount;    // Set the correct state to note we're starting a scan.
    } else {                  // We're shutting down?
      state = scannerOff;     // Set state to mask callbacks.
    }
    areScanning = onOff;      // Whatever state we're going into. Note it.
  }


// Our little engine for running in thebackground.
void networkScanObj::idle(void) {
  
  if (scanning) {                               // Are we even on?
    switch (state) {                            // OK, we're on, what state are we in?
      case waiting :                            // Wating for refresh time.
        if (ding()) {                           // If it is time for refresh..
          if (startScan()) {                    // Fire off the scanner to get a count.
            state = waitOnCount;                // Setup to wait for this count..
          }
        }
        break;
      case waitOnCount :                          // Ok, waiting for the scan count to arrive.
        if (scanTimer.ding()) {                   // Has the timer expired?
          debugOut("scan timer timed out.");      // If in debug mode, display the error.
          state = wait;                           // In any case we go back to waiting.
        }
        break;
      case loadInfo :                             // Time to fire off info load.
        if (startInfo()) {
          state = waitOnInfo();
        }
        break;
      case waitOnInfo :
        if (scanTimer.ding()) {                   // Has the timer expired?
          debugOut("info timer timed out.");      // If in debug mode, display the error.
          state = wait;                           // In any case we go back to waiting.
        }
        break;
      default : debugOut("Got a unknown state in networkScanObj::idle()"); break;
    }
  }
}



// Start functions. Kick off an action and set the scanTimer.


// Ask how many networks are out there..
bool networkScanObj::startScan(void) {
  
  int res;
  
  res = m2m_wifi_request_scan(M2M_WIFI_CH_ALL);
  if (res>=0){
    scanTimeout.start();
    return true;
  }
  debugOut("m2m_wifi_request_scan() failed.");
  return false;
}


// We're looking for info on AP x. Start the query.
bool networkScanObj::startInfo(void) {
  
  int res;
  
  res = m2m_wifi_req_scan_result(currentIndex);
  if (res==M2M_SUCCESS) {
    scanTimer.start();
    return true;
  }
  debugOUt("m2m_wifi_req_scan_result() faild.");
  return false;
}



// Handeler callbacks.

// Someone asked how many networks can we see.
// Here is the result of that action.
void  networkScanObj::handleScanDone(void* pvMsg) {
  
  if (state==waitOnCount) {                             // If we're actually waiting for this.
    lastCount = m2m_wifi_get_num_ap_found();            // Grab the count.
    if (lastCount>0) {                                  // If we saw any..
      currentIndex = 0;                                 // Reset the index to read them from.
      status = loadInfo;                                // Fire off the flag to start loading them.
    } else {                                            // Or if we didn't see any..
      start();                                          // Reset our waiting timer.
      state = wiaing;                                   // Basically go back to sleep for a while.
    }
  }
}


// The network list is being refreshed. As the data is called
// for, we refresh the list and tell whomever its been handled.
void  networkScanObj::handleScanResult(void* pvMsg) {
	
    if (state==waitOnInfo) {                      // If we're actually waiting for this.
      memcpy(&scanData,pvMsg,sizeof(scanData));   // Grab the wad of data.
      updateList();                               // Use it to update the list.
      currentIndex++;                             // Bumpcounter to the next item.
      if (currentIndex<lastCount) {               // We still have more to go?
        state = loadInfo;                         // Flag we need another read.
      } else {                                    // No more to go?
        start();                                  // Reset main timer.
        state = waiting;                          // Go back to sleep for a while.
      }
    }
  }


// When adding or deleting list items, just recount it.
void networkScanObj::countList(void) {
	
	netwkObj* trace;
	
	numInList = 0;
	trace = netList;														// Point trace at our list again.
	while(trace) {
		trace = (netwkObj*)trace->next;
		numInList++;
	}
}
		
		
// Someone called for a list refresh. As a result,
// we have a fresh load of network data in scanData.
// Its time to update the list with it.
void networkScanObj::updateList(void) {
	
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


// "Get" funtions.

// Pass back this one on the list. Or NULL if not there.
netwkObj* networkScanObj::getNet(int pos) {
  
	netwkObj*	trace;
	
	trace = netList;
	while(trace && pos) {
		trace = (netwkObj*)trace->next;
		pos--;
	}
	return trace;
}


// No, its not really a string but everyone thinks it is.
// So, we'll just give 'em a string. Not like it matters,
// Typically its just for display. But then you could use
// This as a way of warding off hackers by using all 32
// bytes and placing Nulls inline.
char* networkScanObj::getSSID(byte pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getSSID();
	}
	return NULL;
}


// BSSID, 6 uneque bytes to finger print an AP. Handy.
uint8_t* networkScanObj::getBSSID(byte pos) {
  
	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getBSSID();
	}
	return 0;
}


// RSSI, a way of putting a number to signal quality.
int32_t networkScanObj::getRSSI(byte pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getRSSI();
	}
	return 0;
}


// What type of encryption is the AP using. WEP? WPA?
uint8_t networkScanObj::getEncrypt(byte pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getEncrypt();
	}
	return 0;
}


// Chanel number, here in the US we get 1-11. Asia gets 1-14?
// I guess this is the chanel number that the AP is using.
uint8_t networkScanObj::getChannel(byte pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getChannel();
	}
	return 0;
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
		
