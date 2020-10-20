#include <lists.h>
#include <lilParser.h>
#include <timeObj.h>

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7

#define COM_BUFF_BYTES  255
#define ANSWER_BYTES    100
#define PN_BUFF_BYTES   20
#define INT_STR_BYTES   10
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



// Our delightful set of global variables.

enum commands { noCommand, rollEm, sayHello, tell, help };
//enum names { noName, ma, pa, alex, dan, shelby };

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
LC_fona fona = LC_fona();

bool        FONAOnline;
byte        comBuff[COM_BUFF_BYTES];   // Buffer for all comunication.
byte        pnBuff[PN_BUFF_BYTES];     // Buffer for current phone number.
char        answer[ANSWER_BYTES];      // Buffer for our reply;
char        intStr[INT_STR_BYTES];     // Buffer to hold a int as a string.
bool        havePN;
int         SMSIndex = 1;
timeObj     serverTimer(100);
lilParser   comParser;
//lilParser   nameParser;
//char        famPN[5][12];



void setup() {

   comParser.addCmd(rollEm,"ROLL");
   comParser.addCmd(sayHello,"HELLO");
   comParser.addCmd(sayHello,"HI");
   comParser.addCmd(help,"?");
   
   /*
   nameParser.addCmd(dan,"DAN");
   nameParser.addCmd(alex,"ALEX");
   nameParser.addCmd(shelby,"SHELBY");
   nameParser.addCmd(ma,"MOM");
   nameParser.addCmd(pa,"DAD");
   
   strcpy(&(famPN[pa][0]),"14083400352");
   strcpy(&(famPN[ma][0]),"13607084218");
   strcpy(&(famPN[alex][0]),"13603335200");
   strcpy(&(famPN[dan][0]),"13609298457");
   strcpy(&(famPN[shelby][0]),"13603332799");
   */
   
   havePN = false;                                 // We do not yet have a phone number to text to.
   pinMode(0, INPUT);                              // Adafruit says to do this. Otherwise it may read noise.
   pinMode(FONA_RST, OUTPUT);                      // Used for resetting the FONA.
   FONAOnline = false;                             // Not ready yet..
   resetFONA();                                    // Hit reset, see if it'll come online.
}


// This checks the message queue on the phone chip. If there is a message
// It will grab it, along with its phone number and return a pointer to the
// message text. No message, or and error, will return NULL.
char* checkForMessage(void) {

   int   numSMS;
   int   index;
   char* message;
   
   havePN = false;                                                // We don't have a valid one right now.
   message = NULL;                                                // Or this either.
   numSMS = fona.getNumSMS();                                     // Number SMS(s). This one gives bogus numbers at times. Max is 30.
   if (numSMS > 30) {                                             // If its more than it can hold..
      numSMS = 0;                                                 // Call it a mistake. Bogus readings get zero.
   }                                 
   if (numSMS) {                                                  // If we have unread text messages..
      comBuff[1] = SMSIndex++;                                    // Load the com buffer up with the index of the message to check. Bump up the index.                                   
      if (SMSIndex>30) {                                          // If the index is > 30..
         SMSIndex = 1;                                            // Reset the index to 1.
      }
      getSMSMsg(comBuff);                                         // Attempt to retrieve a message from the phone chip.
      if (comBuff[0]==0) {                                        // This says no errors. So lets decode what we got.
         strcpy(pnBuff,&(comBuff[1]));                            // Byte 0 is error, after that is the phone number. Copy it out.
         index = strlen(pnBuff)+2;                                // BEFORE we hack it up, locate the message part. Add two, error byte & EOS byte.
         message = &(comBuff[index]);                             // Here's the message.
         filterPNStr(pnBuff);                                     // NOW we can clear out the phone number junk.
         havePN = strlen(pnBuff)>=7;                              // Well, it needs at least 7 chars.
      }
   } else {                                                       // Else we have no messages. Reset index to 1.
      SMSIndex = 1;
   }
   if (havePN) {                                                  // If we got a seemingly valid phone number..
      return message;                                             // The result is the address of the message. NULL for none.
   } else {                                                       // Else, no valid phone number..
      return NULL;                                                // Give 'em back a NULL. Who knows what went wrong?
   }
}


