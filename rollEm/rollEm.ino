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
#define ANSWER_BYTES    100
#define PN_BUFF_BYTES   20
#define MAX_DICE        12



class LC_fona : public Adafruit_FONA {

  public:
    LC_fona(void);
    virtual ~LC_fona(void);

    bool  setParam(FONAFlashStringPtr send, int32_t param);
    bool  checkForCallerID(char* IDBuff, byte numBytes);

    FONAFlashStringPtr ok_reply;
};


LC_fona::LC_fona(void)
  : Adafruit_FONA(FONA_RST) {
  ok_reply = F("OK");
}


LC_fona::~LC_fona(void) {  }


bool LC_fona::setParam(FONAFlashStringPtr send, int32_t param) {

  return sendCheckReply(send, param, ok_reply);
}


bool  LC_fona::checkForCallerID(char* IDBuff, byte numBytes) {

  if (mySerial->available()) {                                          // Is something is there we didn't ask for..
    if (readline(25,true) > 15) {                                      // See if we can grab the first 20 or so chars.
      return parseReplyQuoted(F("+CLIP: "), IDBuff, numBytes, ',', 0);  // Run it through the Adafruit parser thing to see if its a caller ID.
    }
  }
  return false;
}


SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
LC_fona fona = LC_fona();

bool     FONAOnline;
byte     comBuff[COM_BUFF_BYTES];    // Buffer for all comunication.
byte     pnBuff[PN_BUFF_BYTES];      // Buffer for current phone number.
char     answer[ANSWER_BYTES];       // Buffer for our reply;
bool     havePN;
int      SMSIndex = 1;
timeObj  serverTimer(500);

void setup() {
  /*
    Serial.begin(57600);
    while (!Serial) { }
    Serial.println("I'm here.");
  */
  havePN = false;                                 // We do not yet have a phone number to text to.
  pinMode(0, INPUT);                              // Adafruit says to do this. Otherwise it may read noise.
  pinMode(13, OUTPUT);                            // Our one and only debugging tool.
  //CIDBuff[0] = '\0';                              // Reset the callerID buffer.
  //ourComObj.begin(comBuff, COM_BUFF_BYTES, 9600); // Buff, bytes & Baud. Setup for talking to the GUI.
  //checkComErr();                                  // Read comunications error. On error we stop here and blink.
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


// Fill this out big guy.
char* str(int value) { }


void loop() {

   int   numSMS;
   int   index;
   char* message;
   char* numStr;
   char* comStr;
   int   numDice;
   int   total;
   int   roll;
    
   idle();                                                           // Let anyone that's idleing have some time.
   if (serverTimer.ding()) {
      serverTimer.start();
      numSMS = fona.getNumSMS();                                     // Number SMS(s). This one gives bogus numbers at times. Max is 30.
      if (numSMS > 30) { 
         numSMS = 0;                                                 // Bogus readings get zero.
      }                                 
      if (numSMS) {
         comBuff[1] = SMSIndex++;
         if (SMSIndex>30) {
            SMSIndex = 1;
         }
         doGetSMSMsg(comBuff);
         if (comBuff[0]==0) {                                        // This says no errors. So lets decode what we got.
            strcpy(pnBuff,&(comBuff[1]));                            // Byte 0 is error, after that is the phone number.
            index = strlen(pnBuff)+2;                                // Locate the message part. Add two, error byte & EOS byte.
            message = &(comBuff[index]);                             // Here's the message.
            filterPNStr(pnBuff);                                     // Clear out the phone number junk.
            comStr = strtok(message," ");                            // First slice HI-YA!
            if (comStr) {
               toupper(comStr);
               if (!strcmp("ROLL",comStr)) {
                  numStr = strtok (NULL," ");
                  numDice = atoi(numStr);
                  if (numDice==0) {
                     numDice = 1;
                  } else if (numDice>MAX_DICE) {
                     strcpy(answer,"Too many dice! You only get ");
                     strcat(answer,str(MAX_DICE));
                  } else {
                     strcpy(answer,"Rolling! ");
                     total = 0;
                     for (int i=1;i<=numDice;i++) {
                        roll = random(1,6);
                        total = total + roll;
                        strcat(answer,str(roll));
                        if (i!=numDice) {
                           strcat(answer,", ");
                        } else {
                           strcat(answer," Total : ");
                           strcat(answer,str(total));
                        }
                     }
                  }
                  strcpy(&(comBuff[1]),answer);
               } else {
                  strcpy(&(comBuff[1]),"Sorry, the only thing I can do is Roll dice. Msg me ROLL and a number and I'll roll 'em for you.");
               } 
            }
            doSendSMSMsg(comBuff);
         }
      }
   }
}
       

// Drop in a string and this'll strip out anything
// that's not a "dial-able" character.
// ** WARNING ** This writes to the string in place. So you can't pass a string that was
// allocated at compile time IE char myNum = "1 408 340-0352"
void filterPNStr(char* str) {

   int numChars;
   int index;
   
   if (str) {                          // Sanity, they could pass in a NULL. They do that now and then.
      numChars = strlen(str);          // Ok have something. Lets count 'em.
      index = 0;                       // We'll use this to index mRawPN.
      for(int i=0;i<=numChars;i++) {   // We'll loop though including the EOL.
         switch(str[i]) {              // There may be better ways of doing this,
            case '0'  :                //  but this makes it realaly obvious what we're doing.
            case '1'  :
            case '2'  :
            case '3'  :
            case '4'  :
            case '5'  :
            case '6'  :
            case '7'  :
            case '8'  :
            case '9'  :
            case '#'  :
            case '*'  :
            case '\0' : str[index++] = str[i]; break;  // Stuff in the "filtered" char.
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
  fona.setParam(F("AT+CALS="), 3);              // Set 3rd ringtone. 0..19
  fona.setParam(F("AT+CLIP="), 0);              // Set caller ID (0 = off).
  //fona.enableNetworkTimeSync(true);           // See if it works..
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
      //ourComObj.replyComBuff(numPNBytes + numMsgBytes + 2);       // Send back our result. + the error flag. (Add one, I'm missing the '\0')
      return;                                                     // At this point its success and we're done!
    }
  }
  buff[0] = 1;                                                    // We got here? Note the error.
  //ourComObj.replyComBuff(1);                                      // Send back the error.
}
