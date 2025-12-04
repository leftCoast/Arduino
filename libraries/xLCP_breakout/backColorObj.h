#ifndef backColorObj_h
#define backColorObj_h

#include "Arduino.h"
#include <colorObj.h>

class backColorObj :	public colorObj {

	public:
				backColorObj(void);
	virtual	~backColorObj(void);
	
				void		setBackColor(colorObj* aColor);
				void		setBackColor(byte r,byte g,byte b);
};

#endif