#include "comPlayer.h"


// **************************************************
//                Example user command.
//                 Prints out a string.
// **************************************************


class myComObj :  public comObj {

      public:
                     myComObj(char* msg); // We'll need a constructor to greate our command.
      virtual        ~myComObj(void);     // and a destructor, this will be living in dynamec RAM. Must be able to delete it.

      virtual  void  doCommand(void);     // When the player finds this in its queue. It'll call this as its action.
      
               char* mMsg;                // In this case we print a string, this is where we'll store that string.
};


// Our constructor
myComObj::myComObj(char* msg) {

   mMsg = NULL;                              // Init the string pointer to NULL. (Important)
   if (resizeBuff(strlen(msg)+1,&mMsg)) {    // Count the chars in the string, add one, and see if we can allocate that much RAM.
      strcpy(mMsg,msg);                      // We got the RAM? Cool, save a copy of the string.
   }
}


// Destructor. pointers initialized with NULL can use resizeBuff() for
// both allocation and deallocation.
myComObj::~myComObj(void) { resizeBuff(0,&mMsg); }


// This is our custom command method. We fill this out to do our action.
void myComObj::doCommand(void) {

   if (mMsg) {                // If we have an allocated message.. 
      Serial.println(mMsg);   // We print it out with a line feed.
   }
}



// **************************************************
//              Example program starts here
//         prints out a list of strings over time.
//           And blinks the LED at the same time.
//                      Cool huh?      
// **************************************************

#define LED_PIN   13 // we're going to blink an LED.

comPlayer myPlayer;  // Our command player. We can have more than one if we need to run multiple things at a time.


// A little helper function to make adding commands easy peasy. All strings can be added with a time for them to
// hold on the screen before writing the next one. So here we pass in a string and a hold time in miliseconds.
void addStr(char* aStr,int holdMS) {

   myComObj*   strComCom;
   comObj*     sleepCom;

   strComCom = new myComObj(aStr);     // We create one of our custom message objects with the passed in String.
   myPlayer.addCom(strComCom);         // We add it to the player. (The player will delete it when the time comes.)
   if (holdMS>0) {                     // If we have a positive hold time..
      sleepCom = new comObj(holdMS);   // We create a default command object with the hold time as a wait timer.
      myPlayer.addCom(sleepCom);       // Add the wait timer to the player's list.
   }
}


void setup() {

   addStr("Hello!",500);      // Using the addStr() function we wrote, we add the strings and wait commands.
   addStr("How",200);
   addStr("are",200);
   addStr("you?",600);
   addStr("Big guy.",500);
   addStr("",1500);
   myPlayer.repeat(true);     // We want this to repeat forever.

   pinMode(LED_PIN, OUTPUT);      // Now, lets set up the LED for blinking.
}


void loop() {

   idle();
   digitalWrite(LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
   sleep(300);                  // Wait for a second. LOOK! Not using delay(). Use sleep() So everything keeps running.
   digitalWrite(LED_PIN, LOW);   // turn the LED off by making the voltage LOW
   sleep(1000);                  // Again, use sleep() NOT delay().
}