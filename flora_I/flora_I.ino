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


// Pins for the Trinet MO dot-Star.
#define D_STAR_DAT_PIN    7
#define D_STAR_CLK_PIN   8


Adafruit_DotStar px(1, D_STAR_DAT_PIN, D_STAR_CLK_PIN, DOTSTAR_BRG);

Adafruit_seesaw ss;

#define BUTTON_PIN  1
#define MOTOR_1_PIN 3
#define MOTOR_2_PIN 4
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
}


void loop() {

  digitalWrite(13, !digitalRead(BUTTON_PIN));
  digitalWrite(MOTOR_1_PIN, !digitalRead(BUTTON_PIN));

  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  Serial.print("Temperature: "); Serial.print(tempC); Serial.print("*C   ");
  Serial.print("Capacitive: "); Serial.println(capread);
}
