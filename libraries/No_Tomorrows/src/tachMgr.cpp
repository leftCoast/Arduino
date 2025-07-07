#include "tachMgr.h"


// Interrupt handler stuff

volatile  long  pulseCount = 0;

void tachIntHandler(void) { pulseCount++; }



// tachMgr stuff

tachMgr::tachMgr(int teeth,float waitTime)
   : idler(),timeObj(waitTime) {
  
   RPM      = -1;											// Impossible flag.
   constVal = 60/(teeth*(waitTime/waitTime));	// Do as much math as possbible up front.
   runAuto = true;										// As opposed to manually setting values.
}


tachMgr::~tachMgr(void) { if (tach) delete(tach); }


bool tachMgr::begin(int inPin,int outPin) {

   tach  = new blinker(outPin);
   if (tach) {
      setupMappers();
      setTach(0);
      attachInterrupt(digitalPinToInterrupt(inPin), tachIntHandler, RISING);
      hookup();
      return true;
   }
   return false;
}


void tachMgr::setupMappers(void) {

   RPM_Mapper.addPoint(0,100);
   RPM_Mapper.addPoint(100,98.5);
   RPM_Mapper.addPoint(200,47);
   RPM_Mapper.addPoint(300,30);
   RPM_Mapper.addPoint(400,23);
   RPM_Mapper.addPoint(500,19);
   RPM_Mapper.addPoint(700,13.5);
   RPM_Mapper.addPoint(1000,9.8);
   RPM_Mapper.addPoint(1500,6.7);
   RPM_Mapper.addPoint(1700,5.9);
   RPM_Mapper.addPoint(1800,5.55);
   RPM_Mapper.addPoint(2000,5.03);
   RPM_Mapper.addPoint(2300,4.42);
   RPM_Mapper.addPoint(2500,4.03);
   RPM_Mapper.addPoint(2700,3.75);
   RPM_Mapper.addPoint(2800,3.6);
   RPM_Mapper.addPoint(3000,3.37);
   RPM_Mapper.addPoint(3500,2.89);
   RPM_Mapper.addPoint(4000,2.52);
}


void tachMgr::setAuto(bool onOff) { runAuto = onOff; }


void tachMgr::idle(void) {

   long  count;
   float newRPM;
   
   if (runAuto && ding()) {
      count = pulseCount;
      pulseCount = 0;
      if (count) {
         newRPM = count * constVal;
         setTach(newRPM);
      } else {
         setTach(0);
      }
      stepTime();
   }
}


// Raw jam it in kinda' thing. For debugging new tachs.
void tachMgr::setMS(float inMS) {

	tach->setPeriod(inMS);
	tach->setPercent(50);
	if (!tach->running()) {
		tach->setOnOff(true);
	}
}


void tachMgr::setTach(float inRPM) {

   float period;

   if (runAuto && (round(inRPM*100) == round(RPM*100))) return;
   period = RPM_Mapper.map(inRPM);
   if (!runAuto) {
   	Serial.print("Ms = ");
   	Serial.println(period,2);
   }
   if (inRPM>=200) {
      tach->setPeriod(period);
      tach->setPercent(50);
      if (!tach->running()) {
         tach->setOnOff(true);
      }
   } else {
      tach->setOnOff(false);
   }
   RPM = inRPM;
}


float tachMgr::getTach(void) { return RPM; }

