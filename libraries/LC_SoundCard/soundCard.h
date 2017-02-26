#ifndef soundCard_h
#define soundCard_h

#include <SD.h>
#include <Adafruit_VS1053.h>

#include <idlers.h>
#include <timeObj.h>

// The usual SPI pins..
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card

// These are the pins used for the breakout example
#define BREAKOUT_RESET  2      // VS1053 reset pin (output)
#define BREAKOUT_CS     1     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    7      // VS1053 Data/command select pin (output)

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)      

// The default SD card select pin. (Fixed for the Shield type)
#define soundCard_SD_CS 4  

// Possible setups 
#define soundCard_SHIELD   0
#define soundCard_BREAKOUT 1

// how long 'till we load in more sound data.
#define soundCard_SLEEP_MS 100

enum action { play, pause, restart };

enum soundCardErr { 
  noErr, badSetup, initErr, badCommand, 
  noFileErr,mallocErr,nullStrErr, unknownErr
  };


class soundCard : public idler, public timeObj {

  public:
    soundCard(byte boardSetup);
    ~soundCard(void);

          boolean       begin(void);
          boolean       setSoundfile(char* inFilePath);
          boolean       command(action inCommand);
          boolean       isPlaying(void);
          void          setVolume(byte volume);
          byte					getVolume(void);
          void          setError(soundCardErr inErr);
          soundCardErr getLastError(void);

  protected:
  virtual void idle(void);

    Adafruit_VS1053_FilePlayer* musicPlayer;
    soundCardErr lastErr;
    byte          setupType;
    char*         filePath;
    byte					volume;
    boolean       playing;
};

#endif

