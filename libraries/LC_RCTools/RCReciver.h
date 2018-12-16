#ifndef RCReciever_h 
#define RCReciever_h

#include <arduino.h>

#define TIMEOUT_MICROS 25000
#define MAX_PINS 9                // You need 0-8 handlersX functions in the cpp file. (Hack)


class reciverPin {
   
	public: 
				reciverPin(int inPin);
	virtual	~reciverPin(void);
   
				unsigned long	pinResult(void);   // What's the last value read?
				//void			dataDump(void);  // uncomment this and its method to dump to serial.
				void				handeler(void);
      
   protected:
				void          attachHandler(byte state);
				byte          pin;
				byte          index;
				byte          currentState;
				unsigned long startMicros;
				unsigned long endMicros;                      
				unsigned long result;
};

extern reciverPin* pinList[MAX_PINS];

#endif // RCReciever_h

