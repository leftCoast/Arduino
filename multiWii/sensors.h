

  #define ITG3200 - Gyro
  #define BMA180
  #define BMP085
  #define HMC5883
  #define ACC_ORIENTATION(X, Y, Z)  {imu.accADC[ROLL]  = -X; imu.accADC[PITCH]  = -Y; imu.accADC[YAW]  =  Z;}
  #define GYRO_ORIENTATION(X, Y, Z) {imu.gyroADC[ROLL] =  Y; imu.gyroADC[PITCH] = -X; imu.gyroADC[YAW] = -Z;}
  #define MAG_ORIENTATION(X, Y, Z)  {imu.magADC[ROLL]  =  Y; imu.magADC[PITCH]  = -X; imu.magADC[YAW]  = -Z;}
  
  #define GYRO_ADDR B01101000   // Figured out that much.
  
