#include "netScanner.h"


#define LIST_TIME   20000	// The're allowed this much time with no signal.
#define SLEEP_TIME  1000  // This much time between scans.
#define	SCAN_TIME		10000	// Time for scanning the lists.
#define	INFO_TIME		1000	// Time to retrieve the scanned info per AP.

// Wanted netScanner to inherit from idler but both were list objects.
// So this'll do the trick.
scanKicker::scanKicker(netScanner* inScannner) { scanner = inScannner; }				
scanKicker::~scanKicker(void) { }	
void scanKicker::idle(void) { scanner->idle(); }
	

// The global of us that is used by the callback function.
netScanner* gNetworkScanObj = NULL;



netScanner::netScanner(void)
  : timeObj(SLEEP_TIME) {
    
    ourKicker = new scanKicker(this);
    lastCount = 0;		// Count of new data.
    numInList = 0;		// Count of total in list.
    scanning(false);
  }
  
  
netScanner::~netScanner(void) {

		delete(ourKicker);
		while(next) {
			delete(next);
		}
	}
  
  
// Turn scanning on or off.
void netScanner::scanning(bool onOff) {

    if (onOff) {              // We turning on?
      ourKicker->hookup();    // May be first time, call hookup();
      startScan();            // Don't want to waste time on starup. Kick in the scanner.
      state = waitOnCount;    // Set the correct state to note we're starting a scan.
    } else {                  // We're shutting down?
      state = scannerOff;     // Set state to mask callbacks.
    }
  }


// Our little engine for running in the background.
// Actually, its run by ourKicker, in the background.
void netScanner::idle(void) {

	
  if (state!=scannerOff) {												// Are we even on?
  	m2m_wifi_handle_events(NULL);
    switch (state) {                            	// OK, we're on, what state are we in?
      case waiting :                            	// Wating for refresh time.
        if (ding()) {                           	// If it is time for refresh..
          if (startScan()) {                    	// Fire off the scanner to get a count.
            state = waitOnCount;                	// Setup to wait for this count..
          }
        }
        break;
      case waitOnCount :                          	// Ok, waiting for the scan count to arrive.
        if (scanTimer.ding()) {                   	// Has the timer expired?
          Serial.println("scan timer timed out.");	// If in debug mode, display the error.
          state = waiting;                        	// In any case we go back to waiting.
        }
        break;
      case loadInfo :                             	// Time to fire off info load.
        if (startInfo()) {
          state = waitOnInfo;
        }
        break;
      case waitOnInfo :
        if (scanTimer.ding()) {                   	// Has the timer expired?
          Serial.println("info timer timed out.");  // If in debug mode, display the error.
          state = waiting;                        	// In any case we go back to waiting.
        }
        break;
      default : Serial.println("Got a unknown state in netScanner::idle()"); break;
    }
  }
}



// "Start functions". Kick off an action and set the scanTimer.


// Ask how many networks are out there..
bool netScanner::startScan(void) {
  
  int res;
  
  res = m2m_wifi_request_scan(M2M_WIFI_CH_ALL);
  if (res>=0){
    scanTimer.setTime(SCAN_TIME);
    return true;
  }
  Serial.println("m2m_wifi_request_scan() failed.");
  return false;
}


// We're looking for info on AP x. Start the query.
bool netScanner::startInfo(void) {
  
  int res;
  
  res = m2m_wifi_req_scan_result(currentIndex);
  if (res==M2M_SUCCESS) {
    scanTimer.setTime(INFO_TIME);
    return true;
  }
  Serial.println("m2m_wifi_req_scan_result() faild.");
  return false;
}



// Handeler callbacks.

// Someone asked how many networks can we see.
// Here is the result of that action.
void  netScanner::handleScanDone(void* pvMsg) {
  
  if (state==waitOnCount) {                             // If we're actually waiting for this.
    lastCount = m2m_wifi_get_num_ap_found();            // Grab the count.
    if (lastCount>0) {                                  // If we saw any..
      currentIndex = 0;                                 // Reset the index to read them from.
      state = loadInfo;																	// Fire off the flag to start loading them.
    } else {                                            // Or if we didn't see any..
      start();                                          // Reset our waiting timer.
      state = waiting;                                  // Basically go back to sleep for a while.
    }
  }
}


