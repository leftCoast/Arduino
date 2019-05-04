 
// This one is all about simple test cases. I'm tired of being in the dark
// on how this stupid thing works.
 

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7

#define COM_BUFF_BYTES  255
#define CID_BUFF_BYTES  20

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

  if (strlen(replybuffer) > 15) {
    return parseReplyQuoted(F("+CLIP: "), IDBuff, numBytes, ',', 0);
  }
  return false;
}





SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = LC_fona();

bool          FONAOnline;
byte          comBuff[COM_BUFF_BYTES]; // Buffer for all comunication.
char          CIDBuff[CID_BUFF_BYTES];  // Buffer for callerID.


void setup() {

    Serial.begin(57600);
    while (!Serial) {
      
    }
    Serial.println("I'm here.");
    
    pinMode(0,INPUT);                               // Adafruit says to do this. Otherwise it may read noise.
    pinMode(13, OUTPUT);                            // Our one and only debugging tool.
    pinMode(FONA_RST, OUTPUT);            // Used for resetting the FONA.
    resetFONA();                          // Hit reset, see if it'll come online.
    Serial.print("FONA onine : ");Serial.println(FONAOnline);
}

/*
void loop() {
  uint16_t batteryVolts;
  
  //idle();
  if (fona.getBattVoltage(&batteryVolts)) {             // Attempt to read the battery voltage..
    Serial.print("Volts = ");Serial.println(batteryVolts);
   } else {
    Serial.print("Volts = ");Serial.println("Nothing but errors.");
  }
  delay(1000);
}
*/

void loop() {

  char aChar;
  
  if (Serial.available()) {
    aChar = Serial.read();
    fonaSS.write(aChar);
  }
  if (fonaSS.available()) {
    aChar = fonaSS.read();
    switch(aChar) {
      case '\n' : Serial.print("[NL]"); break;
      case '\r' : Serial.print("[RT]"); break;
      case '\0' : Serial.print("[NULL]"); break;
    }
    Serial.print(aChar);
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
  fonaSS.begin(4800);                   // For talking to the FONA.
  FONAOnline = fona.begin(fonaSS);      // Able to fire up the FONA.
  fona.setAudio(FONA_EXTAUDIO);         // Um.. Why is this here?
  fona.setVolume(40);
  fona.enableNetworkTimeSync(true);     // See if it works..

/*
For those who wonder : You can add a call fona
setAudio(FONA_EXTAUDIO) to your initialization code or issue
direct command 'AT+CHFA=1' towards SIM800 module.
In my case this module is no longer losing SIM card when
receiving or placing phone calls.
Thanks for a tip!
*/
}
