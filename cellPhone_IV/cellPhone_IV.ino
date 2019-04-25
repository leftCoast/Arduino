#include <idlers.h>
#include <lists.h>
#include <quickCom.h>
#include "cellCommon.h"

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7

#define COM_BUFF_BYTES  255
#define PN_BUFF_BYTES    20
#define UNS_BUFF_BYTES   20   // Unsolicited messages. Like caller ID.

class LC_fona : public Adafruit_FONA {

  public:
    LC_fona(void);
    virtual ~LC_fona(void);

    void  flushSweep(void);
    bool  setParam(FONAFlashStringPtr send, int32_t param);

    FONAFlashStringPtr ok_reply;
};


LC_fona::LC_fona(void)
  : Adafruit_FONA(FONA_RST) {
  ok_reply = F("OK");
}


LC_fona::~LC_fona(void) {  }


void LC_fona::flushSweep(void) { flushInput(); }


bool LC_fona::setParam(FONAFlashStringPtr send, int32_t param) {

  return sendCheckReply(send, param, ok_reply);
}


SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
//Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

LC_fona fona = LC_fona();

bool          FONAOnline;
qCSlave       ourComObj;
byte          comBuff[COM_BUFF_BYTES];  // Buffer for all comunication.
byte          pnBuff[PN_BUFF_BYTES];    // Buffer for current phone number.
byte          unsBuff[UNS_BUFF_BYTES];  // Buffer for unsolicited bytes.
byte          unsIndex;
bool          sweepState;
byte*         comPtr;
bool          havePN;


void setup() {

  havePN = false;                                 // We do not yet have a phone number to text to.
  pinMode(0, INPUT);                              // Adafruit says to do this. Otherwise it may read noise.
  pinMode(13, OUTPUT);                            // Our one and only debugging tool.
  unsIndex = 0;                                   // Init unsolicited(sweep) buff index.
  unsBuff[0] = 0;                                 // Reset the unsolicited(sweep) buffer.
  ourComObj.begin(comBuff, COM_BUFF_BYTES, 9600); // Buff, bytes & Baud. Setup for talking to the GUI.
  checkComErr();                                  // Read comunications error. On error we stop here and blink.
  pinMode(FONA_RST, OUTPUT);                      // Used for resetting the FONA.
  FONAOnline = false;                             // Not ready yet..
  resetFONA();                                    // Hit reset, see if it'll come online.
}


// Single blink..
void aBlink(void) {
  
  digitalWrite(13, HIGH);
  delay(75);
  digitalWrite(13, LOW);
}


// String of blinks.
void blink(int numBlinks) {

  if (numBlinks == 1) {
    aBlink();
  } else {
    for (int i = 1; i < numBlinks; i++) {
      aBlink();
      delay(150);
    }
    aBlink();
  }
}


// If we get a comunications error, we give up and just loop blinking
// the error number on our LED.
void checkComErr() {
  
  int error;
  
  error = ourComObj.readErr();  // Read the error.
  if (!error == NO_ERR) {       // If we had an error..
    while (true) {              // Loop forever..
      digitalWrite(13, HIGH);   // Light on.
      delay(1000);              // Wait a second.
      digitalWrite(13, LOW);    // Light off.
      delay(1000);              // Wait a second.
      blink(error);             // Blink out the error number.
      delay(1000);              // Wait a second.
    }
  }
}


void loop() {
  
  char  aChar;
  
  idle();                                                       // Let anyone that's idleing have some time.
  checkComErr();                                                // Check for errors.
  if (ourComObj.haveBuff()) {                                   // If we have a complete command..
    comPtr = ourComObj.getComBuff();                            // Lets point at the command character.
    switch (comPtr[0]) {                                        // Using the command character, decide our action.
      case getStatus    : doStatus(comPtr);     break;          // Pack up a status struct and send it back.
      case makeCall     : doCall(comPtr);       break;          // Make a call. PN is to be packed in the buffer.
      case hangUp       : doHangUp(comPtr);     break;          // Hang up.
      case setCurrentPN : doSetPN(comPtr);      break;          // Set the phone number we plan to have a dialog with.
      case sendSMS      : doSendSMSMsg(comPtr); break;          // Send a message to the current phone number.
      case getSMS       : doGetSMSMsg(comPtr);  break;          // Read & delete an incoming message.
      case pickUp       : doPickUp(comPtr);     break;          // If there's a incoming call, pick it up.
      case touchTone    : doTouchTone(comPtr);  break;          // Start or stop a touch tone.
      case sweepUNS     : doSweepUNS(comPtr);   break;          // Wrap up the first 20 bytes we saved and send them off.
      case callerID     : doCallID(comPtr);     break;          // Turn caller ID on/off. Comes in on every ring. uugh!
      default           : break;                                // Who knows? Some sort of nonsense.
    }
  } else {                                                      // If we don't have a command..
    if (sweepState) {                                           // If Mr user wants to track unsolicited(sweep) data.
      while(fonaSS.available()&&unsIndex<(UNS_BUFF_BYTES-1)) {  // If some stray bytes float in..
        aChar = fonaSS.read();                                  // Grab a byte.`
        if (isprint(aChar)) {                                   // If the char is printable..
          unsBuff[unsIndex] = aChar;                            // Save it away.
          unsIndex++;                                           // Increment the index.
        } else {                                                // Not printable..
          unsIndex = 0;                                         // We toss everything and restart. Looking for a string of 20 printable chars.
        }
      }
      if (unsIndex==UNS_BUFF_BYTES-1) {                         // If we've maxed out the buffer..
        unsBuff[unsIndex] = 0;                                  // Pop in a null as a end of string marker.`  
        fona.setParam(F("AT+CLIP="),0);                         // Shut off the callerID stuff.
        sweepState = false;                                     // Stop looking.
      }
    }
  }
}


