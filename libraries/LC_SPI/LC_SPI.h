// SPI setting choices are actually a function of the processor.
// Many have hardware dedicated for this, so these are the pins
// we should be using. Trying to define these settings at the
// hardware level is a mistake.
//
// Hence, this file.

#ifndef LC_SPI_H
#define LC_SPI_H

#include <SPI.h>

// The four shared SPI pins..
// These pins are best choice for Arduino & Teensy.
#define LC_CLK	13	// Clock
#define LC_MISO 12	// Input data
#define LC_MOSI 11	// Output data
#define LC_DC    9	// Data/command

// This set is for Adafruit Feather MO #2995
//#define LC_CLK 	24
//#define LC_MOSI	23
//#define LC_MISO	22
//#define LC_DC		21   // Free to choose.


// Each device needs its own chip select pin defined.
// Some devices want reset pins defined as well.


#endif