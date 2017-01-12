#include "calculator.h"
#include <string.h>
#include <ctype.h>
#include <math.h>


calculator::calculator(void) {

	x = 0;
	y = 0;
	z = 0;
	t = 0;
	for (int i=0;i<10;i++) { sto[i] = 0; }
    fix = 2;
	degrees = true;
    enter = false;
	fixPressed = false;
	stoPressed = false;
	rclPressed = false;
	startEditing();
}


calculator::~calculator(void) { }


void calculator::buttonClick(const char* inButtonID) {
	
	char buttonID[100];
	int i=0;
	
	strcpy(buttonID,inButtonID);
	while (buttonID[i]) {
		buttonID[i] = toupper(buttonID[i]);
		i++;
	}
	
	if (!strcmp(buttonID,"0")) handleNumber(0);
	else if (!strcmp(buttonID,"1")) handleNumber(1);
	else if (!strcmp(buttonID,"2")) handleNumber(2);
	else if (!strcmp(buttonID,"3")) handleNumber(3);
	else if (!strcmp(buttonID,"4")) handleNumber(4);
	else if (!strcmp(buttonID,"5")) handleNumber(5);
	else if (!strcmp(buttonID,"6")) handleNumber(6);
	else if (!strcmp(buttonID,"7")) handleNumber(7);
	else if (!strcmp(buttonID,"8")) handleNumber(8);
	else if (!strcmp(buttonID,"9")) handleNumber(9);
	else if (!strcmp(buttonID,"-X")) x = -x;
	else if (!strcmp(buttonID,".")) { handlePoint(); }
	
	// This is also talked about at the end of this function.
    // Enter is really odd in that it must effect the next keypress.
    // This is why, once called we immedialy exit. Every other keypress
    // will exit out the bottom and clear this flag.
	else if (!strcmp(buttonID,"ENTER")) {
        pushStack();
		point = false;
		editing = false;
        enter = true;
        return;
    }
	
	// Like "ENTER", "FIX" sets a boolean for the next keystroke.
	// So we set boolean flag and bail out.
	// (Its cleared at the end of this function.)
	else if (!strcmp(buttonID,"FIX")) {
        fixPressed = true;
        return;
    }
	
	// Actually, "STO" acts exactly like "FIX" so..
	// Oh, "RCL" too..
	else if (!strcmp(buttonID,"STO")) {
        stoPressed = true;
        return;
    } else if (!strcmp(buttonID,"RCL")) {
        rclPressed = true;
        return;
    }
	
    else if (!strcmp(buttonID,"CLX")||!strcmp(buttonID,"CX")) { handleClx();}
	else if (!strcmp(buttonID,"RDN")) { handleRollDown();}
	else if (!strcmp(buttonID,"+")) { xyFunction(y+x); }
	else if (!strcmp(buttonID,"-")) { xyFunction(y-x); }
	else if (!strcmp(buttonID,"X")) { xyFunction(y*x); }
	else if (!strcmp(buttonID,"/")) { xyFunction(y/x); }
	else if (!strcmp(buttonID,"MOD")) {
		int intX = x;
		int intY = y;
		xyFunction(intX % intY);
	}
	//else if (!strcmp(buttonID,"√X")) { xFunction(sqrt(x)); }
    else if (!strcmp(buttonID,"SRT")) { xFunction(sqrt(x)); }   //Arduino
	else if (!strcmp(buttonID,"Y^X")) { xyFunction(pow(y,x)); }
    else if (!strcmp(buttonID,"X^2")) { xFunction(pow(x,2)); }
	else if (!strcmp(buttonID,"1/X")) { xFunction(1/x); }
	else if (!strcmp(buttonID,"π")) { xFunction(M_PI); }
    else if (!strcmp(buttonID,"PI")) { xFunction(M_PI); }   // arduino
	else if (!strcmp(buttonID,"Y<>X")||(!strcmp(buttonID,"Y~X"))) {
		double temp;
		temp = x;
		x = y;
		y = temp;
		editing = false;
	}
	else if (!strcmp(buttonID,"SIN")) {
		if (degrees) {
			xFunction(sin(deg_2_rad(x)));
		} else {
			xFunction(sin(x));
		}
    }
	else if (!strcmp(buttonID,"COS")) {
		if (degrees) {
			xFunction(cos(deg_2_rad(x)));
		} else {
			xFunction(cos(x));
		}
	}
	else if (!strcmp(buttonID,"TAN")) {
		if (degrees) {
			xFunction(tan(deg_2_rad(x)));
		} else {
			xFunction(tan(x));
		}
	}
	else if (!strcmp(buttonID,"ASIN")||!strcmp(buttonID,"ASN")) {
		if (degrees) {
			xFunction(rad_2_deg(asin(x)));
		} else {
			xFunction(asin(x));
		}
	}
	else if (!strcmp(buttonID,"ACOS")||!strcmp(buttonID,"ACS")) {
		if (degrees) {
			xFunction(rad_2_deg(acos(x)));
		} else {
			xFunction(acos(x));
		}
	}
	else if (!strcmp(buttonID,"ATAN")||!strcmp(buttonID,"ATN")) {
		if (degrees) {
			xFunction(rad_2_deg(atan(x)));
		} else {
			xFunction(atan(x));
		}
	}
	else if (!strcmp(buttonID,"LOG")) { xFunction(log10(x)); }
	else if (!strcmp(buttonID,"10^X")||!strcmp(buttonID,"T^X")) { xFunction(pow(10,x)); }
	else if (!strcmp(buttonID,"LN")) { xFunction(log(x)); }
	else if (!strcmp(buttonID,"E^X")) { xFunction(exp(x)); }
	
	// To rectangular..
    else if (!strcmp(buttonID,"»R")||!strcmp(buttonID,">R")) {
		double radians;
		double distance;
		
		if (degrees) {
			radians = deg_2_rad(y);
		} else {
			radians = y;
		}
		distance = x;
		y = sin(radians) * distance;
		x = cos(radians) * distance;
		editing = false;
	}
	
	// To polar..
	// r = √ (x^2 + y^2)
	//θ = arctan (y/x) where x != 0
	else if (!strcmp(buttonID,"»P")||!strcmp(buttonID,">P")) {
		
		double xDistance = x;
		double yDistance = y;
		
		if (xDistance) {
			x = sqrt(xDistance*xDistance + yDistance*yDistance);
			y = atan(yDistance/xDistance);
		} else {
			x = yDistance;
			y = M_PI/2;
		}
		if (degrees) {
			y = rad_2_deg(y);
		}
		editing = false;
	}
	
    // Enter is wierd!
    // From the HP11C calculator manual, it diables the stack for the -next- keypress.
    // So, we're going to set it when the enter key is pressed, then exit immediatly.
    // On every other keypress, we exit through this route.
    // So it'll be checked cleared automatically.
    //
	// Fix is kinda' the same..
	//
	// Well actually, so is Sto. And Rcl
	
    enter = false;  // And away we go!
	fixPressed = false;
	stoPressed = false;
	rclPressed = false;
}


