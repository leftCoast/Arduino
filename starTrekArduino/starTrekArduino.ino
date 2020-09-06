#include "idlers.h"        // Sets up an idle loop so things can run in background.
#include "sst.h"           // Main definition file for the Star Trek game.
#include "textBuff.h"      // Rin buffer for text. Kind'a like streams. Lighter, just text.
#include "arduinoTrek.h"   // Glue code lives in here and it's buddy arduinoTrek.cpp

// *****************************************************
//                      starTrekUpdater .h stuff..
// *****************************************************

class starTrekUpdater : public idler {

  public:
            starTrekUpdater(void);
  virtual   ~starTrekUpdater(void);

  virtual  void  idle(void);         
};



// *****************************************************
//                      starTrekUpdater .cpp stuff..
// *****************************************************


starTrekUpdater::starTrekUpdater(void) { hookup(); }


starTrekUpdater::~starTrekUpdater(void) {  }


// The game sits in getChar() forever looking for input. We use this background idle time to..
// First, find what the game has outputted and feed that to the screen.
// Then, see if the user has sent us anything. If so, feed that to the game.
void starTrekUpdater::idle(void) {

   char  aChar;
   
   while(!trekReplyBuffer->empty()) {              // While the reply buffer is not empty..
      Serial.print(trekReplyBuffer->readChar());   // Send each char to the serial monitor.
   }
   while(Serial.available()) {                     // While there are command chars available..
      aChar = Serial.read();                       // Grab each char.
      trekComBuffer->addChar(aChar);               // Send it to the game.
      Serial.print(aChar);                         // Echo it to the screen.
   }
}


starTrekUpdater* ourUpdater;

void setup(void) {

   ourUpdater = new starTrekUpdater(); // This is what actually runs everyhing through the monitor.
   arduinoTrekSetup();                 // The initialization bit for the game.
}

void loop(void) {

   idle();
   arduinoTrekLoop();                              // This blocks 'till the gane finishes.
   prout((char*)"Do you want to play again?");     // Lie, loop forces you to play again and again forever!
   if (!ja()) {                                    // The game's call for yes/no.
      skip(1);                                     // the game's call for skipping(x) lines.
      prout((char*)"May the Great Bird of the Galaxy roost upon your home planet.");
      sleep(4000);                                 // Give you some time to read it.
   }
}
