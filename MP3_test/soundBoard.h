#ifndef soundBoard_h
#define soundBoard_h

#include <SD.h>
#include <Adafruit_VS1053.h>

#include <idlers.h>
#include <timeObj.h>

// The usual SPI pins..
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)      

// The default SD card select pin. (Fixed for the Shield type)
#define SOUNDBOARD_SD_CS 4  

// Possible setups 
#define SOUNDBOARD_SHIELD   0
#define SOUNDBOARD_BREAKOUT 1

// how long 'till we load in more sound data.
#define SOUNDBOARD_SLEEP_MS 100

enum action { play, pause, restart };

enum soundBoardErr { 
  noErr, badSetup, initErr, badCommand, 
  noFileErr,mallocErr,nullStrErr, unknownErr
  };


class soundBoard : public idler, public timeObj {

  public:
    soundBoard(byte boardSetup);
    ~soundBoard(void);

          boolean       begin(void);
          boolean       setSoundfile(char* inFilePath);
          boolean       command(action inCommand);
          boolean       isPlaying(void);
          void          setError(soundBoardErr inErr);
          soundBoardErr getLastError(void);

  protected:
  virtual void idle(void);

    Adafruit_VS1053_FilePlayer* musicPlayer;
    soundBoardErr lastErr;
    byte          setupType;
    char*         filePath;
    boolean       playing;
};

#endif

