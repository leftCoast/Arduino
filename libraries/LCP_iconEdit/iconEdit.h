#ifndef iconEdit_h
#define iconEdit_h

#include <lilOS.h>



class iconEdit	: public panel {

	public:
					iconEdit(lilOS* ourOS,int ourAppID);
	virtual		~iconEdit(void);
	
	virtual void  		setup(void);
	virtual void  		loop(void);
	virtual void  		drawSelf(void);
	virtual void  		closing(void);

};

#endif
