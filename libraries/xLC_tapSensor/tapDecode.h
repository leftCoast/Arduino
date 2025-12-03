#ifndef tapDecode_h
#define tapDecode_h

#include "tapSensor.h"
#include "lists.h"
/*

class tapRule :	public linkListObj {

	public:
				tapRule();
	virtual	~tapRule();
					
				bool	tap;			// Tap or time (pause).
				float	threshold;	// Max ignored tapping.
				float	minMs;		// Time value.
				float maxMs;		// Time value.
};
	

class tapDecode :	public tapSensor,
						public linkList {

	public:
					tapDecode(int sensorPin,float sumTime=DEF_SUMTIME,int numData=DEF_NUMDATA);
	virtual	~tapDecode(void);
					
				void	addTap(float threshold);
				void	addPause(float threshold,float minMs, float maxMs);
				void	addPause(float threshold,float minMs);
				void	begin(void);
				void	end(void);
	virtual	void	idle(void);
	virtual	void	doAction(void);
	
				tapRule*	index;
};
	
*/				
#endif