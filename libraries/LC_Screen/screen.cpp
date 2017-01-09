#include "screen.h"
#include "adafruit_1431_Obj.h"
#include "adafruit_1947_Obj.h"

displayObj* screen = NULL;

boolean initScreen(byte hardware, int inRotation) {

  switch (hardware) {
    case ADAFRUIT_1431 :
      screen = (displayObj*) new adafruit_1431_Obj(ADAFRUIT_1431_SPI_CS, ADAFRUIT_1431_SPI_DC, ADAFRUIT_1431_SPI_MOSI, ADAFRUIT_1431_SPI_CLK, ADAFRUIT_1431_SPI_RST);
      if (screen) {
      	if (screen->dispObjBegin()) {
        		screen->setRotation(inRotation);
        		return true;
      	}
      }
      return false;
      break;
    case ADAFRUIT_1947 :
      screen = (displayObj*) new adafruit_1947_Obj(ADAFRUIT_1947_SPI_CS, ADAFRUIT_1947_SPI_DC, ADAFRUIT_1947_SPI_MOSI, ADAFRUIT_1947_SPI_CLK, ADAFRUIT_1947_SPI_RST,ADAFRUIT_1947_SPI_MISO);
      if (screen) {
        if (screen->dispObjBegin()) {
        		screen->setRotation(inRotation);
        		return true;
        }
      }
      return false;
      break;
    default : return false;
  }
}

