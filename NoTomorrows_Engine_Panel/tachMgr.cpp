#include "tachMgr.h"


// Interrupt handler stuff

volatile  long  pulseCount = 0;

void tachIntHandler(void) { pulseCount++; }



// tachMgr stuff

tachMgr::tachMgr(int teeth,float waitTime)
   : idler(),timeObj(waitTime) {
  
   RPM      = -1;                            // Impossible flag.
   constVal = 60/(teeth*(waitTime/1000.0));  // Do as much math as possbible up front.
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

   RPMUpMapper.addPoint(200,55);
   RPMUpMapper.addPoint(300,38);
   RPMUpMapper.addPoint(400,25.7);
   RPMUpMapper.addPoint(500,21.5);
   RPMUpMapper.addPoint(600,17);
   RPMUpMapper.addPoint(800,12.6);
   RPMUpMapper.addPoint(1000,10.0);
   RPMUpMapper.addPoint(1200,7.8);
   RPMUpMapper.addPoint(1400,6.5);
   RPMUpMapper.addPoint(1500,6);
   RPMUpMapper.addPoint(1600,5.6);
   RPMUpMapper.addPoint(1800,4.85);
   RPMUpMapper.addPoint(2000,4.4);
   RPMUpMapper.addPoint(2200,4.05);
   RPMUpMapper.addPoint(2400,3.72);
   RPMUpMapper.addPoint(2500,3.6);
   RPMUpMapper.addPoint(3000,3.0);
   RPMUpMapper.addPoint(3200,2.83);
   RPMUpMapper.addPoint(3400,2.7);
   RPMUpMapper.addPoint(3500,2.635);
   RPMUpMapper.addPoint(3800,2.39);
   RPMUpMapper.addPoint(4000,2.26);

   RPMDnMapper.addPoint(200,55);
   RPMDnMapper.addPoint(300,38);
   RPMDnMapper.addPoint(400,28);
   RPMDnMapper.addPoint(500,22);
   RPMDnMapper.addPoint(600,18);
   RPMDnMapper.addPoint(800,13.5);
   RPMDnMapper.addPoint(1000,10.5);
   RPMDnMapper.addPoint(1200,8.27);
   RPMDnMapper.addPoint(1400,6.8);
   RPMDnMapper.addPoint(1500,6.3);
   RPMDnMapper.addPoint(1600,5.8);
   RPMDnMapper.addPoint(1800,5);
   RPMDnMapper.addPoint(2000,4.5);
   RPMDnMapper.addPoint(2200,4.16);
   RPMDnMapper.addPoint(2400,3.845);
   RPMDnMapper.addPoint(2500,3.67);
   RPMDnMapper.addPoint(2600,3.555);
   RPMDnMapper.addPoint(2800,3.3);
   RPMDnMapper.addPoint(3000,3.07);
   RPMDnMapper.addPoint(3200,2.88);
   RPMDnMapper.addPoint(3400,2.73);
   RPMDnMapper.addPoint(3500,2.66);
   RPMDnMapper.addPoint(3800,2.41);
   RPMDnMapper.addPoint(4000,2.26);
}


void tachMgr::idle(void) {

   long  count;
   float newRPM;
   
   if (ding()) {
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


void tachMgr::setTach(float inRPM) {

   float period;

   if (round(inRPM*100) == round(RPM*100)) return;
   if (inRPM>RPM) {
      period = RPMUpMapper.map(inRPM);
   } else {
      period = RPMDnMapper.map(inRPM);
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
