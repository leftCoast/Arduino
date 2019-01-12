#ifndef stampObj_h
#define stampObj_h

#include <drawObj.h>


class stampObj :	public drawObj {

	public:
				stampObj(rect* aRect);
	virtual	~stampObj(void);
	
	virtual	void stamp(void);
};

#endif	
				