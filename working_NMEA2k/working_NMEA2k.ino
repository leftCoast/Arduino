// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13

#include <SPI.h>
#include "mcp_can.h"
#include <N2kMsg.h>
#include <N2kMessages.h>

#define SPI_CS_PIN  12


MCP_CAN CAN(SPI_CS_PIN);   // Set CS pin

void setup() {
    
    Serial.begin(115200);
    while (!Serial) delay(100);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}


void loop() {
    unsigned char               len = 0;
    unsigned char               buf[8];
    unsigned char               canId;
    unsigned char               SID;
    double                      WaterReferenced;
    double                      GroundReferenced;
    tN2kSpeedWaterReferenceType SWRT;
    tN2kMsg                     N2kMsg;
    
    if(CAN_MSGAVAIL == CAN.checkReceive()) {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        canId = CAN.getCanId();
        
        //ParseN2kPGN128259(&N2kMsg,&SID,&WaterReferenced,&GroundReferenced,&SWRT);

        Serial.println("-----------------------------");
        Serial.print("get data from ID: ");
        Serial.println(canId);
        for(int i = 0; i<len; i++) {
            Serial.print(buf[i]);
            Serial.print("\t");
        }
        Serial.println();
    }
}
