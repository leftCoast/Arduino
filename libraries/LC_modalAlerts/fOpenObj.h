#ifndef fOpenObj_h
#define fOpenObj_h

#include <fileBaseViewer.h>


class fOpenObj :	public fileBaseViewer {

	public:
				fOpenObj(panel* inPanel,bool(*funct)(char*)=NULL);
	virtual	~fOpenObj(void);
				
	virtual	void	chooseFile(char* name);
	virtual	void	setSuccess(bool trueFalse);
};

#endif