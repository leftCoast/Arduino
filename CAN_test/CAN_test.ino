
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> ourCANBus;


void setup() {
  while(!Serial) delay(100);
  Serial.println("SERIAL ON!");

  ourCANBus.begin();
	ourCANBus.setBaudRate(500E3);
	ourCANBus.setMaxMB(16);
	ourCANBus.enableFIFO();
	ourCANBus.enableFIFOInterrupt();
	ourCANBus.onReceive(incomingCANMsg);
	ourCANBus.mailboxStatus();
}

void incomingCANMsg(const CAN_message_t &msg) {

  int   i;

  if (msg.flags.extended) {
    Serial.println(msg.id);
    Serial.println(msg.len);
    i = 0;														
    while (i < msg.len) {    				
      Serial.println(msg.buf[i]);	
      i++;														
    }
  }	
  Serial.println("-----");														
}



void loop() {

  ourCANBus.events();
}
