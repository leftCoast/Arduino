#ifndef sliderPuzzleBits_h
#define sliderPuzzleBits_h


#include <iconButton.h>
#include <bitmap.h>
#include <filePath.h>
#include <cardIndex.h>
#include <strTools.h>


#define NUM_SQR	4 
#define SQR_SIZE	60
#define BRD_SIZE	NUM_SQR * SQR_SIZE


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

				sliderBoard*	ourBoard;
				bool				visable;
				int				ourID; 
};



// ************* sliderBoard **************


class sliderBoard : public drawGroup {

	public:
				sliderBoard(int x, int y);
				~sliderBoard(void);
				
				
				void 			setup(filePath* imagePath);
				sliderRect*	findSlider(rect* aRect);
				void			blindSwap(sliderRect* rectA,sliderRect* rectB);
				void			scramble();
				void 			clicked(sliderRect* theClicked);
				void			readLine(File src,char* buff,int numBytes);
				bool			readState(void);
				void			writeState(void);
	virtual	void			drawSelf(void);
					
				bitmap*	ourBmp;
				filePath	ourImagePath;
				filePath	ourSavePath;
				char*		saveFileName;
				int		numSquares;
				byte		locList[NUM_SQR*NUM_SQR*2];	// 2 bytes for each.
};

#endif