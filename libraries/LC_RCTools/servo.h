#include "PulseOut.h"
#include "mapper.h"

class servo : public PulseOut {
	public:
				servo (int inpin);
	virtual	~servo(void);
     
				void	setServo(float val); 
};
