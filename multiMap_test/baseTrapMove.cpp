#include "baseTrapMove.h"

double deg_2_rad(double x) { return x*M_PI/180; }


baseTrapMove::baseTrapMove(void)
   :multiMap() { }

   
baseTrapMove::~baseTrapMove(void) {  }


// Units D, D/T, Degrees.
void baseTrapMove::setupMove(double distD,double velocityV,double accelAngA) {

   double   Vtmax;
   double   halfTime;
   double   cruiseTime;

   //Serial.print("setupMove  distD : ");Serial.print(distD);Serial.print("   velocityV : ");Serial.print(velocityV);Serial.print("   accelAngA : ");Serial.println(accelAngA);
   clearMap();                   // Clear out any old data..
   D  = distD;                   // Units of.. D
   V  = velocityV;               // Units of D per T.
   a  = accelAngA;               // Units of degrees.
   TANa = tan(deg_2_rad(a));     // Units of.. I don't really know. Do I need to know? dV/dT I think it is.
   Vtmax = sqrt(D * TANa);       // In a right triangle the height = root (1/2Aria * tan(angle)) Our totalDist is 2x the triangle area, hence..
   //Serial.print("TANa : ");Serial.println(TANa);
   //Serial.print("Calculated Vtmax : ");Serial.println(Vtmax);
   if (Vtmax<=V) {               // If the height we calculated (Of the triangluar velocity curve) is <= to V..
      //Serial.println("Triangle move : ");
      halfTime = sqrt(D/TANa);   // We calculate half time for triangle move. (Base of right triangle)
      T = halfTime * 2;          // Total time T is 2 * halfTime. Makes sense..
      addPoint(0,0);             // First point, time 0, velocity 0.
      addPoint(halfTime,Vtmax);  // Second point, half time, max velocity.
      addPoint(T,0);             // Third point, end time velocity 0.
   } else {                      // Else we're looking at a trapizodial move.. 
      //Serial.println("Trapizoidal move : ");
      cruiseTime = V/TANa;       // Gettin fuzzy.. Are we still in percent?
      //Serial.print("cruiseTime : ");Serial.println(cruiseTime);
      T = (D/V) + (V/TANa);      // Calculated total travel time.
      addPoint(0,0);             // First point, time 0, velocity 0.
      addPoint(cruiseTime,V);    // Second point, cruise time (Time when we hit cruis speed), max velocity.
      addPoint(T-cruiseTime,V);  // Third point, end of cruising. Time to ramp down.
      addPoint(T,0);             // Last point, we ramp down to zero.
   }
   //Serial.print("Calculated time : ");Serial.println(T);
}


double baseTrapMove::position(double timeT,bool forward) {

   if (!isEmpty()) {
      if (forward) {
         return integrate(0,timeT);
      } else {
         return integrate(T-timeT,T);
      }
   } else {
      return 0;
   }
}
