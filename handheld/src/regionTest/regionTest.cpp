#include  "regionTest.h"


// And it all starts up again..
regionTest::regionTest()
  : panel() { }


// The world as you know it, is ending..
regionTest::~regionTest(void) { }


// setup() & loop() panel style.
void regionTest::setup(void) { }


void regionTest::loop(void) {  }


// The default here is to not draw ourselves. You can change that.
void regionTest::drawSelf(void) {  }


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void regionTest::closing(void) {  }
