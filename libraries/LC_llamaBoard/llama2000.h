#ifndef llama2000_h
#define llama2000_h


#include <idlers.h>
#include <mapper.h>
#include <runningAvg.h>
#include <CAN.h>
#include <SAE_J1939.h>


// ************ llama2000 ************


/*
When you plug into the network, other devices will want to know what/who you are. These
are the setting that tell your network neighbors this information. What you see here is
just a default set put in place as an example. Change to match what you actually are
by doing these calls in your program's setup() function before letting this thing start
running.

setID(0);                        // Device ID. We make these up. You get 21 bits.
setManufCode(0);                 // This would be assigned to you by NMEA people.
setECUInst(0);                   // First netObj (Electronic control unit.)
setFunctInst(0);                 // First depth transducer.
setFunction(DEV_FUNC_GP_TRANS);  // Depth transducer.
                                 // Some spare bit here..
setVehSys(DEV_CLASS_INST);       //  We are an instrument.
setSystemInst(0);                // We are the first of our device class.
setIndGroup(Marine);             // What kind of machine are we ridin' on?
setArbitraryAddrBit(?);          // Will be set when we choose our addressing mode.

*/





// To "attach" the SAE J1939/NMEA2000 prototcal stack to your hardware, you will need to
// create a c++ class inheriting and extending the netObj class of the J1939 library.
// This is an example of such a class.
//
// This example is designed to work with the Left Coast llama2K board.
//
//
// These six functions you need to create and fill out as they are the attachment points
// to your code and CAN BUS hardware.
// In your main program you will create and instance of your class,  not the netObj class.
// In this example we chose to call our derived class the llama2000 class. If you look at
// the example sketch you will see that this is the class we intrface with.

class llama2000 : public netObj {

   public:
   llama2000(int inResetPin,int inIntPin);
   ~llama2000(void);
   
   virtual  bool  begin(byte inAddr,addrCat inAddrCat,int inCS);
   virtual  void  sendMsg(message* outMsg);
   virtual  void  recieveMsg(void);
   virtual  void  idle(void);
   
   protected:
   int     resetPin; // Reset pin, you need this.
   int     intPin;   // inturrupt pin. Optional. Not used here.  
};


#endif
