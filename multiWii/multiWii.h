#ifndef multiWii_h {
#define multiWii_h

// This is ripped from the shared multiWii code about this board.
#define ITG3200
#define BMA180
#define BMP085
#define HMC5883
#define ACC_ORIENTATION(X, Y, Z)  {imu.accADC[ROLL]  = -X; imu.accADC[PITCH]  = -Y; imu.accADC[YAW]  =  Z;}
#define GYRO_ORIENTATION(X, Y, Z) {imu.gyroADC[ROLL] =  Y; imu.gyroADC[PITCH] = -X; imu.gyroADC[YAW] = -Z;}
#define MAG_ORIENTATION(X, Y, Z)  {imu.magADC[ROLL]  =  Y; imu.magADC[PITCH]  = -X; imu.magADC[YAW]  = -Z;}
  
  
// Pinouts for the LEDs we can control.
#define RED_LED   13  // HIGH is on
#define GREEN_LED 30  // LOW  is on
#define BLUE_LED  31  // LOW  is on
#define AMBER_LED 32  // LOW  is on

  
// Just in case, this is the standard multiWii mapping.
#define THROTTLEPIN A8
#define ROLLPIN     A9
#define PITCHPIN    A10
#define YAWPIN      A11
#define AUX1PIN     A12
#define AUX2PIN     A13
#define AUX3PIN     A14
#define AUX4PIN     A15


// Starboard side of board. Silkscreen mapping..
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

