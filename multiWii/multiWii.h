#ifndef multiWii_h
#define multiWii_h

// Pinouts for the LEDs we can control.
#define RED_LED   13  // HIGH is on
#define GREEN_LED 30  // LOW  is on
#define BLUE_LED  31  // LOW  is on
#define AMBER_LED 32  // LOW  is on


// Pinout mappings from silkscreen names to pin numbers.
// Port side of board. Port K? seem's fishy to me..
#define PIN_A8  A0
#define PIN_A9  A1
#define PIN_A10 A2
#define PIN_A11 A3
#define PIN_A12 A4
#define PIN_A13 A5
#define PIN_A14 A6
#define PIN_A15 A7
  
  
// Just in case, this is the standard multiWii mapping.
#define THROTTLEPIN PIN_A8
#define ROLLPIN     PIN_A9
#define PITCHPIN    PIN_A10
#define YAWPIN      PIN_A11
#define AUX1PIN     PIN_A12
#define AUX2PIN     PIN_A13
#define AUX3PIN     PIN_A14
#define AUX4PIN     PIN_A15


// Starboard side of board.
#define PIN_D10 10
#define PIN_D9   9
#define PIN_D8   8
#define PIN_D7   7
#define PIN_D6   6
#define PIN_D5   5
#define PIN_D3   3
#define PIN_D2   2

#define PIN_D44 44
#define PIN_D45 45
#define PIN_D46 46


#endif // multiWii_h