// The network list is being refreshed. As the data is called
// for, we refresh the list and tell whomever its been handled.
void  netScanner::handleScanResult(void* pvMsg) {
	
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


// Knock out the expired nodes.
void netScanner::trimList(void) {

	netwkObj* trace;
	netwkObj* deathPtr;
	
	trace = (netwkObj*)next;														// Point trace at our list again.
	while(trace) {
		if (trace->ding()) {
			deathPtr = trace;
			trace = (netwkObj*)trace->next;
			delete deathPtr;
		} else {
			trace = (netwkObj*)trace->next;
		}
	}
}


// When adding or deleting list items, just recount it.
void netScanner::countList(void) {
	
	netwkObj* trace;
	
	numInList = 0;
	trace = (netwkObj*)next;														// Point trace at our list again.
	while(trace) {
		trace = (netwkObj*)trace->next;
		numInList++;
	}
}
		
		
// Someone called for a list refresh. As a result,
// we have a fresh load of network data in scanData.
// Its time to update the list with it.
void netScanner::updateList(void) {
	
		netwkObj*	newOjb;
		netwkObj*	trace;
		bool			done;
	
		if (next==NULL) {														// Special case, empty list.
			newOjb = new netwkObj(&scanData);					// Create the new one.
			newOjb->linkAfter(this);									// Link it in.
		} else {																		// Looks like we have a list.
			trace = (netwkObj*)next;									// Point trace at our list.
			done = false;
			while(!done) {
				if (trace->isMe(&scanData)) {						// Ask each node, "Is this you?"
					trace->setData(&scanData);						// If it is, refresh the node.
					done = true;													// And our work is done here.
				} else if (trace->next==NULL) {					// it wasn't his, and to more nodes.
					newOjb = new netwkObj(&scanData);			// Create the new one.
					newOjb->linkAfter(trace);							// Hook in after trace.
					done = true;													// And again, we are done.
				} else {																// It wasn't his, and there is more nodes? 
					trace = (netwkObj*)trace->next;				// Lets go see!
				}
			}
		}
		trimList();
		countList();																// We change, it's small, we recount.
	}


// "Get" funtions.


// How many do we got?
byte netScanner::getCount(void) { return numInList; }


// Pass back this one on the list. Or NULL if not there.
netwkObj* netScanner::getNet(byte pos) {
  
	netwkObj*	trace;
	
	trace = (netwkObj*)next;
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
char* netScanner::getSSID(byte pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getSSID();
	}
	return NULL;
}


// BSSID, 6 uneque bytes to finger print an AP. Handy.
byte* netScanner::getBSSID(byte pos) {
  
	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getBSSID();
	}
	return 0;
}


// RSSI, a way of putting a number to signal quality.
int netScanner::getRSSI(byte pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getRSSI();
	}
	return 0;
}


// What type of encryption is the AP using. WEP? WPA?
byte netScanner::getEncrypt(byte pos) {

	netwkObj*	trace;
	
	trace = getNet(pos);
	if (trace) {
		return trace->getEncrypt();
	}
	return 0;
}


// Chanel number, here in the US we get 1-11. Asia gets 1-14?
// I guess this is the chanel number that the AP is using.
byte netScanner::getChannel(byte pos) {

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


// Make me a fresh node..
netwkObj::netwkObj(tstrM2mWifiscanResult* inData)
	: timeObj(LIST_TIME) {
	
	SSID = NULL; 
	setData(inData);
}


netwkObj::~netwkObj(void) { if (SSID) free(SSID); }
		

// We'll key on the BSSID, seeing its fixed and unique.
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
	encrypt = inData->u8AuthType;
	channel = inData->u8ch;
	start();	// Reset the timer.
}


char*	netwkObj::getSSID(void)			{ return SSID; }
byte* netwkObj::getBSSID(void)		{ return (byte*)(&(BSSID[0])); }
int		netwkObj::getRSSI(void)			{ return RSSI; }
byte	netwkObj::getEncrypt(void)	{ return encrypt; }
byte	netwkObj::getChannel(void)	{ return channel; }
		
