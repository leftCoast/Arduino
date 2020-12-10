#include "LC_fona.h"
#include <idlers.h>
#include <timeObj.h>
#include "readData.h"


#define COM_BUFF_BYTES  255
#define ANSWER_BYTES    255
#define PN_BUFF_BYTES   20
#define NUM_STR_BYTES   15
#define sensor          Serial1

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);

LC_fona fona = LC_fona();

bool     FONAOnline;
byte     comBuff[COM_BUFF_BYTES];  // Buffer for all comunication.
byte     pnBuff[PN_BUFF_BYTES];    // Buffer for current phone number.
char     answer[ANSWER_BYTES];     // Buffer for our reply;
char     numStr[15];               // Buffer for writing numbers.
bool     havePN;
int      SMSIndex = 1;
timeObj  serverTimer(500);
timeObj  sensorTimer(10000);
dataIn   ourDataReader;
bool     readingSensor = false;


void setup() {
  
   Serial.begin(57600);
   Serial.println("I'm here.");
   sensor.begin(9600);
   readingSensor = false;
   
   havePN = false;                                 // We do not yet have a phone number to text to.
   pinMode(0, INPUT);                              // Adafruit says to do this. Otherwise it may read noise.
   pinMode(13, OUTPUT);                            // Our one and only debugging tool.
   pinMode(FONA_RST, OUTPUT);                      // Used for resetting the FONA.
   FONAOnline = false;                             // Not ready yet..
   resetFONA();                                    // Hit reset, see if it'll come online.
   serverTimer.start();                            // Fire up the server timer.
   sensorTimer.start();                            // Fire up the sensor timer.
}


char* i_str(int value) { 

   snprintf(numStr,15,"%d",value);
   return numStr;
}

char* f_str(int value,int fix=2) { 

   snprintf(numStr,15,"%d.2",value);
   return numStr;
}

void blink13(void) {
  
  digitalWrite(13,HIGH);
  delay(20);
  digitalWrite(13,LOW);
}

void loop() {

   idle();
  //checkDebug();
  //checkText();
  checkSensor();
  loadData();
  //Serial.println(answer);
  //Serial.print(strlen(answer));Serial.println(" chars");
}



void checkSensor(void) {
   
   if (!readingSensor) {
      while(sensor.available()) {
         sensor.read();
      }
      ourDataReader.reset();
      readingSensor = true;
   } else {
      if (ourDataReader.mComplete) {
         readingSensor = false;
         sensorTimer.start();
      } else if(sensor.available() && !ourDataReader.mComplete) {
         ourDataReader.readStream();
      }
   }
}

void loadData(void) {

   strcpy(answer,"----\n"); 
   strcat(answer,i_str(ourData.hour));strcat(answer,":");strcat(answer,i_str(ourData.min));strcat(answer,":");strcat(answer,i_str(ourData.sec));strcat(answer,"\n");
   strcat(answer,i_str(ourData.day));strcat(answer,"/");strcat(answer,i_str(ourData.month));strcat(answer,"/");strcat(answer,i_str(ourData.year));strcat(answer,"\n");
   strcat(answer,"Fix - "); if (ourData.valid) { strcat(answer,"valid\n"); } else { strcat(answer,"invalid\n"); }
   strcat(answer,"Lat ");strcat(answer,i_str(ourData.degLat));strcat(answer," Deg ");strcat(answer,f_str(ourData.minLat));strcat(answer," Min "); if(ourData.latQuad==north) { strcat(answer,"N\n"); } else { strcat(answer,"S\n"); }
   strcat(answer,"Lon ");strcat(answer,i_str(ourData.degLon));strcat(answer," Deg ");strcat(answer,f_str(ourData.minLon));strcat(answer," Min "); if(ourData.lonQuad==east) { strcat(answer,"E\n"); } else { strcat(answer,"W\n"); }
   strcat(answer,"Spd ");strcat(answer,f_str(ourData.knots));strcat(answer," Kn\n");
   strcat(answer,"Course ");strcat(answer,f_str(ourData.course));strcat(answer," Deg\n");
   strcat(answer,"Mag var ");strcat(answer,f_str(ourData.magVar));if (ourData.varDir>0) { strcat(answer," + Deg\n"); } else { strcat(answer," - Deg\n"); }
   strcat(answer,"Temp ");strcat(answer,f_str(ourData.temp));strcat(answer," Deg C\n");
   strcat(answer,"Pres ");strcat(answer,f_str(ourData.pressure));strcat(answer," hPa\n");
   strcat(answer,"Humidity ");strcat(answer,f_str(ourData.humidity));strcat(answer," %\n");
   strcat(answer,"Air qual ");strcat(answer,f_str(ourData.gas));strcat(answer," KOhms\n");
   strcat(answer,"Alt ");strcat(answer,f_str(ourData.altitude));strcat(answer," m\n"); 
}
    
void checkText() {
  
   int   numSMS;
   int   index;
   char* message;
   char* numStr;
   char* comStr;
    
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
               if (!strcmp("DATA",comStr)) {                         // If the token is "DATA"..
                  loadData();
                  
                  strcpy(&(comBuff[1]),answer);                      // Copy the assemebled answer to the com buffer.
               } else {
                  strcpy(&(comBuff[1]),"Sorry, all I can do is send data. Msg me DATA and I'll send you some.");
               } 
            }
            doSendSMSMsg(comBuff);                                   // Send the reply stored in the com buffer
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
