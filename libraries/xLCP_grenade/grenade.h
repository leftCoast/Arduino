#ifndef grenade_h
#define grenade_h

#include <lilOS.h>
#include <liveText.h>
#include <squareWave.h>
#include <toneObj.h>

class quickLabel;
class nextBtn;
class greenBtn;
class blueBtn;
class cardIndex;
class alarmBeeper;
				
class grenade :  public panel {

	public:
				grenade(int ourAppID);
	virtual	~grenade(void);
				
	virtual	void	setup(void);
				void	createSounds(void);
	virtual	void	drawSelf(void);
				int	countWordList(void);
				void	nextBtnClick(void);
				void	greenBtnClick(void);
				void	blueBtnClick(void);
				void	showWinner(void);
				void	chooseWord(void);
	virtual	void	loop(void);
	virtual	void	closing(void);
	
				enum states { pregame, waiting, playing, exploding, scoring, winning, error };
				states			ourState;
				timeObj			playTimer;
				nextBtn*			nextButton;
				quickLabel*		theWord;
				int				wordCount;
				cardIndex*		wordIndex;
				alarmBeeper*	ourBeeper;
				label*			message;
				blueBtn*			blueCross;
				int				blueScore;
				label*			blueScoreLbl;
				greenBtn*		greenCross;
				int				greenScore;
				label*			greenScoreLbl;
				toneObj*			ourPlayer;
				tune				explodeSnd;
				tune				winTune;
};


class quickLabel :	public label {
	
	public:
				quickLabel(int inLocX, int inLocY, int inWidth,int inHeight);
	virtual	~quickLabel(void);
	
	virtual	void 	setColors(colorObj* tColor, colorObj* bColor);
	virtual	void 	setValue(const char* str);
	virtual	void	eraseSelf(void);										
	
				colorObj	savedBColor;
				char*		savedStr;
};


class nextBtn :	public bmpObj {

	public:
				nextBtn(int inX,int inY,int inWidth,int inHeight,const char* bmpPath);
	virtual	~nextBtn(void);
	
				void	setGrenade(grenade* inPtr);
	virtual	void	doAction(void);
	
				grenade*	ourGrenadePtr;
};



class greenBtn :	public bmpObj {

	public:
				greenBtn(int inX,int inY,int inWidth,int inHeight,const char* bmpPath);
	virtual	~greenBtn(void);
	
				void	setGrenade(grenade* inPtr);
	virtual	void	doAction(void);
	
				grenade*	ourGrenadePtr;
};



class blueBtn :	public bmpObj {

	public:
				blueBtn(int inX,int inY,int inWidth,int inHeight,const char* bmpPath);
	virtual	~blueBtn(void);
	
				void	setGrenade(grenade* inPtr);
	virtual	void	doAction(void);
	
				grenade*	ourGrenadePtr;
};


class alarmBeeper :	public squareWave {

	public:
				alarmBeeper(int inPin, float totalMs,int inHiHz, int inLowHz);
	virtual	~alarmBeeper(void);
	
	virtual	void	pulseOn(void);
	virtual	void	pulseOff(void);
	virtual	void	setOnOff(bool onOff);
	virtual	void	idle(void);
	
				enum 		states { waiting, phase1, phase2, phase3, phase4 };
				states	ourState;
				int		tonePin;
				timeObj	alarmTimer;
				int		hiHz;
				int		lowHz;
				int		mult;
				bool		hiLow;
};

#endif