void calculator::startEditing() {
	
	editing = true;
	point = false;
	multiplyer = 0.1;
}


void calculator::handleNumber(int inNum) {

	if (fixPressed) {
		fix = inNum;
	} else if (stoPressed) {
		sto[inNum] = x;
		editing = false;
	} else if (rclPressed) {
		pushStack();
		x = sto[inNum];
		editing = false;
	} else {
		if (editing) {
			if (point) {
				if(x>=0) {
					x = x + (inNum * multiplyer);
				} else {
					x = x - (inNum * multiplyer);
				}
				multiplyer = multiplyer / 10;
			} else {
				x = x * 10;
				if(x>=0) {
					x = x + inNum;
				} else {
					x = x - inNum;
				}
			}
		} else {
			startEditing();
			if (!enter) {
				pushStack();
			}
			x = inNum;
		}
	}
}


void calculator::handlePoint(void) {
		
	if (!editing) {
		startEditing();
		if (!enter) {
			pushStack();
		}
		x = 0;
	}
	point = true;
}


void calculator::handleClx(void) {
	
	x = 0;
	startEditing();
}

void calculator::handleRollDown(void) {
	
	popStack();
	editing = false;
}


void calculator::pushStack(void) {
    
    t = z;
	z = y;
	y = x;
}


void calculator::popStack(void) {
    
    x = y;
    y = z;
    z = t;
}


void calculator::xyFunction(double result) {
    
    y = result;
    popStack();
    editing = false;
}


void calculator::xFunction(double result) {
    
    x = result;
    editing = false;
}


double calculator::getX() { return x; }
double calculator::getY() { return y; }
double calculator::getZ() { return z; }
double calculator::getT() { return t; }

double calculator::getStoVal(int i) { return sto[i]; }


bool calculator::isEditing(void) { return editing; }


void calculator::setDegrees(bool inDegrees) { degrees = inDegrees; }


bool calculator::toggleDegrees(void) {
	
	degrees = !degrees;
	return degrees;
}


bool calculator::getDegrees(void) { return degrees; }


double calculator::rad_2_deg(double x) { return x*180/M_PI; }


double calculator::deg_2_rad(double x) { return x*M_PI/180; }


int  calculator::getFixVal(void) {
    
    if(editing) {
        if (!point) {
            return 0;
        } else {
            return (-log10(multiplyer));
        }
    } else
        return fix;
}


bool calculator::getFix(void) { return fixPressed; }


bool  calculator::getSto(void) { return stoPressed; }


bool  calculator::getRcl(void) { return rclPressed; }


bool  calculator::getEnter() { return enter; }

