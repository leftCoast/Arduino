#include <SPI.h>
#include <SSD_13XX.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <timeObj.h>

// The four shared SPI pins..
// These pins are best choice for Arduino & Teensy.
#define LC_CLK  13  // Clock
#define LC_MISO 12  // Input data
#define LC_MOSI 11  // Output data
#define LC_DC    9  // Data/command

// The  four the sound card uses.
#define SOUND_CS    20
#define SOUND_SDCS  21
#define SOUND_DRQ   1
#define SOUND_RST   15

// The OLED is only using these two. screen & reset.
#define OLED_CS     10
#define OLED_RST    6
#define OLED_SDCS   -1    // Not wired

timeObj songTimer(40);  // 40ms song timer.
SSD_13XX tft(OLED_CS, LC_DC, OLED_RST, LC_MOSI, LC_CLK);

Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer
  (SOUND_RST, SOUND_CS, LC_DC, SOUND_DRQ, 0);


void setup() {
  Serial.begin(9600); while (!Serial);
  Serial.println(F("Serial's up."));

  SD.begin(SOUND_SDCS);


  if (!musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053"));
  } else {
    Serial.println(F("VS1053 found"));
    musicPlayer.startPlayingFile("Corvette.mp3");
    songTimer.start();
  }

  /*
    tft.begin(false);
    Serial.print("For error we got: ");Serial.println(tft.getErrorCode());
    tft.fillScreen(BLUE);
    tft.print("Ready, Sir!");
  */
}

void loop() {

  if (musicPlayer.playingMusic && songTimer.ding()) {
    noInterrupts();
    musicPlayer.feedBuffer();
    interrupts();
    songTimer.start();
  }
}
