#include <triDBase.h>


// This rotates a point by angle radians around the x,y axis. It's been checked out and it
// works!
void rotate(twoDPoint* ptA,double angle,twoDPoint* center) {

	double	mag;
	double	radF;
	
	if (center) {
		ptA->x = ptA->x - center->x;
		ptA->y = ptA->y - center->y;
		rotate(ptA,angle,NULL);
		ptA->x = ptA->x + center->x;
		ptA->y = ptA->y + center->y;
		return;
	}
	if (ptA->x||ptA->y) {												// IF its not a 0,0 point..
		mag = sqrt((ptA->x * ptA->x) + (ptA->y * ptA->y));		// Calc magnatude.
		if (ptA->x<0.001&&ptA->x>-0.001) {							// As y/x->infinity..
			radF = M_PI/2.0 + angle;									// Angle goes to Pi/2 Radians.
		} else {
			radF = atan(ptA->y/ptA->x)+angle;							// Calc resulting angle.
		}
		ptA->x = mag * cos(radF);				// Calc new x.
		ptA->y = mag * sin(radF);				// Clac new y.
	}
}


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
triDPoint	triDPointScale(triDPoint* ptA,double scale) {

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


// What's the smallest x y & z values in this thing?
triDPoint leastValues(triDTriangle* triangle) {

	triDPoint	res;
	
	res.x = triangle->corners[0].x;
	if (triangle->corners[1].x < res.x) {
		res.x = triangle->corners[1].x;
	}
	if (triangle->corners[2].x < res.x) {
		res.x = triangle->corners[2].x;
	}
	
	res.y = triangle->corners[0].y;
	if (triangle->corners[1].y < res.y) {
		res.y = triangle->corners[1].y;
	}
	if (triangle->corners[2].y < res.y) {
		res.y = triangle->corners[2].y;
	}
	
	res.z = triangle->corners[0].z;
	if (triangle->corners[1].z < res.z) {
		res.z = triangle->corners[1].z;
	}
	if (triangle->corners[2].z < res.z) {
		res.z = triangle->corners[2].z;
	}
	return res;
}


// Offset this triangle by subbing this point to each vertex.
void subtractPoint(triDTriangle* triangle,triDPoint* pt) {
	
	for (byte i=0;i<3;i++) {
		triangle->corners[i].x = triangle->corners[i].x - pt->x;
		triangle->corners[i].y = triangle->corners[i].y - pt->y;
		triangle->corners[i].z = triangle->corners[i].z - pt->z;
	}
}


// Offset this triangle by adding this point to each vertex.
void addPoint(triDTriangle* triangle,triDPoint* pt) {

	for (byte i=0;i<3;i++) {
		triangle->corners[i].x = triangle->corners[i].x + pt->x;
		triangle->corners[i].y = triangle->corners[i].y + pt->y;
		triangle->corners[i].z = triangle->corners[i].z + pt->z;
	}
}

	
void printTriDTriangle(triDTriangle* triangle) {

	Serial.println("-------");
	Serial.println("triDTriangle");
	Serial.print("corners[0]: ");
	printTriDPt(&(triangle->corners[0]));
	Serial.print("corners[1]: ");
	printTriDPt(&(triangle->corners[1]));
	Serial.print("corners[2]: ");
	printTriDPt(&(triangle->corners[2]));
	Serial.println("-------");
}


triDRotation setRotation(double xVal,double yVal,double zVal) {

	triDRotation	res;
	
	res.xRad = xVal;
	res.yRad = yVal;
	res.zRad = zVal;
	return res;
}