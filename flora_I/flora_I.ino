#include <Adafruit_DotStar.h>
#include <Adafruit_seesaw.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>

#define BUTTON_PIN  1
#define MOTOR_1_PIN 3
#define MOTOR_2_PIN 4

#define D_STAR_DAT_PIN  7
#define D_STAR_CLK_PIN  8

#define DRY                 315
#define MUD                 1015
#define DEF_MOISTURE_LIMIT  45
#define DEF_WATER_TIME      10000
#define DEF_SOAK_TIME       120000

Adafruit_DotStar px(1, D_STAR_DAT_PIN, D_STAR_CLK_PIN, DOTSTAR_BRG);

Adafruit_seesaw ss;

runningAvg  cSmoother(20);
runningAvg  tSmoother(20);
timeObj     readTime(1000);
timeObj     waterTime(DEF_WATER_TIME);
timeObj     soakTime(DEF_SOAK_TIME);
mapper      mudMapper(DRY,MUD,0,100);
float       moisture;
float       moistureLimit;

enum        weDo { sitting, watering, soaking };
weDo        weAre;

void setup() {

  px.begin();                             // Initialize the LED.
  px.show();                              // Turn the LED off.
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);      // Motor button
  pinMode(MOTOR_1_PIN, OUTPUT);           // IN1 Motor
  digitalWrite(MOTOR_1_PIN, LOW);         // This direction off.
  pinMode(MOTOR_2_PIN, OUTPUT);           // IN2Motor
  digitalWrite(MOTOR_2_PIN, LOW);         // This direction off.
  pinMode(13, OUTPUT);                    // Standard red LED.
  
  Serial.begin(57600);                    // Fire up serial port.
  
  if (!ss.begin(0x36)) {                  // Start up moisture sensor.
    Serial.println("ERROR! no Sensor.");  // Failed!
    while(1);                             // Lock here.
  } else {
    Serial.print("Sensor version: ");     // Sensor up.
    Serial.println(ss.getVersion(), HEX); // Display verions number.
  }
  moistureLimit = DEF_MOISTURE_LIMIT;
  moisture = 100;
  readTime.start();
  weAre = sitting;
}

void setColor(colorObj* aColor) {

  px.setPixelColor(0,aColor->getGreen(),aColor->getRed(),aColor->getBlue());
  px.show();
  delay(10); 
}


void setPump(void) {

  if (!digitalRead(BUTTON_PIN) || weAre==watering) {
    digitalWrite(13, HIGH);
    digitalWrite(MOTOR_1_PIN,HIGH);
  } else {
    digitalWrite(13, LOW);
    digitalWrite(MOTOR_1_PIN,LOW);
  }
}


void loop() {

  float     tempC;
  uint16_t  capread;
  
  if (readTime.ding()) {
    tempC = ss.getTemp();
    capread = ss.touchRead(0);
    capread = cSmoother.addData(capread);
    tempC = tSmoother.addData(tempC);
    moisture = mudMapper.Map(capread);
    moisture = round(moisture);
  
    Serial.print("Temperature: "); Serial.print(tempC); Serial.print("*C   ");
    Serial.print("Capacitive: "); Serial.print(capread);Serial.print("   ");
    Serial.print("Moisture: "); Serial.print((int)moisture);Serial.print("%  current state : ");
    switch (weAre) {
      case sitting  :   Serial.println("sitting");  break;
      case watering :   Serial.println("watering"); break;
      case soaking  :   Serial.println("soaking");  break;
    }
    readTime.stepTime();
  }

  switch (weAre) {
    case sitting :
      setColor(&black);
      if (moisture<moistureLimit) {
        Serial.println("Watering");
        waterTime.start();
        weAre = watering;
      }
     break;
     case watering :
      setColor(&blue);
      if (waterTime.ding()) {
        Serial.println("Soaking");
        soakTime.start();
        weAre = soaking;
      }
     break;
     case soaking :
      setColor(&green);
      if (soakTime.ding()) {
        Serial.println("Back to sitting.");
        weAre = sitting;
      }
     break;
  }
  setPump();
}
