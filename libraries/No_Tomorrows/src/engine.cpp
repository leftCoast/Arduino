#include <engine.h>
#include <strTools.h>


#define ENG_DEVICE_ID		7484				// You get 21 bits. Think serial number.
#define ENG_DEFAULT_ADDR	47					// This initial value will be set using the serial monitor.
#define ENG_DEVICE_SYSTEM	DEV_SYSTEM_PROPEL
#define ENG_DEVICE_FUNCT	DEV_FUNC_ENG_GATEWAY


engine::engine(void) 
: NMEA2kBase(ENG_DEVICE_ID,ENG_DEVICE_SYSTEM,ENG_DEVICE_FUNCT) {

	ourTachMgr		= NULL;
	oilPSI_Sender	= NULL;
	tempSender		= NULL;
	ourBeeper		= NULL;
	engHdler			= NULL;
	fuelHdlr			= NULL;
	updateTimer		= NULL;
}


engine::~engine(void) {

	if (ourTachMgr) delete(ourTachMgr);
	if (oilPSI_Sender) delete(oilPSI_Sender);
	if (tempSender) delete(tempSender);
	if (ourBeeper) delete(ourBeeper);
	if (engHdler) delete(engHdler);
	if (engHdlerII) delete(engHdlerII);
	if (fuelHdlr) delete(fuelHdlr);
	if (updateTimer) delete(updateTimer);
}


void engine::setup() {
   
   NMEA2kBase::setup();
   ourTachMgr = new tachMgr(TEETH_ROTATE,WAIT_TIME_MS);
   if (!ourTachMgr) {
   	Serial.println("Can not allocate Tach!");
      while(1);
   }
   if (!ourTachMgr->begin(TACH_IN_PIN,TACH_OUT_PIN)) {
      Serial.println("Can't start up Tach!");
      while(1);
   }
   updateTimer		= new timeObj(100);
   oilPSI_Sender	= new mechButton(OIL_PIN);
   tempSender		= new mechButton(TEMP_PIN);
   ourBeeper		= new blinker(BEEP_OUT_PIN,ENG_ALARM_PULSE,ENG_ALARM_PERIOD);
   if (!oilPSI_Sender||!tempSender||!ourBeeper||!updateTimer) {
   	Serial.println("Allocations failed!");
      while(1);
   }
}

		
void 	engine::loop() {

	float		currentRPM;
	alarms	alarmState;
	
	NMEA2kBase::loop();															// Let our ancestors do their thing.
	alarmState.lowOilPSI = false;												// Clear out the alarm block.
	alarmState.overTemp = false;												//
	if (!oilPSI_Sender->getState() || !tempSender->getState()) {	// If switches say alarm?!
		if (ourTachMgr->getTach()>MIN_RPM) {								// And the engine is running!
			if (!ourBeeper->running()) {										// And, and.. The alarm has NOT been turned on!
				ourBeeper->setOnOff(true);										// Sound the alarm!
				alarmState.lowOilPSI = oilPSI_Sender->getState();		// Set up the alarm block.
				alarmState.overTemp = tempSender->getState();			//
				engHdlerII->setAlarms(&alarmState);							// On change, send the alarm block.
			}																			//
		} else {																		// Else.. Engine is off.
			if (ourBeeper->running()) {										// If its beeping..
				ourBeeper->setOnOff(false);									// Shut it off.
				engHdlerII->setAlarms(&alarmState);							// On change, change the network.
			}																			//
		}																				//
	} else {																			// Else, we don't want the alarm on..
		if (ourBeeper->running()) {											// If its beeping..
			ourBeeper->setOnOff(false);										// Shut it off.
			engHdlerII->setAlarms(&alarmState);								// On change, send the alarm block.
		}																				//
	}																					//
	if (updateTimer->ding()) {													// When our timer expires..
		currentRPM = ourTachMgr->getTach();									// Get the tach info.
		engHdler->setRPM(currentRPM);											// Send it out the wire.
		updateTimer->stepTime();												// And, reset the timer.
	}																					//
}



// NMEA & Command stuff ********************************************


bool	engine::addNMEAHandlers(void) {

	engHdler = new engParam(llamaBrd);
	engHdlerII = new engParamII(llamaBrd);
	fuelHdlr = new fluidLevelObj(llamaBrd);
	if (engHdler && engHdlerII && fuelHdlr) {
		llamaBrd->addMsgHandler(engHdler);
		llamaBrd->addMsgHandler(engHdlerII);
		llamaBrd->addMsgHandler(fuelHdlr);
		return true;
	} else {
		if (engHdler) {
			delete(engHdler);
			engHdler = NULL;
		}
		if (engHdlerII) {
			delete(engHdlerII);
			engHdlerII = NULL;
		}
		if (fuelHdlr) {
			delete(fuelHdlr);
			fuelHdlr = NULL;
		}
	}
	return false;
}


void 	engine::addCommands(void) {

	NMEA2kBase::addCommands();
	cmdParser.addCmd(setRPM,"setrpm");
	cmdParser.addCmd(setRPM,"rpm");
	cmdParser.addCmd(setMs,"ms");
	cmdParser.addCmd(setFuel,"setfuel");
	cmdParser.addCmd(readFuel,"readfuel");
	cmdParser.addCmd(setAuto,"auto");
}


void 	engine::checkAddedComs(int comVal) {

	switch(comVal) {
		case setRPM		: doSetRPM();		break;
		case setMs		: doSetMS();		break;
		case setFuel	: doSetFuel();		break;
		case readFuel	: doReadFuel();	break;
		case setAuto	: doSetAuto();		break;
		default			: printHelp();		break;
	}
}


void engine::printHelp(void) {

	NMEA2kBase::printHelp();
	Serial.println("                                   Engine panel commands.");
	Serial.println("           ----------------------------------------------------------------------");
	Serial.println("RPM manually sets the RPM gauge to an RPM level.");
	Serial.println("setFuel followed by a level percent, sends the fuel level out the NEMA network.");
	Serial.println("readFuel reads the fuel level being sent in by the NEMA network.");
	Serial.println("auto toggles auto reading and broacasting RPM values");
}
	
void engine::doSetRPM(void) {
	
	float RPMValue;
	
	if (cmdParser.numParams()==1) {
		RPMValue = atof(cmdParser.getNextParam());
		ourTachMgr->setTach(RPMValue);
		Serial.print("Tach set to : ");
		Serial.print(RPMValue);
		Serial.println(" RPM");
	} else {
		Serial.println("Well, we were looking for a RPM value to set it to.");
	}
}


void engine::doSetMS(void) {

	float	ms;
	
	if (cmdParser.numParams()==1) {
		ms = atof(cmdParser.getNextParam());
		ourTachMgr->setMS(ms);
		Serial.print("MS set to ");
		Serial.println(ms,2);
	} else {
		Serial.println("Well, we were looking for a MS value to set it to.");
	}
}


void engine::doSetFuel(void) {

	float level;
	
	level = 0;
	if (cmdParser.numParams()==1) {
		level = atof(cmdParser.getNextParam());
		fuelHdlr->setLevel(level);
	}
	Serial.print("Fuel set to ");
	Serial.println(level);
}


void engine::doReadFuel(void) {
	
	Serial.print("Fuel level : ");
	Serial.print(fuelHdlr->getLevel());
}


void engine::doSetAuto(void) {
	 
	if (cmdParser.numParams()==0) {
		ourTachMgr->setAuto(!ourTachMgr->runAuto);
		Serial.print("Auto set to ");
		if (ourTachMgr->runAuto) {
			Serial.println("on");
		} else {
			Serial.println("off");
		}
	}
}


	