// *************************************************
// ******************* handlers ********************
// *************************************************

void resetFONA(void) {

  FONAOnline = false;
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  digitalWrite(FONA_RST, LOW);
  delay(10);
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  fonaSS.begin(4800);                           // For talking to the FONA.
  FONAOnline = fona.begin(fonaSS);              // Able to fire up the FONA.
  fona.setAudio(FONA_EXTAUDIO);                 // Use speaker, not headseat.
  fona.setVolume(35);                           // This is the audio volume.
  fona.setParam(F("AT+CRSL="), 60);             // Set ringer volume.
  fona.setParam(F("AT+CALS="), 0);              // Set 3rd ringtone. 0..19
  sweepState = fona.setParam(F("AT+CLIP="),1);  // Send caller ID after each ring.
  fona.enableNetworkTimeSync(true);             // See if it works..
}


void doStatus(byte* buff) {

  cellStatus* statPtr;
  int         error;
  int         numSMS;

  statPtr = (cellStatus*)buff;                                      // Basically lay a template over the com buffer.
  error = 0;                                                        // No erors yet.
  statPtr->FONAOnline = FONAOnline;                                 // First thing. Is FONA online or not?
  if (statPtr->FONAOnline) {                                        // If FONA is online..
    if (!fona.getBattVoltage(&statPtr->batteryVolts)) {             // Attempt to read the battery voltage..
      statPtr->batteryVolts = 0;                                    // If there was an error, send out 0 volts.
      error = error | B00000010;                                    // Set the error bit for our error flag.
    }
    if (!fona.getBattPercent(&statPtr->batteryPercent)) {           // Attmept to read the battery percent value.
      statPtr->batteryPercent = 0;                                  // If there was an error, send out 0 percent.
      error = error | B00000100;                                    // And set the error bit for our error flag.
    }
    statPtr->RSSI = fona.getRSSI();                                 // Read RSSI, no error checking available here.
    statPtr->networkStat = (networkStatus)fona.getNetworkStatus();  // Read network status. Same, no error checking.
    statPtr->volume = fona.getVolume();                             // Volume.
    statPtr->callStat = (callStatus)fona.getCallStatus();           // Call status, no error checking.
    numSMS = fona.getNumSMS();                                      // Number SMS(s). This one gives bogus numbers at times.
    if (numSMS < 0) {                                               // Try to catch the errors but might not work.
      statPtr->numSMSs = 0;                                         // Bogus readings get zero.
      error = error | B00001000;                                    // Flag the error.
    }
    statPtr->numSMSs = numSMS;                                      // Send on what we found.
    fona.getTime(statPtr->networkTime, 23);                         // Write out the network time string.
  } else {
    error = B00000001;                                              // If the FONA was offline, flag the eror.
  }
  statPtr->errByte = error;                                         // And finally the error flag goes in.
  ourComObj.replyComBuff(sizeof(cellStatus));                      // Send the info on its way.
}


void doPickUp(byte* buff) {

  if (fona.pickUp()) {
    buff[0] = 0;                   // We'll pass a 0 for no error. (We got the call.)
  } else {
    buff[0] = 1;                   // We'll pass a 1 for error. (We missed the call.)
  }
  ourComObj.replyComBuff(1);
}


void doCall(byte* buff) {

  if (fona.callPhone((char*)&buff[1])) {
    buff[0] = 0;                   // We'll pass a 0 for no error.
  } else {
    buff[0] = 1;                   // We'll pass a 1 for error.
  }
  ourComObj.replyComBuff(1);
}


void doHangUp(byte* buff) {

  timeObj hangTimer(2000);

  hangTimer.start();                  // Start our time out timer.
  buff[0] = 0;                        // Lets be ready for success.
  while (!hangTimer.ding()) {         // While we have time..
    if (fona.getCallStatus() == 0) {  // If its not even connected..
      ourComObj.replyComBuff(1);      // Send the successful result.
      return;                         // We're out of here!
    }
    if (fona.hangUp()) {              // Connected? Ok, see if we can hang up.
      ourComObj.replyComBuff(1);      // If successful, send back the result.
      return;                         // And we're done.
    }
  }
  buff[0] = 1;                        // Blast! We timed out.
  ourComObj.replyComBuff(1);          // Send back the error.
}


