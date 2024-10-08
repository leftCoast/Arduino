#include <triDBase.h>



// rotatePolar Got this from Tim2000 on the Wokwi server of Discord.
void rotate(twoDPoint *point,double angle_rad, twoDPoint *center) {
  
  double magnitude;
  double theta_rad;

  // Subtract the offset if available.
  if (center != nullptr)
  {
    point->x -= center->x;
    point->y -= center->y;
  }

  // Convert from Cartesian to Polar form.
  magnitude = sqrt((point->x * point->x) + (point->y * point->y));
  theta_rad = atan2(point->y, point->x);

  // Add the angle
  theta_rad += angle_rad;

  // Convert back to Cartesian form.
  point->x = magnitude * cos(theta_rad);
  point->y = magnitude * sin(theta_rad);

  // Add the offset if available.
  if (center != nullptr)
  {
    point->x += center->x;
    point->y += center->y;
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