#ifndef mapper_h
#define mapper_h

#include <Arduino.h>

class mapper {
public:
  mapper(void);
  mapper(double x1,double x2,double y1,double y2);

  double Map(double inNum);

  // This stuff is for using the mapper as a liner calculator.
  void setValues(double x1,double x2,double y1,double y2);
  double getSlope(void);
  double getMinX(void);
  double getMaxX(void);
  double getIntercept(void);
  //void   printMap(void);      // For debugging.
private:
  double minX;
  double maxX;
  double slope;
  double intercept;
};

#endif

