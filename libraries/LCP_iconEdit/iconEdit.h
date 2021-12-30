#ifndef iconEdit_h
#define iconEdit_h

#include <lilOS.h>
#include	<iconEditScr.h>
#include <documentPanel.h>

#define ICON_FLDR	"/EIcons/"


class iconEdit	: public documentPanel {

	public:
					
				iconEdit(lilOS* ourOS,int ourAppID);
	virtual	~iconEdit(void);
	
	virtual	void	createDocObj(void);
	virtual	void	setup(void);
	virtual	void	drawSelf(void);
	virtual	void	handleComHaveNamedFileNoEdits(stdComs comID);
	virtual	void	handleComAskOpen(stdComs comID);
	virtual	void	handleCom(stdComs comID);
	
};

#endif
