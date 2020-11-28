#ifndef LC_fona_h
#define LC_fona_h

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX         9
#define FONA_TX         8
#define FONA_RST        4
#define FONA_RI         7


class LC_fona : public Adafruit_FONA {

  public:
    LC_fona(void);
    virtual ~LC_fona(void);

    bool  setParam(FONAFlashStringPtr send, int32_t param);
    bool  checkForCallerID(char* IDBuff, byte numBytes);

    FONAFlashStringPtr ok_reply;
};


#endif
