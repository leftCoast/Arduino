#ifndef homePanel_h
#define homePanel_h

#include <lilOS.h> 
#include <bmpObj.h> 
#include <rectArrange.h>

// *****************************************************
//                      iconArrange
// *****************************************************

class iconArrange :     public rectArrange {

	public:	
				iconArrange(void);
	virtual	~iconArrange(void);

	virtual	void arrangeList(void);
};



// *****************************************************
//                      homeScreen
// *****************************************************


class homeScreen : public homePanel {

	public:
				homeScreen(void);
	virtual	~homeScreen(void);

	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	void	drawSelf(void);

         	bmpObj*	mBackImage;
};



#endif
