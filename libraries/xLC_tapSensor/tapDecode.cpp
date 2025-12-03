/*
#include "tapDecode.h"


//*******************************************************
//								tapRule
//*******************************************************

tapRule::tapRule()
	: linkListObj() {  }
		
tapRule::~tapRule() {  }



//*******************************************************
//								tapDecode
//*******************************************************


tapDecode::tapDecode(int sensorPin,float sumTime,int numData)
	: tapSensor(sensorPin,sumTime,numData),
	linkList(); { index = NULL; }
	
	
tapDecode::~tapDecode(void) {  }

					
void tapDecode::addTap(float threshold) {
	
	tapRule* newRule;
	
	newRule = new tapRule();
	if (newRule) {
		newRule->tap = true;
		newRule->threshold = threshold;
		addToEnd(newRule);
	}
}


void tapDecode::addPause(float threshold,float minMs, float maxMs) {

	tapRule* newRule;
	
	newRule = new tapRule();
	if (newRule) {
		newRule->tap = false;
		newRule->threshold = threshold;
		newRule->minMs = minMs;
		newRule->minMs = maxMs;
		addToEnd(newRule);
	}
}


void tapDecode::addPause(float threshold,float minMs) {

	tapRule* newRule;
	
	newRule = new tapRule();
	if (newRule) {
		newRule->tap = false;
		newRule->threshold = threshold;
		newRule->minMs = minMs;
		newRule->minMs = 0;
		addToEnd(newRule);
	}
}


void tapDecode::begin(void) { index = theList; }


void tapDecode::end(void) { index = NULL; }


bool ruleBroken(void) {

}


bool ruleMet(void) {

}


void advance(void) {

}


void tapDecode::idle(void) {
	
	tapSensor::idle();
	if (index) {								// If not NULL, the hunt is afoot!
		if (ruleBroken()) begin();			// Broke a rule? Start over!
		else if (ruleMet()) advance();	// We met this rule, we advance!
	}
}


void tapDecode::doAction(void) {

}

*/