// Calling program sends us a c string of a packed phone number. Save it to
// our pnBuff to be used for outgoing SMS texts.
void doSetPN(byte* buff) {

  char numChars;

  numChars = strlen((char*)&buff[1]) + 1;   // Lets see just how many bytes is this number?
  if (numChars <= PN_BUFF_BYTES) {          // If we have enough room for it..
    strcpy(pnBuff, (char*)&buff[1]);        // Its a c string so just copy it over.
    buff[0] = 0;                            // Success! So we send, 0. As in zero errors.
    havePN = true;                          // Flag saying "we have a phone number to text to".
  } else {                                  // else not enough room?
    buff[0] = 1;                            // Fail! We had one error, its just to big!
  }
  ourComObj.replyComBuff(1);                // Send back our result.
}


void doSendSMSMsg(byte* buff) {

  if (havePN) {                                 // Sanity, do we even have a phone number?
    if (fona.sendSMS(pnBuff, (char*)&buff[1])) { // Let the FONA code have a go at sending the message.
      buff[0] = 0;                              // Success, 0 errors.
    } else {                                    // else FONA didn't like it for some reason.
      buff[0] = 1;                              // Note the error.
    }
  } else {                                      // We don't have a phone number to text to.
    buff[0] = 2;                                // Note the error.
  }
  ourComObj.replyComBuff(1);                    // Send back our result.
}


// First incoming byte was the command, second byte is the index of the
// message we're after. When we pack our reply it will be error byte
// then two c-strings. First is sending phone number, second is the text
// message itself. Once a message is sent, it's deleted from the SIM chip.
void doGetSMSMsg(byte* buff) {

  uint16_t  numPNBytes;
  uint16_t  numMsgBytes;
  uint16_t  buffLen;
  char*     buffPtr;
  byte      msgIndex;

  msgIndex = buff[1];                                             // What message are we talking about here?
  buffPtr = &(buff[1]);                                           // In the buffer, this is where our reply message starts.
  buffLen = COM_BUFF_BYTES - 2;                                   // How many bytes we have to work wih here? (Subtract 2 for luck.)
  if (fona.getSMSSender(msgIndex, buffPtr, buffLen)) {            // First we read out the phone number..
    numPNBytes = strlen(buffPtr) + 1;                             // They don't tell us how many bytes they used. So we count 'em ourselves.
    buffPtr = buffPtr + numPNBytes;                               // Offset the buffer pointer.
    buffLen = buffLen - numPNBytes;                               // Reset the buffer length.
    if (fona.readSMS(msgIndex, buffPtr, buffLen, &numMsgBytes)) { // If we are successfull..
      fona.deleteSMS(msgIndex);                                   // Delete the message from the SIM.
      buff[0] = 0;                                                // Set no error flag.
      ourComObj.replyComBuff(numPNBytes + numMsgBytes + 2);       // Send back our result. + the error flag. (Add one, I'm missing the '\0')
      return;                                                     // At this point its success and we're done!
    }
  }
  buff[0] = 1;                                                    // We got here? Note the error.
  ourComObj.replyComBuff(1);                                      // Send back the error.
}


// This packs up the contents of the sweep buffer as a c string and sends it to the calling program.
void doSweepUNS(byte* buff) {

  /*
  unsBuff[unsIndex] = 0;                  // Add trailing '\0'.
  for(byte i=0;i<=unsIndex;i++) {         // Copy our bytes to the reply buff.
    buff[i]==unsBuff[i];
  }
  ourComObj.replyComBuff(unsIndex+1);     // Send off what we got.
  unsIndex = 0;                           // Reset the index for the next batch.
  unsBuff[0] = 0;                         // Reset the sweep buffer.
  */
  strcpy(buff,"Blubber");
  ourComObj.replyComBuff(strlen(buff)+1);
  unsIndex = 0;                           // Reset the index for the next batch.
  unsBuff[0] = 0;                         // Reset the sweep buffer.
}


// CallerID comes in as "unsolicited data". Meaning, between FONA commands. The buffer we save this
// unsolicited data in is sometimes called the sweep buffer. You want caller ID info.? Turn on the
// sweep buffer and it will save off the first 20 or so bytes of unsolicited data. Another issue is
// that this callerID stuff comes in on every ring! So once we get a load of it, we shut the damn
// thing off. The calling program can turn it back on when the call is complete.
void doCallID(byte* buff) {

  if(fona.setParam(F("AT+CLIP="),buff[1])) {  // Turn caller ID on/off.
    buff[0] = 0;                              // Success, reply 0 errors.
  } else {
    buff[0] = 1;                              // Fail, reply some sort of error.
  }                                           // Reset the sweep buffer.
  sweepState = !(buff[1]==0);                 // Either way, set the sweep state to what they wanted.
  unsIndex = 0;                               // Reset the sweep index.
  unsBuff[0] = 0;                             // Reset buffer.
  fona.flushSweep();                          // Clear out the old junk.
  ourComObj.replyComBuff(1);                  // Send back the reply.
}


void doTouchTone(byte* buff) {

  if (fona.playDTMF(buff[1])) {
    buff[0] = 0;
  } else {
    buff[0] = 1;
  }
  ourComObj.replyComBuff(1);
}
