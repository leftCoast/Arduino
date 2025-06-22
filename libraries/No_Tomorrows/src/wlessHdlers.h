#ifndef wlessHdlers_h
#define wlessHdlers_h

#include <NMEA2kBase.h>
#include <globalPos.h>


// ************* rngBearingHdlr *************


class rngBearingHdlr :	public msgHandler {

	public:
				rngBearingHdlr(netObj* inNetObj);
	virtual	~rngBearingHdlr(void);
	
	virtual  bool  		handleMsg(message* inMsg);
				float 		getRangeKn(void);
            float 		getBearingT(void);
            float 		getBearingM(void);
            
				
				globalPos	mark;
				globalPos	fix;
				float			rangeKn;
				float 		bearingT;
				float			bearingM;
};

				
// ************* barometerHdlr *************



class barometerHdlr :	public msgHandler {

   public:
            barometerHdlr(netObj* inNetObj);
            ~barometerHdlr(void);
          
   virtual  bool  handleMsg(message* inMsg);
            float getAirTemp(void);
            float getInHg(void);

            float       degF;
            runningAvg* inHgSmooth;
            runningAvg* inHg20Min;
            timeObj*    minuteTimer;
            float       inHg;
            float       deltaHg;       // Change in inHg per hour.
};




#endif