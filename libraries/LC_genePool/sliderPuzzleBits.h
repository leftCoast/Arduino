#ifndef sliderPuzzleBits_h
#define sliderPuzzleBits_h


#include <iconButton.h>
#include <bitmap.h>

class sliderBoard;

// ************** sliderRect **************


class sliderRect : public iconButton {

	public:
				sliderRect(sliderBoard* inBoard,int inID);
				~sliderRect(void);

				void  setVisable(bool yesNo);
				bool  isClear(void);
				void  doAction(void);
				void  swapWith(sliderRect* inRect);
				void	drawOffscreen(void);
	virtual	void	drawSelf(void);

				sliderBoard* ourBoard;
				bool        visable;
				int         ourID; 
};



// ************* sliderBoard **************


class sliderBoard : public drawGroup {

	public:
				sliderBoard(int x, int y);
				~sliderBoard(void);

				void 			setup(void);
				sliderRect*	findSlider(rect* aRect);
				void			setImage(const char* path);
				void			scramble();
				void 			clicked(sliderRect* theClicked);
	virtual	void			drawSelf(void);				
				bitmap*	ourBmp;
};

#endif