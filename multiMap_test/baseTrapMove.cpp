#include "baseTrapMove.h"


baseTrapMove::baseTrapMove(void)
   :multiMap() { }

   
baseTrapMove::~baseTrapMove(void) {  }


// Units D, D/T, Degrees.
void baseTrapMove::setupMove(double distD,double velocityV,double accelAngA) {

   double   Vtmax;
   double   halfTime;
   double   cruiseTime;

   clearMap();                   // Clear out any old data..
   D  = distD;                   // Units of.. D
   V  = velocityV;               // Units of D per T.
   a  = accelAngA;               // Units of degrees.
   TANa = tan(a);                // Units of.. I don't really know. Do I need to know? dV/dT I think it is.
   Vtmax = sqrt(D * TANa);       // In a right triangle the height = root (1/2Aria * tan(angle)) Our totalDist is 2x the triangle area, hence..
   if (Vtmax<=V) {               // If the height we calculated (Of the triangluar velocity curve) is <= to V..
      halfTime = sqrt(D/TANa);   // We calculate half time for triangle move. (Base of right triangle)
      T = halfTime * 2;          // Total time T is 2 * halfTime. Makes sense..
      addPoint(0,0);             // First point, time 0, velocity 0.
      addPoint(halfTime,Vtmax);  // Second point, half time, max velocity.
      addPoint(2*halfTime,0);    // Third point, end time velocity 0.
   } else {                      // Else we're looking at a trapizodial move.. 
      cruiseTime = V/TANa;       // Gettin fuzzy.. Are we still in percent?
      T = (D/V) + (V/TANa);      // Calculated total travel time.
      addPoint(0,0);             // First point, time 0, velocity 0.
      addPoint(cruiseTime,V);    // Second point, cruise time (Time when we hit cruis speed), max velocity.
      addPoint(T-cruiseTime,V);  // Third point, end of cruising. Time to ramp down.
      addPoint(T,0);             // Last point, we ramp down to zero.
   }
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
