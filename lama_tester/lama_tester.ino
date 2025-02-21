
#include "LC_llama2000.h"
#include <serialStr.h>

#define SPI_CS 10

llama2000         llamaBrd;
waterSpeedObj*    knotMeter;
waterTempObj*     thermometer;
fluidLevelObj*    fuelSender;
airTempBarometer* barometer;
serialStr         serialMgr;
byte              nameBytes[8] = { 170, 6, 32, 9, 0, 170, 160, 64 };
timeObj           waitTimer(10,false);


void setup() {


  Serial.begin(115200);
  delay(10);
  

  knotMeter   = new waterSpeedObj(&llamaBrd);
  thermometer = new waterTempObj(&llamaBrd);
  fuelSender  = new fluidLevelObj(&llamaBrd);
  barometer   = new airTempBarometer(&llamaBrd);
  
  llamaBrd.addMsgHandler(knotMeter);
  llamaBrd.addMsgHandler(thermometer);
  llamaBrd.addMsgHandler(fuelSender);
  llamaBrd.addMsgHandler(barometer);
  
  if (!llamaBrd.begin(SPI_CS)) {
    Serial.println("Starting llama failed!");
    while (1);
  }
  serialMgr.setCallback(gotCmd);
  Serial.println("Up and running");
}


void gotCmd(char* inStr) {

  float     value;
  
  if (!strcmp(inStr,"refresh")) {
    llamaBrd.refreshAddrList();
    waitTimer.setTime(BCAST_T1_MS);
    Serial.println("Refreshing address list.");
  } else if (!strcmp(inStr,"see")) {
    llamaBrd.showName();
    Serial.println();
  } else if (!strcmp(inStr,"show")) {
    showValues();
  } else if (!strcmp(inStr,"list")) {
    llamaBrd.showAddrList(true);
  } else if (!strcmp(inStr,"bar")) {
    Serial.print(barometer->getInHg(),3);
  } else if (!strcmp(inStr,"find")) {
    netName aName;
    aName = llamaBrd.findName(117);
   aName.showName();
   Serial.println();
   Serial.print("And address : ");
   Serial.println(llamaBrd.findAddr(&aName));
  } else if (!strcmp(inStr,"change")) {

    netName aName;

    aName.setName(nameBytes);
    llamaBrd.addrCom(&aName,120);
  } else {
    value = atof(inStr);
    Serial.print("Setting fuel level to ");
    Serial.print(value,1);
    Serial.println(" %");
    fuelSender->setTankType(fuel);
    fuelSender->setLevel(value);
    fuelSender->setCapacity(20);
    fuelSender->newMsg();
  } 
}

void showValues(void) {

  Serial.print(knotMeter->getSpeed(),1);
  Serial.print("\tKnots\t\t");
  Serial.print(barometer->getInHg(),3);
  Serial.print("\tinHg\t\t");
  Serial.print(thermometer->getTemp(),1);
  Serial.println("\tDeg F.");
}

//timeObj graphTime(15 * 1000);

void loop() {
  
  idle();
  sleep(2000);
  /*
   if (barometer->getInHg()>31) {
    showValues();
  }
<<<<<<< Updated upstream
  if (waitTimer.ding()) {
    llamaBrd.showAddrList(true);
    waitTimer.reset();
  }
=======
  */
>>>>>>> Stashed changes
  /*
  if (graphTime.ding()) {
    Serial.println(barometer->getInHg(),3);
    graphTime.stepTime();
  }
  */
}
  
