#ifndef PNLabel_h
#define PNLabel_h

#include <label.h>

// A label that will auto format a phone number to "look" correct.
// With paretheses, spaces and dashes in all the right places. Its
// tougher thann  you'd think.

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
				
	private:
				char* mFormattedPN;		// Don't be thinking there's anything useful saved here.
				char* mRawPN;				// Or in here.
};


#endif
