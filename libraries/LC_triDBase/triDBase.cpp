#include <triDBase.h>


// res = ptA + ptB
triDPoint	triDPointAdd(triDPoint* ptA,triDPoint* ptB) {

	triDPoint	res;
	
	res.x = ptA->x + ptB->x;
	res.y = ptA->y + ptB->y;
	res.z = ptA->z + ptB->z;
	return res;
}


// res = ptA - ptB
triDPoint	triDPointSub(triDPoint* ptA,triDPoint* ptB) {

	triDPoint	res;
	
	res.x = ptA->x - ptB->x;
	res.y = ptA->y - ptB->y;
	res.z = ptA->z - ptB->z;
	return res;
}

// res = ptA * ptB
triDPoint	triDPointMult(triDPoint* ptA,triDPoint* ptB) {

	triDPoint	res;
	
	res.x = ptA->x * ptB->x;
	res.y = ptA->y * ptB->y;
	res.z = ptA->z * ptB->z;
	return res;
}

// res = ptA * scale
triDPoint	triDPointScale(triDPoint* ptA,float scale) {

	triDPoint	res;
	
	res.x = ptA->x * scale;
	res.y = ptA->y * scale;
	res.z = ptA->z * scale;
	return res;
}


void printTriDPt(triDPoint* pt) {

	Serial.print("triDPoint x,y,z: ");
	Serial.print(pt->x);Serial.print(", ");
	Serial.print(pt->y);Serial.print(", ");
	Serial.println(pt->z);
}


triDPoint getCentPt(triDTriangle* triangle) {

	triDPoint res;
	
	res.x = (triangle->corners[0].x + triangle->corners[1].x + triangle->corners[2].x)/3.0;
	res.y = (triangle->corners[0].y + triangle->corners[1].y + triangle->corners[2].y)/3.0;
	res.z = (triangle->corners[0].z + triangle->corners[1].z + triangle->corners[2].z)/3.0;
	return res;
}

	
void printTriDTriangle(triDTriangle* triangle) {

	Serial.println("-------");
	Serial.println("triDTriangle, corners[3]");
	Serial.print("corners[0]: ");
	printTriDPt(&(triangle->corners[0]));
	Serial.print("corners[1]: ");
	printTriDPt(&(triangle->corners[1]));
	Serial.print("corners[2]: ");
	printTriDPt(&(triangle->corners[2]));
	Serial.println("-------");
}