
#ifdef ARDUINO_AVR_MEGA2560
#pragma message "Mega!"
#else
#pragma message "NOT Mega!"
#endif

#ifdef ARDUINO_AVR_UNO
#pragma message "UNO!"
#else
#pragma message "NOT UNO!"
#endif

#ifdef ARDUINO_TEENSY32
#pragma message "Teensy!"
#else
#pragma message "NOT Teensy!"
#endif


#ifdef ARDUINO_RASPBERRY_PI_PICO
#pragma message "rPi Pico!"
#else
#pragma message "NOT rPi Pico!"
#endif

#ifdef ADAFRUIT_FEATHER_M0
#pragma message "Feather M0!"
#else
#pragma message "NOT Feather M0!"
#endif

void setup() {
   // put your setup code here, to run once:

}

void loop() {
   // put your main code here, to run repeatedly:

}
