#include "RCReciver.h"

#ifndef PinChangeInt_h
#define LIBCALL_PINCHANGEINT
#include <PinChangeInt.h>
#endif


reciverPin* pinList[MAX_PINS] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

// hardcoded induvidual handler functions. This is so sad.
void handler0(void) { if (pinList[0]) pinList[0]->handeler(); }
void handler1(void) { if (pinList[1]) pinList[1]->handeler(); }
void handler2(void) { if (pinList[2]) pinList[2]->handeler(); }
void handler3(void) { if (pinList[3]) pinList[3]->handeler(); }
void handler4(void) { if (pinList[4]) pinList[4]->handeler(); }
void handler5(void) { if (pinList[5]) pinList[5]->handeler(); }
void handler6(void) { if (pinList[6]) pinList[6]->handeler(); }
void handler7(void) { if (pinList[7]) pinList[7]->handeler(); }
void handler8(void) { if (pinList[8]) pinList[8]->handeler(); }


reciverPin::reciverPin(int inPin) { 

   pin = inPin;
   startMicros = 0;
   endMicros = micros();             // This sets up the timeout calculator.
   result = 0;
   index = 0;
   while(index<MAX_PINS && pinList[index]!=NULL) index++;
   if (index<MAX_PINS) {                // Means we found one.
      pinMode(pin, INPUT);              // Make it an input pin.
      digitalWrite(pin, HIGH);          // Use the internal pullup resistor
      pinList[index] = this;            // Stuff a pointer to us in the array.
      attachHandler(RISING);            // Looking for a high pulse to start off with.
   }
}


reciverPin::~reciverPin(void) {
   
   if (index<MAX_PINS) {                 // Means we have one.
      PCintPort::detachInterrupt(pin);   // Detach the inturupt
      pinList[index]  = NULL;            // Take us out of the list.
   }
}


unsigned long reciverPin::pinResult(void) {
  
   if (micros()>(endMicros + TIMEOUT_MICROS)||!startMicros) // Timed out or never started..
      return 0;                                             // No pulses, 0 means we lost contact.
    else 
       return result;
}

/*
void reciverPin::dataDump(void) {
   
   Serial.print("pin = "); Serial.println(pin);
   Serial.print("index = "); Serial.println(index);
   Serial.print("currentState = "); 
   switch(currentState) {
      case RISING : Serial.println("RISING"); break;
      case FALLING : Serial.println("FALLING"); break;
      case CHANGE : Serial.println("CHANGE"); break;
      default : Serial.println("INVALID"); break;
   };
   Serial.print("startMicros = "); Serial.println(startMicros);
   Serial.print("endMicros = "); Serial.println(endMicros);
   Serial.print("result = "); Serial.println(result);
   Serial.println();
}
*/

void reciverPin::handeler(void) {
    switch(currentState) {
       case RISING : 
          startMicros = micros();
          PCintPort::detachInterrupt(pin);
          attachHandler(FALLING);
       break;
       case FALLING :
          endMicros = micros();
          PCintPort::detachInterrupt(pin);
          attachHandler(RISING);
          if (endMicros>startMicros) 
             result = endMicros-startMicros;
       break;
    };  
}
   
   // Why is case 0 different? I don't remember.   
void reciverPin::attachHandler(byte state) {

   switch(index) {
      case 0 : PCintPort::attachInterrupt(pin, &handler0, state); break;
      case 1 : PCintPort::attachInterrupt(pin, handler1, state); break;
      case 2 : PCintPort::attachInterrupt(pin, handler2, state); break;
      case 3 : PCintPort::attachInterrupt(pin, handler3, state); break;
      case 4 : PCintPort::attachInterrupt(pin, handler4, state); break;
      case 5 : PCintPort::attachInterrupt(pin, handler5, state); break;
      case 6 : PCintPort::attachInterrupt(pin, handler6, state); break;
      case 7 : PCintPort::attachInterrupt(pin, handler7, state); break;
      case 8 : PCintPort::attachInterrupt(pin, handler8, state); break;
   };
   currentState = state;
}


