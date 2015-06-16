#include "remoteRPNCalc.h"


class RPNClient {
 
   public:
 
    RPNClient(void);
    ~RPNClient(void);
    
    void     begin(void);
    void     buttonClick(char* buttonID);
    int     getFixVal(void);
    double   getX(void);
};


