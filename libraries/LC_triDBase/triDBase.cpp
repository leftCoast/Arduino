#include <triDBase.h>

void printTriDPt(triDPoint* pt) {

	Serial.print("triDPoint x,y,z: ");
	Serial.print(pt->x);Serial.print(", ");
	Serial.print(pt->y);Serial.print(", ");
	Serial.println(pt->z);
}

