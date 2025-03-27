
#include "setup.h"
#include "handlers.h"
#include <lilParser.h>


#define CAN_CS 10          // The chip select for the SPI connection to the CAN board.
#define LIST_MS   30000    // How long before we decide we need to refresh address list?
#define MAX_DEC   10       // Max digits beyond decimal point.


llama2000         llamaBrd;      // The class that inherits netObj, adding our attachmet to hardware.
waterSpeedObj*    knotMeter;     // Handler to read boatspeed.
waterTempObj*     thermometer;   // Handler to read water tempature.
fluidLevelObj*    fuelSender;    // Handler that sends out fuel level messages.
airTempBarometer* barometer;     // Handler to read atmisphereic pressure.
LC_ChatObj*       chatObj;       // Hanlder that implements, with a clone of ourselves, a text message service.

lilParser         cmdParser;              // Toolkit for implementing a command line interface.
bool              gettingDevList;         // Getting a device list takes time. This says we're waiting.
timeObj           devListTimer(LIST_MS);  // A timer to time how long we allow before calling the list "stale".
bool              devListNeedsRefresh;    // This holds the current state of the device list.
netName           ourName;                // We save a copt of our net name when starting up.
byte              ourAddr;                // And our address.
netName           aName;                  // A place to save a net name. (For copy/paste)

enum commands {                           // Our command list declrations.
   noCommand,
   devList,
   setFuel,
   showName,
   showValues,
   showHg,
   showMb,
   findName,
   copyName,
   pasteName,
   changeAddr,
   toggleBugs,
   reset,
   help
};


void setup() {

   // Standardopening of serial port.
   Serial.begin(9600);
   delay(100);
   Serial1.begin(9600);
   delay(100);
   // Creating and adding the message handlers.
   knotMeter   = new waterSpeedObj(&llamaBrd);
   thermometer = new waterTempObj(&llamaBrd);
   fuelSender  = new fluidLevelObj(&llamaBrd);
   barometer   = new airTempBarometer(&llamaBrd);
   chatObj      = new LC_ChatObj(&llamaBrd);
   llamaBrd.addMsgHandler(knotMeter);
   llamaBrd.addMsgHandler(thermometer);
   llamaBrd.addMsgHandler(fuelSender);
   llamaBrd.addMsgHandler(barometer);
   llamaBrd.addMsgHandler(chatObj);

   // Resetting the hardware.
   if (!llamaBrd.begin(CAN_CS)) {
   Serial.println("Starting llama failed!");
      while (1);
   }

   // Associating the commands with typed user commands.
   cmdParser.addCmd(devList,"list");
   cmdParser.addCmd(setFuel,"fuel");
   cmdParser.addCmd(showName,"seename");
   cmdParser.addCmd(showName,"showname");
   cmdParser.addCmd(showName,"name");
   cmdParser.addCmd(showValues,"seevalues");
   cmdParser.addCmd(showValues,"showvalues");
   cmdParser.addCmd(showValues,"values");
   cmdParser.addCmd(showHg,"hg");
   cmdParser.addCmd(showMb,"mb");
   cmdParser.addCmd(findName,"findname");
   cmdParser.addCmd(copyName,"copyname");
   cmdParser.addCmd(copyName,"copy");
   cmdParser.addCmd(pasteName,"paste");
   cmdParser.addCmd(pasteName,"pastename");
   cmdParser.addCmd(changeAddr,"changeaddr");
   cmdParser.addCmd(toggleBugs,"bugs");
   cmdParser.addCmd(reset,"reset");
   cmdParser.addCmd(help,"help");
   cmdParser.addCmd(help,"?");

   // State setup for the program.
   gettingDevList = false;
   devListNeedsRefresh = true;
   ourName.copyName(&llamaBrd);
   ourAddr = llamaBrd.getAddr();

   // And away we go!
   Serial.println("Up and running");
   printHelp();
}

// chatObj->setOutStr(inStr); // Not using the chat thing yet.
  

