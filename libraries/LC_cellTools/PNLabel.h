#ifndef PNLabel_h
#define PNLabel_h

#include <label.h>

// Handy function to strip any un-dial-able characters out of a
// c string. I'll just leave it here in case someone wants to use it.
void filterPNStr(char* str);


class PNLabel : public label {

	public:
				PNLabel(int inLocX, int inLocY, int inWidth,int inHeight,int textSize);
	virtual	~PNLabel(void);

	virtual	void  setValue(char* str);
				int	getRawNumBytes(void);
				void	getRawPN(char* inBuff);
				void  formatPN(void);
				void  formatOne(void);
				void  formatStar(void);
				void  formatHash(void);
				void  formatStd(void);

				char* mFormattedPN;
				char* mRawPN;
};


#endif
