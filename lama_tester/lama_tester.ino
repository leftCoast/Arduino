
#include "LC_llama_NMEA2000.h"
#include <serialStr.h>

#define SPI_CS 10

llama_NMEA2000  llamaBrd;
waterSpeedObj*  knotMeter;
waterTempObj*   thermometer;
fluidLevelObj*  fuelSender;
serialStr       serialMgr;

void setup() {
  
  Serial.begin(115200);
  delay(100);
  serialMgr.setCallback(gotCmd);
  llamaBrd.addMsgObj(0x1F503);
  llamaBrd.addMsgObj(0x1F50B);
  llamaBrd.addMsgObj(0x1FD08);
  llamaBrd.addMsgObj(0x1F211);
  
  knotMeter = (waterSpeedObj*) llamaBrd.getMsgObj(0x1F503);
  thermometer = (waterTempObj*) llamaBrd.getMsgObj(0x1FD08);
  fuelSender = (fluidLevelObj*)  llamaBrd.getMsgObj(0x1F211);

 
  if (knotMeter) Serial.println("Have knotMeter");
  if (thermometer) Serial.println("Have thermometer");
  if (fuelSender) Serial.println("Have fuelSender");    

  llamaBrd.setAddrCat(nonConfig);
  llamaBrd.setAddr(20);
  llamaBrd.setIndGroup(Marine);
   
  if (!llamaBrd.begin(SPI_CS)) {
    Serial.println("Starting llama failed!");
    while (1);
  }
}


void gotCmd(char* inStr) {

  float     value;
  
  if (!strcmp(inStr,"go")) {
    llamaBrd.requestForAddressClaim(255);
  } else if (!strcmp(inStr,"see")) {
    llamaBrd.showName();
    Serial.println();
  } else {
    value = atof(inStr);
    Serial.print("Setting fuel level to ");
    Serial.print(value,1);
    Serial.println(" %");
    fuelSender->setTankType(fuel);
    fuelSender->setLevel(value);
    fuelSender->setCapacity(20);
    fuelSender->sendMsg();
  } 
}


void loop() {
  
  idle();
  //Serial.print(knotMeter->getSpeed(),1);
  //Serial.print("\tKnots\t\t");
  //thermometer->showDataBytes();
  //Serial.print(thermometer->getTemp(),1);
  //Serial.println("\tDeg F.");
  //sleep(2000);
}
  