void loop() {

   char   aChar;
   
   idle();                                                        // Most things we do run in the background. This enables that.
   checkDeviceList();                                             // Special if we are waiting on a fresh device list.
   if (devListTimer.ding()) {                                     // We call the device list stale after the timer expires.
      devListNeedsRefresh = true;                                 // Noting it.
      devListTimer.reset();                                       // Shutting off the timer.
   }                                                              //
   if (Serial.available()) {                                      // If the user has typed a char..
      aChar = Serial.read();                                      // Read and save it.
      Serial.print(aChar);                                        // Echo it back to the serial monitor.
      aChar = tolower(aChar);                                     // Make it all lower case. Makes sorting it out easier.
      switch(cmdParser.addChar(aChar)) {                          // If we find a command, we call a hanlder for it.
         case noCommand       :                          break;   // noCommand just means we've not seen the end of the command string yet.
         case devList         : showDeviceList();        break;   // From here on we call handlers.
         case setFuel         : setFuelLevel();          break;
         case showName        : showDeviceName();        break;
         case showValues      : showBroadcastValues();   break;
         case showHg          : showAirPressure();       break;
         case showMb          : showAirPressureInMb();   break;
         case findName        : findNameFromAddr();      break;
         case copyName        : copyNameFromAddr();      break;
         case pasteName       : pasteNameToSelf();       break;
         case changeAddr      : changeAnAddr();          break;
         case toggleBugs      : showReq = !showReq;      break;
         case reset           : resetNameNAddr();        break;
         case -1              : error(-1);               break;
         case -2              : error(-2);               break;
         case -3              : error(-3);               break;
         default              : printHelp();             break;
      }
   }
}

  
//      ******* Command handlers  *******
//   Not to be confused with message handlers.
// These are user commands. Typiclaly for debug.


// Refresh and start watching for the list to be ready to list out.
void showDeviceList(void) {

   if (!gettingDevList) {
      Serial.println("Refreshing device list.");
      Serial.println("This will take a second or so.");
      Serial.println();
      llamaBrd.refreshAddrList();
      gettingDevList = true;
   }
}


// If we called showDeviceList() this watches the macine to see when it's ready to display.
void checkDeviceList(void) {

   if (gettingDevList) {
      if (!llamaBrd.isBusy()) {
         gettingDevList = false;
         devListNeedsRefresh = false;
         devListTimer.start();
         llamaBrd.showAddrList(true);
      }
   }
}


// Sets in a new value for the fuel level.
void setFuelLevel(void) {

   float value;
   
   if (cmdParser.numParams()==1) {
      value = atof(cmdParser.getNextParam());
      Serial.print("Setting fuel level to ");
      Serial.print(value,1);
      Serial.println(" %");
      fuelSender->setTankType(fuel);
      fuelSender->setLevel(value);
      fuelSender->setCapacity(20);
      fuelSender->newMsg();
   }
}


// Shows our device name.
void showDeviceName(void) {
   

   if (cmdParser.numParams()==0) {
      llamaBrd.showName();
   }
}


// Shows what we are reading on the network using our "handlers".
void showBroadcastValues(void) {

    if (cmdParser.numParams()==0) {
      Serial.print(knotMeter->getSpeed(),1);
      Serial.print("\tKnots\t\t");
      Serial.print(barometer->getInHg(),3);
      Serial.print("\tinHg\t\t");
      Serial.print(thermometer->getTemp(),1);
      Serial.println("\tDeg F.");
    }
}


// Displays the current broadcast air pressure in inHg.
void showAirPressure(void) {

   int   dec;

   dec = 2;
   if (cmdParser.numParams()==1) {
      dec = atoi(cmdParser.getNextParam());
      if (dec<0) {
         dec = 0;
      } else if (dec>MAX_DEC) {
         Serial.println("Oh come on, get real.");
         dec = MAX_DEC;
      }
   }
   Serial.print(barometer->getInHg(),dec);
   Serial.println(" inHg");
}


