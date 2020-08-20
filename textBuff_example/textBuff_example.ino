#include "textBuff.h"
#include "timeObj.h"


textBuff*   replyBuff;


void setup(void) {

   Serial.begin(57600);              // Or whatever speed you want.
   replyBuff = new textBuff(80);    // Set the size of your text buffer.
                                    // Watch out UNO users, you don't have a lot of RAM to play with.

}


// Send the text buffer out the serial port to see.
void echoTheText(void) {

   while(!replyBuff->empty()) {
      Serial.print(replyBuff->readChar());
   }
}


void loop(void) {

  timeObj timer(10);

  if (Serial.available()) {                        // If we have an incoming char..
    timer.start();                                 // Start our timer.
    while (!replyBuff->full()&&!timer.ding()) {    // While our buffer is NOT full and the timer has not expired..   
      while (Serial.available()) {                 // While we have something to grab from the serial port..
        replyBuff->addChar(Serial.read());         // Grab a char and stuff it into our buffer.
        timer.start();                             // We got one, reset the timer.
      }
    }
  }
  echoTheText();                                   // If there's something in the buffer, lets see it!
}
