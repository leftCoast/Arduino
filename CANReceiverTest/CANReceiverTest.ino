// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>
#include <mapper.h>

#define SPI_CS 10
#define INT 2
#define RESET_PIN 8

word  theID;
struct bytes {
  byte  aByte[4];
};

struct msg_t {
  uint32_t pgn;
  uint8_t sa;
  uint8_t ps;
  uint8_t dp;
  uint8_t priority;
};

msg_t msg;

byte    dataPack[8];
mapper  speedMap(0,1024,0,(1024*1.943844)*0.01);

bytes* bytePtr;

void setup() {
  bytePtr = (bytes*)&theID;
  pinMode(RESET_PIN, OUTPUT);
  delay(50);
  digitalWrite(RESET_PIN, LOW);
  delay(50);
  digitalWrite(RESET_PIN, HIGH);

  Serial.begin(115200);
  delay(1000);
  Serial.println("CAN Receiver");

  Serial.print("CLK ");
  Serial.println(PIN_SPI_SCK);
  Serial.print("MISO ");
  Serial.println(PIN_SPI_MISO);
  Serial.print("MOSI ");
  Serial.println(PIN_SPI_MOSI);
  Serial.println("-----");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}


void printByte(byte inByte) {

  for (int i = 7; i >= 0; i--) {
    if (bitRead(inByte, i)) {
      Serial.print("1");
    } else {
      Serial.print("0");
    }
  }
}


void readAddress (uint32_t can_id, msg_t * msg) {
  uint32_t buffer = can_id;
  msg->sa = buffer & 0xFF;
  buffer = buffer >> 8;
  msg->pgn = buffer & 0x3FFFF;
  msg->ps = buffer & 0xFF;
  msg->dp = (buffer & 0xFF00) >> 8;
  msg->priority = (buffer & 0x1C0000) >> 18;

}


uint32_t makeAddress (uint32_t pgn, uint8_t priority, uint8_t source) {
  return ((pgn << 8) | priority << 26) | source;
}


void printType(uint32_t inType) {

  int i = 0;
  
  if (msg.pgn==inType) {
    switch (msg.pgn) {
      case 0x0FF1F  : Serial.print("Magnetic Variation (Raymarine Proprietary)"); break;
      case 0x1F010  : Serial.print("System Time"); break;
      case 0x1F105  : Serial.print("Heading/Track Control"); break;
      case 0x1F10D  : Serial.print("Rudder"); break;
      case 0x1F112  : Serial.print("Vessel Heading"); break;
      case 0x1F113  : Serial.print("Rate of Turn"); break;
      case 0x1F11A  : Serial.print("Magnetic Variation"); break;
      case 0x1F200  : Serial.print("Engine Parameters, Rapid Update"); break;
      case 0x1F503  : Serial.print("Speed, Water referenced"); break;
      case 0x1F50B  : Serial.print("Water Depth"); break;
      case 0x1F513  : Serial.print("Distance Log"); break;
      case 0x1F801  : Serial.print("Position, Rapid Update"); break;
      case 0x1F802  : Serial.print("COG & SOG, Rapid Update"); break;
      case 0x1F805  : Serial.print("GNSS Position Data"); break;
      case 0x1F809  : Serial.print("Local Time Offset"); break;
      case 0x1F814  : Serial.print("Datum"); break;
      case 0x1F903  : Serial.print("Cross Track Error"); break;
      case 0x1F904  : Serial.print("Navigation Data"); break;
      case 0x1F905  : Serial.print("Navigation — Route/WP information"); break;
      case 0x1F90B  : Serial.print("Set & Drift, Rapid Update"); break;
      case 0x1FA03  : Serial.print("GNSS DOPs"); break;
      case 0x1FA04  : Serial.print("GNSS Sats in View"); break;
      case 0x1FC12  : Serial.print("Route and WP Service — Route/WP— List Attributes"); break;
      case 0x1FC13  : Serial.print("Route and WP Service — Route — WP Name & Position"); break;
      case 0x1FC1A  : Serial.print("Route and WP Service — WP List — WP Name & Position"); break;
      case 0x1FD02  : Serial.print("Wind Data"); break;
      case 0x1FD06  : Serial.print("Environmental Parameters"); break;
      case 0x1FD07  : Serial.print("Environmental Parameters"); break;
      case 0x1FD08  : Serial.print("Temperature"); break;
      case 0x1FD09  : Serial.print("Humidity"); break;
      case 0x1FD0A  : Serial.print("Actual Pressure"); break;
      case 0x1FD0C  : Serial.print("Temperature, Extended Range"); break;
      case 0x1F80E  : Serial.print("AIS Class A Position Report"); break;
      case 0x1F80F  : Serial.print("AIS Class B Position Report"); break;
      case 0x1F810  : Serial.print("AIS Class B Extended Position Report"); break;
      case 0x1F811  : Serial.print("AIS Aids to Navigation (AtoN) Report"); break;
      case 0x1FB01  : Serial.print("AIS UTC and Date Report"); break;
      case 0x1FB02  : Serial.print("AIS Class A Static and Voyage Related Data"); break;
      case 0x1FB06  : Serial.print("AIS SAR Aircraft Position Report"); break;
      case 0x1FB11  : Serial.print("AIS Class B “CS” Static Data Report, Part A"); break;
      case 0x1FB12  : Serial.print("AIS Class B “CS” Static Data Report, Part B"); break;
      default       : Serial.print("Uknown value\t0x"); Serial.print(msg.pgn, HEX); break;
    }
    while (CAN.available()) {
      dataPack[i] = CAN.read();
      Serial.print("\t");
      Serial.print(dataPack[i]);
      i++;
    }
  }
}

struct uint16 {
  byte lowByte;
  byte hiByte;
};
uint16* bytes;
unsigned int rawSpeed;


void loop() {

  int packetSize;
  
  bytes = (uint16*)&rawSpeed;
  packetSize = CAN.parsePacket();
  if (packetSize) {
    theID = CAN.packetId();
    readAddress (theID, &msg);
    if (msg.pgn == 0x1F503) {
      printType(0x1F503);        
      Serial.print("\t");
      bytes->lowByte = dataPack[1];
      bytes->hiByte = dataPack[2];
      Serial.print(speedMap.map(rawSpeed), 1);
      Serial.println();
    }
  }
 }