// Displays the current broadcast air pressure in Mb.
void showAirPressureInMb(void) {

   int   dec;

   dec = 2;
   if (cmdParser.numParams()==1) {
      dec = atoi(cmdParser.getNextParam());
      if (dec<0) {
         dec = 0;
      } else if (dec>MAX_DEC) {
         Serial.println("Oh come on, get real.");
         dec = MAX_DEC;
      }
   }
   
   Serial.print(barometer->getInHg()*33.8639,dec);
   Serial.println(" mb");
}


// Given an network address this diplay's that device's name. If possible.
void findNameFromAddr(void) {
   
   int      addr;
   netName  aName;
   
   if (cmdParser.numParams()==1) {
      addr = atoi(cmdParser.getNextParam());
      aName = llamaBrd.findName(addr);
      Serial.println("Address search result.");
      aName.showName();
   } else {
      Serial.println("looking for one parameter. The address.");
   }
}


// Save the name from a certan addresse's device.
void copyNameFromAddr(void) {
  
   int      addr;
   netName  tempName;
   netName  blankName;
   
   if (cmdParser.numParams()==1) {
      addr = atoi(cmdParser.getNextParam());
      tempName = llamaBrd.findName(addr);
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
void pasteNameToSelf(void) {

   if (cmdParser.numParams()==0) {
      llamaBrd.copyName(&aName);
   }
}


// If ony one paramiter this changes our network address. If two parameters this tells
// the unit at parameter one's address to move to parameter two's address.
void changeAnAddr(void) {

   byte     addr1;
   byte     addr2;
   netName  tempName;
   
   if (cmdParser.numParams()==1) {
      addr1 = atoi(cmdParser.getNextParam());
      llamaBrd.setAddr(addr1);
   } else if (cmdParser.numParams()==2) {
      addr1 = atoi(cmdParser.getNextParam());
      addr2 = atoi(cmdParser.getNextParam());
      Serial.print("Address to move : ");Serial.print(addr1);
      Serial.print("\nTo new location : ");Serial.println(addr2);
      tempName = llamaBrd.findName(addr1);
      llamaBrd.addrCom(&tempName,addr2);
   }
}


// Reset the name and address back to what we started with.
void resetNameNAddr() {
   
   if (cmdParser.numParams()==0) {
      llamaBrd.setAddr(ourAddr);
      llamaBrd.copyName(&ourName);
   }
}


// Show the error.
void error(int errNum) {

   
   switch(errNum) {
      case PARSE_ERR    : Serial.println("Something went wrong, I can't parse that. Type ? for help.");  break;
      case CONFIG_ERR   : Serial.println("I think I ran out of RAM during startup.");                    break;
      case PARAM_ERR    : Serial.println("The Param buffer is too small for that string.");              break;
   }
}


// Like it says, prints help text.
void printHelp(void) {
   
   Serial.println();
   Serial.println("                             The list of commands available.");
   Serial.println("           ----------------------------------------------------------------------");
   Serial.println("list - Gives a list of the connected devices on the network.");
   Serial.println("fuel followed by a number 0..100 - sets the percentage of fuel to be broadcast.");
   Serial.println("seeName,showName or just name - This will display our device name.");
   Serial.println("seeValues, showValues or just values - Shows the broadcast information we can read");
   Serial.println("Hg - Shows the current broadcast barometer reading in inHg.");
   Serial.println("Mb - Shows the current broadcast barometer reading in mb.");
   Serial.println("findName folowed by a network address - If found, this returns the name of the item at that address.");
   Serial.println("copyName folowed by a network address - If found, copies the name of the item at that address.");
   Serial.println("pasteName or paste - Changes our name to the last one we copied.");
   Serial.println("changeAddr folowed by one value - Changes our address to that value.");
   Serial.println("changeAddr folowed by two values - tells the device at the first address to chnage to the second value.");
   Serial.println("bugs - Toggle whatever bug tracking messages currently in there on or off.");
   Serial.println("reset - This resets your name and address to what it was when the program started.");
   Serial.println("help, or ? - Well that's this. The command list.");
   Serial.println("           ----------------------------------------------------------------------");
   Serial.println();
}
