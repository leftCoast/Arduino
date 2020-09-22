#include  "regionTest.h"
#include  "label.h"

rect	aRect;
bool	first = true;

// And it all starts up again..
regionTest::regionTest()
  : panel(rgnTestApp) { }


// The world as you know it, is ending..
regionTest::~regionTest(void) { }


// setup() & loop() panel style.
void regionTest::setup(void) {

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
