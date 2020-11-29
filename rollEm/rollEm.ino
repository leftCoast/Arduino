#include "LC_fona.h"
#include <idlers.h>
#include <timeObj.h>
#include "GPSnAirCommon.h"


#define SENSOR_TX       12
#define SENSOR_RX       10

#define COM_BUFF_BYTES  255
#define ANSWER_BYTES    100
#define PN_BUFF_BYTES   20
#define MAX_DICE        12


SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial sensor = SoftwareSerial(SENSOR_TX, SENSOR_RX);

LC_fona fona = LC_fona();

bool      FONAOnline;
byte      comBuff[COM_BUFF_BYTES];    // Buffer for all comunication.
byte      pnBuff[PN_BUFF_BYTES];      // Buffer for current phone number.
char      answer[ANSWER_BYTES];       // Buffer for our reply;
char      intStr[10];               // Buffer for writing numbers.
bool      havePN;
int       SMSIndex = 1;
timeObj   serverTimer(500);
timeObj   sensorTimer(5000);


void setup() {
  
  Serial.begin(57600);
  //while (!Serial) { }
  Serial.println("I'm here.");
  sensor.begin(9600);
  Serial.println("sensor.begin(9600); called.");
  havePN = false;                                 // We do not yet have a phone number to text to.
  pinMode(0, INPUT);                              // Adafruit says to do this. Otherwise it may read noise.
  pinMode(13, OUTPUT);                            // Our one and only debugging tool.
  pinMode(FONA_RST, OUTPUT);                      // Used for resetting the FONA.
  FONAOnline = false;                             // Not ready yet..
  resetFONA();
  Serial.println("resetFONA(); called");                                    // Hit reset, see if it'll come online.
}


char* str(int value) { 

   snprintf(intStr,10,"%d",value);
   return intStr;
}


void loop() {

  //checkDebug();
  //checkText();
  checkSensor();
}

void checkSensor(void) {

  int numBytes;
  timeObj timOut(2000);
  
  if (sensorTimer.ding()) {
    Serial.println("Checking sensor..");
    sensor.listen();
    while(sensor.available()) {
      Serial.println("Dumping..");
      sensor.read();            // Flush out the serial port.
      sleep(5);                 // Time for more..
    }
    Serial.println("Sending the R");
    sensor.print("R");
    while(sensor.available()<sizeof(sensorData) && !timOut.ding()) {
      sleep(5);
    }
    if (timOut.ding()) {
      Serial.print("Time out.");
    } else {
      Serial.print("I think I got it.");
    }
    sensorTimer.start();
  }
}


void checkText() {
  
   int   numSMS;
   int   index;
   char* message;
   char* numStr;
   char* comStr;
   int   numDice;
   int   total;
   int   roll;
    
   idle();                                                           // Let anyone that's idleing have some time.
   if (serverTimer.ding()) {                                         // If its time to check messages..
      serverTimer.start();                                           // Reset our timer.
      numSMS = fona.getNumSMS();                                     // Number SMS(s). This one gives bogus numbers at times. Max is 30.
      if (numSMS > 30) {                                             // If its more than it can hold..
         numSMS = 0;                                                 // Call it a mistake. Bogus readings get zero.
      }                                 
      if (numSMS) {                                                  // If we have unread text messages..
         comBuff[1] = SMSIndex++;                                    // Load the com buffer up with the index of the message to check. Bump up the index.                                   
         if (SMSIndex>30) {                                          // If the index is > 30..
            SMSIndex = 1;                                            // Reset the index to 1.
         }
         doGetSMSMsg(comBuff);                                       // Attempt to retrieve a message from the phone chip.
         if (comBuff[0]==0) {                                        // This says no errors. So lets decode what we got.
            strcpy(pnBuff,&(comBuff[1]));                            // Byte 0 is error, after that is the phone number. Read it out.
            index = strlen(pnBuff)+2;                                // Locate the message part. Add two, error byte & EOS byte.
            message = &(comBuff[index]);                             // Here's the message.
            filterPNStr(pnBuff);                                     // Clear out the phone number junk.
            havePN = strlen(pnBuff)>=7;                              // Well, it needs at least 7 chars.
            comStr = strtok(message," ");                            // First slice HI-YA!
            if (comStr) {                                            // If we have a token to decode..
               toupper(comStr);                                      // Make life easier, just mak everything uppercase.           
               if (!strcmp("ROLL",comStr)) {                         // If the token is "ROLL"..
                  numStr = strtok (NULL," ");                        // See if we can get a number of dice token.
                  numDice = atoi(numStr);                            // Parse the actual number of dice they call for.
                  if (numDice==0) {                                  // If the number of dice is zero..
                     numDice = 1;                                    // We'll give them one.
                  } else if (numDice>MAX_DICE) {                     // If the number of dice is more than the max..
                     strcpy(answer,"Too many dice! You only get ");  // Set the answer sgtring to an error message.
                     strcat(answer,str(MAX_DICE));                   //
                  } else {                                           // Else, the number of dice is ok..
                     strcpy(answer,"Rolling! ");                     // Send back header.
                     total = 0;                                      // Zero out our total.
                     for (int i=1;i<=numDice;i++) {                  // For each die..
                        roll = random(1,6);                          // Get a random value 1..6.
                        total = total + roll;                        // Add it to the total.
                        strcat(answer,str(roll));                    // Add the value to the answer string.
                        if (i!=numDice) {                            // If there are more dice to roll..
                           strcat(answer,", ");                      // We add a comma to the answer.
                        } else {                                     // Else, we rolled them all..
                           strcat(answer," Total : ");               // Add "Total :" to the answer string.
                           strcat(answer,str(total));                // Add the total to the anser string.
                        }
                     }
                  }
                  strcpy(&(comBuff[1]),answer);                      // Copy the assemebled answer to the com buffer.
               } else {
                  strcpy(&(comBuff[1]),"Sorry, the only thing I can do is Roll dice. Msg me ROLL and a number and I'll roll 'em for you.");
               } 
            }
            doSendSMSMsg(comBuff);                                   // Send the reply stored in the com buffer
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
