#include "PulseOut.h"
#include "mapper.h"

class servo : public PulseOut {
  public:
     servo (int inpin);
     
     void setServo (float val); 
};
