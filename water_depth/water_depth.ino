#define N2k_SPI_CS_PIN 12  // Pin for SPI Can Select
#define N2k_CAN_INT_PIN 5 // Use interrupt  and it is connected to pin 5
#define USE_MCP_CAN_CLOCK_SET 8
//#define USE_N2K_CAN 1


#include <SPI.h>
#include <mcp_can.h> 
#include <NMEA2000_mcp.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <N2kMessages.h>



tNMEA2000_mcp NMEA2000(N2k_SPI_CS_PIN);


typedef struct {
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg); 
} tNMEA2000Handler;


void FluidLevel(const tN2kMsg &N2kMsg);
void WaterDepth(const tN2kMsg &N2kMsg);


tNMEA2000Handler NMEA2000Handlers[]={
  {127505L,&FluidLevel},
  {128267L,&WaterDepth},
  {0,0}
};


void setup() {
  Serial.begin(115200);
  while(!Serial) delay(100);
  
  
  pinMode(13,OUTPUT);                           // Resets the controller chip.
  digitalWrite(13,LOW);
  delay(100);
  digitalWrite(13,HIGH);
  
  NMEA2000.EnableForward(false);              // Do not forward bus messages at all.
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
  Serial.println(NMEA2000.Open());
}


void WaterDepth(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;
    Serial.println("Depth");
    if (ParseN2kWaterDepth(N2kMsg,SID,DepthBelowTransducer,Offset) ) {
      if (Offset>0) {
        Serial.print("Water depth:");
      } else {
        Serial.print("Depth below keel:");
      }
      Serial.println(DepthBelowTransducer+Offset);
    }
}


void FluidLevel(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kFluidType FluidType;
    double Level=0;
    double Capacity=0;
    Serial.println("fluid");
    if (ParseN2kFluidLevel(N2kMsg,Instance,FluidType,Level,Capacity) ) {
      switch (FluidType) {
        case N2kft_Fuel:
          Serial.print("Fuel level :");
          break;
        case N2kft_Water:
          Serial.print("Water level :");
          break;
        case N2kft_GrayWater:
          Serial.print("Gray water level :");
          break;
        case N2kft_LiveWell:
          Serial.print("Live well level :");
          break;
        case N2kft_Oil:
          Serial.print("Oil level :");
          break;
        case N2kft_BlackWater:
          Serial.print("Black water level :");
          break;
      }
      Serial.print(Level); Serial.print("%"); 
      Serial.print(" ("); Serial.print(Capacity*Level/100); Serial.print("l)");
      Serial.print(" capacity :"); Serial.println(Capacity);
    }
}

//NMEA 2000 message handler
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  int iHandler;
  // Find handler
  Serial.println("big handler");
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  if (NMEA2000Handlers[iHandler].PGN!=0) {
    NMEA2000Handlers[iHandler].Handler(N2kMsg); 
  }
}

void loop() {
  NMEA2000.ParseMessages();
}
