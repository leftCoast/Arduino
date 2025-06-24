#include <wristDisplay.h>

wristDisplay   ourDisp;

void setup() { ourDisp.setup(); }

void loop() {

	idle();
	ourDisp.loop();
}
