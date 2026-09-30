#include <FlexCAN_T4.h>
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;

void setup(void) {
  Serial.begin(115200); delay(400);
  pinMode(6, OUTPUT); digitalWrite(6, LOW); /* optional tranceiver enable pin */
  Can0.begin();
  Can0.setBaudRate(250E3);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
}

void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}

void loop() {
  Can0.events();
  
  static uint32_t timeout = millis();
  if ( millis() - timeout > 200 ) {
    CAN_message_t msg;
    msg.id = random(0x1,0x7FE);
    for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = i + 1;
    Serial.println("writing");
    Can0.write(msg);
    Can0.mailboxStatus();
    timeout = millis();
  }

}
/*
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
*/