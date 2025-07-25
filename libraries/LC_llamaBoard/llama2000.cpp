#include <llama2000.h>
#include <resizeBuff.h>


// ************ llama2000 ************


llama2000::llama2000(int inResetPin, int inIntPin)
  : netObj() {

  resetPin = inResetPin;  // We have a reset pin.
  intPin   = inIntPin;    // And an interrupt pin. Not used in this version.
}


llama2000::~llama2000(void) {  }


bool llama2000::begin(byte inAddr,addrCat inAddrCat,int inCSPin) {


   netObj::begin(inAddr,inAddrCat);		// Here's our default address and address category.
   pinMode(resetPin, OUTPUT);				// Setup our reset pin.
   delay(50);									// Sit for a bit..
   digitalWrite(resetPin, LOW);			// Set reset low.
   delay(50);									// Set for a bit, again.
   digitalWrite(resetPin, HIGH);			// Flick it high and leave it there.
   hookup();									// Hook ourselves into the ideler queue.
   return CAN.begin(500E3);				// Fire up the hardware.
}



// NOTE : message data size. At this level the messages can ONLY be up to eight bytes.
// CAN bus has this as a fixed limit. And, the odds of receiving or picking up one to send
// that is larger than eight bytes, at this point in the code, is pretty small.
//
// But, that all being said. When writing your handlers for incoming and outgoing messages
// this limit is lifted. You can both send and receive message of arbitrary size. How?
// From the "user" end (Handlers), where messages are generated by you, large ones are
// automatically broken down into eight or less data bytes, and sent as a stream of
// smaller messages through here.
//
// Likewise, when a larg message is addressed to "us", it will come in through here as a
// stream of smaller messages. Behind the scenes it is reassembled. Then it is handed off
// to us in the handlers, as a complete message with a larger than eight byte data pack.


// The transmitting gateway from NEMA2000/SAE J1939 protocol to the actual CAN bus hardware.
void llama2000::sendMsg(message* outMsg) {

   uint32_t CANID;
   int    numBytes;
   
   if (outMsg) {                                // Sanity, if this is not null.
      numBytes = outMsg->getNumBytes();         // Read the number of data bytes we have here.
      if (numBytes <= 8) {                      // More sanity, we can only send eight data bytes over CAN bus.
         CANID = outMsg->getCANID();            // Grab the formatted CAN ID.
         CAN.beginExtendedPacket(CANID);        // Hardware wants this CAN ID.
         for (int i = 0; i < numBytes; i++) {   // For each data byte..
            CAN.write(outMsg->getDataByte(i));  // Send the data byte out.
         }                                      //
         CAN.endPacket();                       // All done, close up shop.
      }
   }
}


// The receiving gateway from NEMA2000/SAE J1939 protocol to the actual CAN bus hardware.
void llama2000::recieveMsg(void) {

   message newMsg;
   int   i;
   
   if (CAN.parsePacket()) {                                    // If we got a parsable packet..
      newMsg.setCANID(CAN.packetId());                         // Let our message objet Decode and store the packet for us.
      newMsg.setNumBytes(CAN.packetDlc());                     // Read and set up message's data buffer. (Up to 8 Bytes)
      i = 0;                                                   // Starting at zero..
      while (CAN.available() && i < newMsg.getNumBytes()) {    // While we have a byte to read and a place to put it..
         newMsg.setDataByte(i, CAN.read());                    // Read and store the byte into the message.
         i++;                                                  // Bump of the storage index.
      }																			//
      incomingMsg(&newMsg);                                    // All stored, let our netObj deal with it.
   }
}


// This machine runs on idle time. The way it is currently written here it automatically
// polls for messages every time idle() is called. Typically through your loop() function.
void llama2000::idle(void) {

   netObj::idle();
   recieveMsg();
}
