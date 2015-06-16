#ifdef remoteRPNCalc_h
#define remoteRPNCalc_h

// Common declertions between RPNClient & RPNServer.

#define REMOTE_RPN_BAUD  9600

typedef struct {
  double xReg;
  int   fix;
} xReg_Fix;

#endif
