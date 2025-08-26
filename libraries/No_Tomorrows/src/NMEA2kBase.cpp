#include <NMEA2kBase.h>
#include <EEPROM.h>
 
#define LC_MANF		35    // Left coast's manufactur's # (I made it up.) As in J/35.
#define LLAMA_CS     10    // llama board chip select pin.
#define LLAMA_RST    6     // llama board reset pin.
#define LLAMA_INT		2     // llama board inturrupt pin. (currently not used.)
#define LIST_MS      30000 // How long before we decide we need to refresh the device list?



// Create and pass in some necessary setup values. 
NMEA2kBase::NMEA2kBase(uint32_t inDeviceID,byte inDeviceClass,byte inDeviceFunct) {
	
	deviceID		= inDeviceID;			// These three are just to hold the values
	deviceClass	= inDeviceClass;		//  'till used in setupName().
	deviceFunct	= inDeviceFunct;		//
	
	devListTimer = new timeObj(LIST_MS);
   gettingDevList = false;
   devListNeedsRefresh = true;
}


// And we pull everything apart to close up shop.
NMEA2kBase::~NMEA2kBase(void) {

	if (devListTimer) {
		delete(devListTimer);
		devListTimer = NULL;
	}
}


// To be called in the .ino's setup. Actually this will probably be inherited, called then
// extended by whatever function this is used for.	
void NMEA2kBase::setup(void) {

	Serial.begin(9600);
	Serial.println("Waiting for 5 sec. before booting up the screen.");
	delay(5000);
	llamaBrd = new llama2000(LLAMA_RST,LLAMA_INT);
	if (!llamaBrd) {
		Serial.println("Can not allocate llama board.");
		while(1);
	}
	setupName();
	if (!addNMEAHandlers()) {
		Serial.println("Can not allocate NMEA2k Handlers.");
		while(1);
	}
	if (!llamaBrd->begin(getInitialAddr(),arbitraryConfig,LLAMA_CS)) {
      Serial.println("Starting llama board failed!");
      while (1);
   }
   addCommands();
}


// This sets up a typical name from passed earlier passed in values. You can override and
// rewrite it, if it doesn't suit your purposes.
void NMEA2kBase::setupName(void) {

	// Our ID stuff.
   llamaBrd->setID(deviceID);				// Device ID. We make these up. You get 21 bits.
   llamaBrd->setManufCode(LC_MANF);		// This would be assigned to you by NMEA people.
   llamaBrd->setECUInst(0);				// First netObj (Electronic control unit.)
   llamaBrd->setFunctInst(0);				// First display.
   llamaBrd->setFunction(deviceFunct);	// What kinda' thing OF our device class are we?
   llamaBrd->setVehSys(deviceClass);	// We class of thing are we?
   llamaBrd->setSystemInst(0);			// We are the first of our device class.
   llamaBrd->setIndGroup(Marine);		// What kind of machine are we ridin' on?
}
 

// Setup our default command set. Like most of these methods, you can inherit this, call
// it and add your own private commands. 
//
// When doing this, you should start your command list with something like this :
//
//	enum moreComs {
//
//		firstCom = lastCom,
//		nextCom,
//		another
//	};
//
// This will add your command numbers correctly to the initial list of command numbers.
void NMEA2kBase::addCommands(void) {

	cmdParser.addCmd(devList,"list");
   cmdParser.addCmd(showName,"seename");
   cmdParser.addCmd(showName,"showname");
   cmdParser.addCmd(showName,"name");
   cmdParser.addCmd(findName,"findname");
   cmdParser.addCmd(copyName,"copyname");
   cmdParser.addCmd(copyName,"copy");
   cmdParser.addCmd(pasteName,"paste");
   cmdParser.addCmd(pasteName,"pastename");
   cmdParser.addCmd(changeAddr,"changeaddr");
   cmdParser.addCmd(reset,"reset");
   cmdParser.addCmd(help,"help");
   cmdParser.addCmd(help,"?"); 
 };
   
  
// Method to read the initial address out of storage and return it.	
byte NMEA2kBase::getInitialAddr(void) {
 	
 	byte addr;
 	
 	EEPROM.get(0,addr);	// Read in the address.
 	return addr;			// Pass it back.
}


// Change our address to this value. If successful, store this value into long tearm
// storage. It will be used as the default address on next power-up.		
void NMEA2kBase::changeAddress(byte inAddr) {
	
	llamaBrd->setAddr(inAddr);	// Set this as our new address.
	EEPROM.put(0,inAddr);		// Save the new address into long term storage.
}


// Should be inherited then used as function to be called by this master .ino's loop()
// function.
void NMEA2kBase::loop(void) {

	char	aChar;
	int	comVal;
	
	idle();
	checkDeviceList();														// Special if we are waiting on a fresh device list.
	if (Serial.available()) {												// If the user has typed a char..
      aChar = Serial.read();												// Read and save it.
      aChar = tolower(aChar);												// Make it all lower case. Makes sorting it out easier.
      Serial.print(aChar);													// Echo it back to the serial monitor.
      comVal = cmdParser.addChar(aChar);								// Parse this char and see what we get back.
      switch(comVal) {														// If we find a command, we call a hanlder for it.
      	case noCommand		: 									break;	// You know, still parsing.
      	case devList		: showDeviceList();			break;
      	case showName  	: showDeviceName();        break;
      	case findName		: findNameFromAddr();      break;
         case copyName		: copyNameFromAddr();      break;
         case pasteName		: pasteNameToSelf();       break;
         case changeAddr	: changeAnAddr();          break;
         case reset			: resetNameNAddr();        break;
         case -1				: error(-1);					break;
         case -2				: error(-2);					break;
         case -3				: error(-3);					break;
      	default				: checkAddedComs(comVal);	break;	// Possibly a command we know nothing about
      }
   }
}


