#include <blinker.h>
#include <serialStr.h>
#include <wiring_private.h> // For pinPeripheral() function
#include "GPS_NMEA_Parser.h"


Uart GPSPort (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
                         
void SERCOM1_Handler(void) {
  GPSPort.IrqHandler();
}

blinker   heartbeat;
serialStr GPSMgr(&GPSPort,'\n',200);
timeObj   outputTimer(1000);

void setup() {

  heartbeat.setOnOff(true);
  Serial.begin(115200);
  while (!Serial) delay(10);
  GPSPort.begin(9600);
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);

  GPSMgr.setCallback(GPSMsg);
}


void GPSMsg(char* GPSStr) { 

  if (GPSStr[0]=='$') {
    //Serial.println(GPSStr);
    addMessage(GPSStr);
  }
}


void loop() {

  idle();
  if (outputTimer.ding()) {
    theGPRMCParser.showData();
    outputTimer.start();
  }
}