void handleMessage(char* message) {

   int   i;
   int   command;
   
   i = 0;
   while(message[i]!='\0') {
      comParser.addChar(toupper(message[i]));
      i++;
   }
   command = comParser.addChar(EOL);
   switch(command) {
      case noCommand : break;
      case rollEm    : doDiceRoll();   break;
      case sayHello  : doSayHello();   break;
      //case tell      : doTell();       break;
      case help      : doHelp();       break;
      default        : doHelp();       break;
   }
}


void loop() {

   char* message;
    
   if (serverTimer.ding()) {                                         // If its time to check messages..
      serverTimer.start();                                           // Reset our timer.
      message = checkForMessage();
      if (message) {
         handleMessage(message);
      }
   }
}
         


// *************************************************
// ******************* handlers ********************
// *************************************************


// User has typed Hi or Hello. Lets send back a greeting.
void doSayHello(void) {

   strcpy(&(comBuff[1]),"Greetings! I'm the online dice rolling machine.");
   sendSMSMsg(comBuff);                                // Send the reply stored in the com buffer
}


// User has typed ROLL and possibly a nunber of dice. This is where we deal with that.
void doDiceRoll(void) {

   char* numStr; 
   int   numDice;
   int   total;
   int   roll;

   numDice = 1;                                          // We start with 1..
   if (comParser.numParams()) {                          // If they typed in something past the command.
      numStr = comParser.getParamBuff();                 // We get the first parameter, assume its the number of dice.
      numDice = atoi(numStr);                            // Parse the actual number of dice they call for.
      free(numStr);                                      // Dump the parameter buffer ASAP.
      if (numDice==0) {                                  // If the number of dice is zero..
         numDice = 1;                                    // We'll give them one anyhow.
      }
   }
   if (numDice>MAX_DICE) {                            // If the number of dice is more than the max..
      strcpy(answer,"Too many dice! You only get ");  // Set the answer sgtring to an error message.
      strcat(answer,str(MAX_DICE));                   //
      strcat(answer,".");                             //
   } else {                                           // Else, the number of dice is ok..
      strcpy(answer,"Rolling ");                     // Send back header.
      strcat(answer,str(numDice));                    //
      strcat(answer," dice.\n");                    //
      total = 0;                                      // Zero out our total.
      for (int i=1;i<=numDice;i++) {                  // For each die..
         roll = random(1,6);                          // Get a random value 1..6.
         total = total + roll;                        // Add it to the total.
         strcat(answer,str(roll));                    // Add the value to the answer string.
         if (i!=numDice) {                            // If there are more dice to roll..
            strcat(answer,", ");                      // We add a comma to the answer.
         } else {                                     // Else, we rolled them all..
            strcat(answer,"\nTotal : ");               // Add "Total :" to the answer string.
            strcat(answer,str(total));                // Add the actual total to the anser string.
         }
      }
   }
   strcpy(&(comBuff[1]),answer);                      // Copy the assemebled answer to the com buffer. 
   sendSMSMsg(comBuff);                                // Send the reply stored in the com buffer
}

 /*      
void doTell() {

   int   numParams;
   char* nameStr;
   char* tokenStr;
   int   i;
   int   name;

   numParams = comParser.numParams();                                            // Get the number of params. 
   if (numParams>1) {                                                            // If they typed in something past the command..
      nameStr = comParser.getParamBuff();                                        // We get the first parameter, assume its ther name.
      i = 0;                                                                     // Set char pointer to zero.
      while(nameStr[i]!='\0') {                                                  // While we have name chars to parse..
         nameParser.addChar(toupper(nameStr[i]));                                // Force uppercase and parse the letters.
         i++;                                                                    // Next.
      }
      name = comParser.addChar(EOL);                                             // After we get all the letters, stuff in an EOL to flag the end.
      free(nameStr);
      if (name>0) {                                                              // If we got a valid name..
         if (!strcmp(pnBuff,&famPN[pa][0])) strcpy(answer,"Dad");                // Dad sent it..
         else if (!strcmp(pnBuff,&famPN[ma][0])) strcpy(answer,"Mom");           // Ma sent it..
         else if (!strcmp(pnBuff,&famPN[alex][0])) strcpy(answer,"Alex");        // Alex sent it..
         else if (!strcmp(pnBuff,&famPN[dan][0])) strcpy(answer,"Dan");          // Dan sent it..
         else if (!strcmp(pnBuff,&famPN[shelby][0])) strcpy(answer,"Shelby");    // Shelby sent it..
         strcat(answer," wanted me to tell you..\n");                            // Finish "sent" line.
         tokenStr = comParser.getParamBuff();                                    // Grab the token after the name.
         while(tokenStr) {                                                       // While we have tokens..                          
            strcat(answer,tokenStr);                                             // Add the token to the output string.
            free(tokenStr);                                                      // Free the token memory.
            tokenStr = nameParser.getParamBuff();                                // Grab the next token.
            if (tokenStr) {                                                      // If we got a token..
               strcat(answer," ");                                               // Add a space to the output string.
            }
         }
         switch(name) {                                                          // Now check the name to see who to send this to.
            case pa     : strcpy(pnBuff,&famPN[pa][0]);     break;               // Pa
            case ma     : strcpy(pnBuff,&famPN[ma][0]);     break;               // Ma
            case alex   : strcpy(pnBuff,&famPN[alex][0]);   break;               // Etc..
            case dan    : strcpy(pnBuff,&famPN[dan][0]);    break;
            case shelby : strcpy(pnBuff,&famPN[shelby][0]); break;
         }
         strcpy(&(comBuff[1]),answer);                                           // Copy the assemebled answer to the com buffer. 
         sendSMSMsg(comBuff);                                                    // Send the reply stored in the com buffer
         return;                                                                 // ON success we leave the party here.
      }
   }
   strcpy(answer,"Tell? Tell what? I've no idea what you are saying.\nTell [name] [message] is the format.");
   strcpy(&(comBuff[1]),answer);                                                 // Copy in the error message. 
   sendSMSMsg(comBuff);                                                          // Send it on its way.
}
*/

