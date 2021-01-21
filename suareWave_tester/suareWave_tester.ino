 /************************************************************************************
*************************************************************************************
                          1/20/2021 Damn pandemic still going on.
                          At least Trump's finally out of office.
                                    
This is a really simple test/demo program for the Left Coast squareWave code.
In the serial monitor..
      
      Type : 
      wave on    to turn on the squarewave. (Pin 13)
      wave off   to turn off the squarewave.
      per 14.5   to set the period to 14.5 ms.
      pulse 10   to set the pulse width to 10 ms
      % 25       to set the pulse width to 25%
      block on   to set code blocking on while pulse is high.
      ?          Displays this list of commands on the serial monitor.
      Anything else will result in an output of the list of
      commands.
      
In order to run this you will need two libraries : 
LC_baseTools
LC_lilParser

Both can be found and installed using the Arduino IDE's library manager.

Or here : https://github.com/leftCoast/LC_baseTools
And : https://github.com/leftCoast/LC_lilParser

*************************************************************************************
*************************************************************************************/

#include <squareWave.h>
#include <lilParser.h>

#define  DEF_PERIOD_MS  1
#define  DEF_PULSE_MS   0.045
#define  OUT_PIN        13

// **************************
//     The outWave class
// **************************

// squareWaves only do the timing. If you want the signal out on a pin, you
// need to inherit squareWave and build you own class that control the pin.
// This is pretty straigh forward because its how squareWave was designed
// to be used. class outWave is just this kind of thing.

class outWave :   public squareWave {

   public:
               outWave(int outPin);
   virtual     ~outWave(void);
   
   virtual  void  setOnOff(bool onOff);
   virtual  void  pulseOn(void);
   virtual  void  pulseOff(void);

            int   mPin;
            bool  init;
};


outWave::outWave(int outPin)
   : squareWave(DEF_PERIOD_MS,DEF_PULSE_MS) { 
      
   mPin = outPin;
   init = false;
}

   
outWave::~outWave(void) {  }

   
void outWave::setOnOff(bool onOff) {

   if (!init) {
      pinMode(mPin,OUTPUT);
      init = true;
   }
   squareWave::setOnOff(onOff);
}


void outWave::pulseOn(void) { digitalWrite(mPin,HIGH); }


void outWave::pulseOff(void){ digitalWrite(mPin,LOW); }



// **************************
// Main program begins here.
// **************************


outWave     ourWave(OUT_PIN);
lilParser   ourParser;
enum commands {   noCommand,  // ALWAYS start with noCommand. Or something simlar.
                  waveOnOff,      
                  setPeriod,
                  setPulse,
                  setPercent,
                  blockingOnOff,
                  readSettings
                  };          


void setup() {

   Serial.begin(57600);                         // Fire up our serial port.
   ourWave.setOnOff(true);                      // Fire up our swiare wave generator.
   ourParser.addCmd(waveOnOff,"wave");          // Add all the commands into our parser.
   ourParser.addCmd(setPeriod,"per");
   ourParser.addCmd(setPulse,"pulse");
   ourParser.addCmd(setPercent,"%");
   ourParser.addCmd(blockingOnOff,"block");
   ourParser.addCmd(readSettings,"settings");
}


void loop() {

   char  inChar;
   int   command;

      idle();                                                  // Let anyone in the back ground do their thing.
   if (Serial.available()) {                                   // If serial has some data..
      inChar = Serial.read();                                  // Read out a charactor.
      Serial.print(inChar);                                    // If using development machine, echo the charactor.
      command = ourParser.addChar(inChar);                     // Try parsing what we have.
      switch (command) {                                       // Check the results.
         case noCommand       : break;                         // Nothing to report, move along.
         case waveOnOff       : handleOnOff();     break;      // The rest are just command handlers. (See below)
         case setPeriod       : handlePeriod();    break;  
         case setPulse        : handlePulse();     break;
         case setPercent      : handlePercent();   break;
         case blockingOnOff   : handleBlocking();  break;
         case readSettings    : handleSettings();  break;
         default              : showHelp();        break;
      }
   }
}



// **************************
//      Command handlers
// **************************


