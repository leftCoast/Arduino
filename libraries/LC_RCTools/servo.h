#ifndef servo_h
#define servo_h

#include "squareWave.h"

class servo : public squareWave {

	public:
				servo (int inPin);
	virtual	~servo(void);
     
				void	setServo(float val); 
	virtual	void	pulseOn(void);
	virtual	void	pulseOff(void);
	
				bool	init;
				int	mPin;
};

#endif