// Offspring should inherit this to check for extended commands that it adds to this default list.
void NMEA2kBase::checkAddedComs(int comVal) {  }



//										******* Command handlers  *******
//
// Not to be confused with message handlers. These are user commands typed in from the
// serial monitor. Typiclaly for debug.
//


// Call refresh then start watching for the list to be ready to display.
void NMEA2kBase::showDeviceList(void) {

   if (!gettingDevList) {
      Serial.println("Refreshing device list.");
      Serial.println("This will take a second or so.");
      Serial.println();
      llamaBrd->refreshAddrList();
      gettingDevList = true;
   }
}


// If we called showDeviceList() this watches the macine to see when it's ready to display.
void NMEA2kBase::checkDeviceList(void) {

	if (gettingDevList) {
		if (!llamaBrd->isBusy()) {
         gettingDevList = false;
         devListNeedsRefresh = false;
         devListTimer->start();
         llamaBrd->showAddrList(true);
      }
   }
}


// Shows our device name.
void NMEA2kBase::showDeviceName(void) {
   
   if (cmdParser.numParams()==0) {
      llamaBrd->showName();
   }
}



// Given a network address, this diplay's that device's name. If possible.
void NMEA2kBase::findNameFromAddr(void) {
   
   int      addr;
   netName  aName;
   
   if (cmdParser.numParams()==1) {
      addr = atoi(cmdParser.getNextParam());
      aName = llamaBrd->findName(addr);
      Serial.println("Address search result.");
      aName.showName();
   } else {
      Serial.println("looking for one parameter. The address.");
   }
}


// Save the name from a certan addresse's device.
void NMEA2kBase::copyNameFromAddr(void) {
  
   int      addr;
   netName  tempName;
   netName  blankName;
   
   if (cmdParser.numParams()==1) {
      addr = atoi(cmdParser.getNextParam());
      tempName = llamaBrd->findName(addr);
      blankName.clearName(false);
      if (!tempName.sameName(&blankName)) {
         aName.copyName(&tempName);
         Serial.println("Name copied");
         aName.showName();
      } else {
         Serial.println("Name was blank Not copied. Probably the address was not found.");
      }
   } else {
      Serial.println("Was looking for an addre to be passed in.");
   }
}


// Change our name to the copied name.
void NMEA2kBase::pasteNameToSelf(void) {

   if (cmdParser.numParams()==0) {
      llamaBrd->copyName(&aName);
   }
}


// If ony one paramiter this changes our network address. If two parameters this tells
// the unit at parameter one's address to move to parameter two's address.
void NMEA2kBase::changeAnAddr(void) {

   byte     addr1;
   byte     addr2;
   netName  tempName;
   
   if (cmdParser.numParams()==1) {
      addr1 = atoi(cmdParser.getNextParam());
      llamaBrd->setAddr(addr1);
      changeAddress(addr1);
   } else if (cmdParser.numParams()==2) {
      addr1 = atoi(cmdParser.getNextParam());
      addr2 = atoi(cmdParser.getNextParam());
      Serial.print("Address to move : ");Serial.print(addr1);
      Serial.print("\nTo new location : ");Serial.println(addr2);
      tempName = llamaBrd->findName(addr1);
      llamaBrd->addrCom(&tempName,addr2);
   }
}


// Reset the name and address back to what we started with.
void NMEA2kBase::resetNameNAddr() {
   
   if (cmdParser.numParams()==0) {
      llamaBrd->setAddr(ourAddr);
      llamaBrd->copyName(&ourName);
   }
}


// Show the error.
void NMEA2kBase::error(int errNum) {

   switch(errNum) {
      case PARSE_ERR    : Serial.println("Something went wrong, I can't parse that. Type ? for help.");  break;
      case CONFIG_ERR   : Serial.println("I think I ran out of RAM during startup.");                    break;
      case PARAM_ERR    : Serial.println("The Param buffer is too small for that string.");              break;
   }
}


// Like it says, prints help text.
void NMEA2kBase::printHelp(void) {
   
   Serial.println();
   Serial.println("                                      Base commands.");
   Serial.println("           ----------------------------------------------------------------------");
   Serial.println("list - Gives a list of the connected devices on the network.");
   Serial.println("seeName,showName or just name - This will display our device name.");
   Serial.println("findName folowed by a network address - If found, this returns the name of the item at that address.");
   Serial.println("copyName folowed by a network address - If found, copies the name of the item at that address.");
   Serial.println("pasteName or paste - Changes our name to the last one we copied.");
   Serial.println("changeAddr folowed by one value - Changes our address to that value.");
   Serial.println("changeAddr folowed by two values - tells the device at the first address to change to the second value.");
   Serial.println("reset - This resets your name and address to what it was when the program started.");
   Serial.println("help, or ? - Well, that's this. The command list.");
   Serial.println();
}