// I'm sure there's a c function that'll up or dn case a string.
// But I'm tired of looking for it. This'll do for now.
void dnCase(char* aStr) {
   
   int i=0;
   
   while (aStr[i]) {
      aStr[i] = tolower(aStr[i]);
      i++;
   }
}

  
void handleOnOff(void) {
   
   char* charBuff;                              // A pointer for the on/off text.
   
   if (ourParser.numParams()) {                 // If they typed in somethng past the command.
      charBuff = ourParser.getParamBuff();      // We get the first parameter.
      dnCase(charBuff);                         // Make it case insensitive.
      Serial.print("Wave gen set to : ");       // Output heading.
      if (!strcmp(charBuff,"on")) {             // If they typed on..
         ourWave.setOnOff(true);                // Turn the wave generator on.
         Serial.println("on");                  // Tell em what we did.
      } else {                                  // Else they didn't type on?
         ourWave.setOnOff(false);               // Then tun it off.
         Serial.println("off");                 // Tell em what we did.
      }
      free(charBuff);                           // Dump the parameter buffer.
   }
}



void handlePeriod(void) {
  
   char* charBuff;                              // A pointer for the value text.
   float period;
   
   if (ourParser.numParams()) {                 // If they typed in somethng past the command.
      charBuff = ourParser.getParamBuff();      // We get the first parameter.
      period = atof(charBuff);                  // Parse out the number.
      ourWave.setPeriod(period);                // Set the period to that number.
      free(charBuff);                           // Dump the parameter buffer.
      Serial.print("Period set to : ");
      Serial.print(period);
      Serial.println(" ms");
   }
}


void handlePulse(void) {
    
   char* charBuff;                              // A pointer for the value text.
   float pulse;
   
   if (ourParser.numParams()) {                 // If they typed in somethng past the command.
      charBuff = ourParser.getParamBuff();      // We get the first parameter.
      pulse = atof(charBuff);                   // Parse out the number.
      ourWave.setPulse(pulse);                  // Set the period to whatever they typed in.
      free(charBuff);                           // Dump the parameter buffer.
      Serial.print("Pulse set to : ");
      Serial.print(pulse);
      Serial.println(" ms");
   }
}


void handlePercent(void) {
   
   char* charBuff;                              // A pointer for the value text.
   float percent;
   
   if (ourParser.numParams()) {                 // If they typed in somethng past the command.
      charBuff = ourParser.getParamBuff();      // We get the first parameter.
      percent = atof(charBuff);                 // Parse out the number.
      ourWave.setPercent(percent);              // Set the period to whatever they typed in.
      free(charBuff);                           // Dump the parameter buffer.
      Serial.print("Pulse width set to : ");
      Serial.print(percent);
      Serial.println(" %");
   }
}


void handleBlocking(void) {
   
   char* charBuff;                              // A pointer for the on/off text.
   
   if (ourParser.numParams()) {                 // If they typed in somethng past the command.
      charBuff = ourParser.getParamBuff();      // We get the first parameter.
      dnCase(charBuff);                         // Make it case insensitive.
      Serial.print("Blocking set to : ");       // Output heading.
      if (!strcmp(charBuff,"on")) {             // If they typed on..
         ourWave.setBlocking(true);             // Turn on blocking.
         Serial.println("on");                  // Tell em what we did.
      } else {                                  // Else they didn't type on?
         ourWave.setBlocking(false);            // Then turn off blocking.
         Serial.println("off");                 // Tell em what we did.
      }
      free(charBuff);                           // Dump the parameter buffer.
   }
}


void showHelp(void) {
   
   Serial.println("*** Command set ***");
   Serial.println();
   Serial.println("\"wave\" Then \"on\" or \"off\" turns the wave generator on or off.");
   Serial.println("\"per\" Then a number, sets period in milliseconds.");
   Serial.println("\"pulse\" Then a number, sets pulsewidth in milliseconds.");
   Serial.println("\"%\" Then a number, sets pulsewidth to that percentage of the period.");
   Serial.println("\"block\" Then \"on\" or \"off\" turns blocking while pulse is high, on or off.");
   Serial.println("\"settings\" Doesn't work.");
   Serial.println();
   Serial.println("*******************");
   Serial.println();
}


void handleSettings() {

   Serial.println("I told you this was broken.");
}
