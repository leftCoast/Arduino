#include  "regionTest.h"
#include  "label.h"
#include <bmpObj.h>
//#include <bmpPipe.h>

rect	aRect;
bool	first = true;


LCDCharBlock::LCDCharBlock(int x,int y)
	: drawObj(x,y,5,8) {
	
	setColors(&yellow,&blue);
	clearMap();
}
	
	
LCDCharBlock::~LCDCharBlock(void) {  }
	
	
void LCDCharBlock::setColors(colorObj* fCOlor,colorObj* bCOlor) {

	mFColor.setColor(&yellow);
	mBColor.setColor(&blue);
}


void LCDCharBlock::setMapRow(byte row,byte bits) {

	if (row>=0&&row<8) {
		mBits[row] = bits;
		setNeedRefresh();
	}
}

void LCDCharBlock::setMap(byte* byteArray) {

	for (byte i=0;i<8;i++) {
		setMapRow(i,byteArray[i]);
	}
}
		
void LCDCharBlock::clearMap(void) { 

	for (byte i=0;i<8;i++) {
		setMapRow(i,0);
	}
}


void LCDCharBlock::drawSelf(void) {
				
	for (byte row=0;row<8;row++) {
		for (byte col=0;col<5;col++) {
			if (bitRead(mBits[row],5-col)) {			
				screen->drawPixel(x+col,y+row,&mFColor);
			} else {
				screen->drawPixel(x+col,y+row,&mBColor);
			}
		}
	}
}

				
byte frame1 [8] = {	

	B01100,
	B01100,
	B00000,
	B01110,
	B11100,
	B01100,
	B11010,
	B10011
};


byte frame2 [8] = {	

	B01100,
	B01100,
	B00000,
	B01100,
	B01100,
	B01100,
	B01100,
	B01110
};


timeObj	frameTimer(250);

bool	firstFrame = true;





// And it all starts up again..
regionTest::regionTest()
  : panel(rgnTestApp) { }


// The world as you know it, is ending..
regionTest::~regionTest(void) { }


// setup() & loop() panel style.
void regionTest::setup(void) {
	/*
	bmpObj* theTestFile = new bmpObj(20,175,12,12,"/neoPixel.bmp");
	theTestFile->begin();
	addObj(theTestFile);
	
	bmpPipe thePix;
	thePix.openPipe("/neoPixel.bmp");
	RGBpack theLine[12];
	
	colorObj	aColor;
	for (int ln=0;ln<12;ln++) {
		thePix.getLine(theLine,ln);
		for (int i=0;i<12;i++) {
			aColor.setColor(&(theLine[i]));
			//aColor.printRGB();
		}
		Serial.println();
	}
	
	backColor.setColor(LC_CHARCOAL);
	backColor.blend(&black,20);
	backColor.blend(&red,10);
	
	label* aLabel = new label("Hey!",1);
	aLabel->setColors(&white,&black);
	aLabel->setLocation(30,7);
	addObj(aLabel);
	
	mRegion = new region();
	aRect.setRect(30,70,20,50);
	mRegion->addRect(&aRect);
	aRect.setRect(44,32,16,86);
	mRegion->addRect(&aRect);
	aRect.setRect(100,132,44,27);
	mRegion->addRect(&aRect);
	aRect.setRect(105,135,39,19);
	mRegion->addRect(&aRect);
	
	aBlock = new LCDCharBlock(100,200);
	aBlock->setMap(frame1);
	addObj(aBlock);
	*/
}


void regionTest::loop(void) {

	
	if (first) {
		sleep(1000);
		tone(BEEP_PIN, 1500,35);
		sleep(100);
		tone(BEEP_PIN, 1500,35);
		sleep(100);
		aRect.setRect(10,25,200,128);
		mRegion->addRect(&aRect);
		tone(BEEP_PIN, 200,100);
		setNeedRefresh();
		first = false;
	}
	if (frameTimer.ding()) {
		frameTimer.start();
		if (firstFrame) {
			aBlock->setMap(frame2);
			firstFrame = false;
		} else {
			aBlock->setMap(frame1);
			firstFrame = true;
		}
	}
	
}

void printRect(rect* trace,char* label) {

	Serial.print(label);
	Serial.print(" x:");
	Serial.print(trace->x);
	Serial.print(" y:");
	Serial.print(trace->y);
	Serial.print(" width:");
	Serial.print(trace->width);
	Serial.print(" height:");
	Serial.println(trace->height);
}


// The default here is to not draw ourselves. You can change that.
void regionTest::drawSelf(void) {

	regionRectObj* trace;
	rect				aRect;
	colorObj			greenColor(LC_GREEN);
	
	screen->fillScreen(&backColor);
	if (!mRegion->isEmpty()) {
		greenColor.blend(&backColor,75);
		aRect = mRegion->getEnclosingRect();
		screen->drawRect(&aRect,&greenColor);
		trace = mRegion->getFirstRectObj();
		while(trace) {
			screen->drawRect(trace,&blue);
			trace = trace->getNext();
		}
	}
}


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void regionTest::closing(void) {  }
