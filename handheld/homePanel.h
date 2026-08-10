#ifndef homePanel_h
#define homePanel_h

#include <lilOS.h> 
#include <bmpObj.h> 
#include <rectArrange.h>
#include <debug.h>



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
          bool    updating;     // How many loops through loop() 'till we try to turn it back on.
};



#endif