void doHelp(void) {

   strcpy(&(comBuff[1]),"Msg HELLO and I'll say hello back .\nType ROll and a number and I'll roll dice for you.");
   sendSMSMsg(comBuff);                                // Send the reply stored in the com buffer
}



// *************************************************
// **************** utiity functions ***************
// *************************************************


// Called by setup. This resets the FONA hardware and then sets up the correct
// configuration for the phone chip.
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
  fona.setParam(F("AT+CLIP="), 0);              // Set caller ID (0 = off).
  //fona.enableNetworkTimeSync(true);           // See if it works..
}


// If there is a current phone number set, A text string starting at char[1] of buff
// will be sent out to that number.
void sendSMSMsg(byte* buff) {

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


// First byte is the index of the message we're after. When we pack our reply
// it will be error byte. Then two c-strings. First is sending phone number,
// second is the text message itself. Once a message is read, it's deleted
// from the SIM chip.
void getSMSMsg(byte* buff) {

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
      return;                                                     // At this point its success and we're done!
    }
  }
  buff[0] = 1;                                                    // We got here? Note the error.
}


// Drop in a c string and this'll strip out anything
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


// Drop in an integer and out pops a pointer to the string version of it.
char* str(int value) { 

   snprintf(intStr,10,"%d",value);
   return intStr;
}
