#ifndef soundCard_h
#define soundCard_h

#include <LC_SPI.h>
#include <Adafruit_VS1053.h>

#include <idlers.h>
#include <timeObj.h>


// These are the pins used for the music maker shield
#define SHIELD_RESET	-1      // VS1053 reset pin (unused!)
#define SHIELD_CS			 7      // VS1053 chip select pin (output)
#define SHIELD_DC			 6      // VS1053 Data/command select pin (output)      
#define SHIELD_SC_CS	 4			// Onboard SD card.
#define SHIELD_DREQ		 3 


// Possible setups 
#define soundCard_SHIELD   0
#define soundCard_BREAKOUT 1

// how long 'till we load in more sound data.
#define soundCard_SLEEP_MS 40

enum action { play, pause, fullStop, restart };

enum soundCardErr { 
  noErr, badSetup, initErr, badCommand, 
  noFileErr,mallocErr,nullStrErr, unknownErr
  };


class soundCard : public idler, public timeObj {

	public:
				soundCard(byte boardSetup,byte inCsPin,byte inDrqPin,byte inResetPin=-1);
	virtual	~soundCard(void);

				bool				begin(void);
				bool				setSoundfile(char* inFilePath);
				bool				command(action inCommand);
				bool				isPlaying(void);
				void				setVolume(byte volume);
				byte				getVolume(void);
				void				setError(soundCardErr inErr);
				soundCardErr	getLastError(void);
				void				playClip(char* filePath);		// NOTE: This BLOCKS! ONLY for "clicks".

  protected:
  
	virtual	void				idle(void);

				Adafruit_VS1053_FilePlayer* musicPlayer;
				soundCardErr	lastErr;
				byte				setupType;
				byte				csPin;
				byte				drqPin;
				byte				resetPin;
				char*				filePath;
				byte				volume;
				bool				playing;
				bool				newSong;
};

#endif

