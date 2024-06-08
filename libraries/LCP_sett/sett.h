#ifndef sett_h
#define sett_h

#include <lilOS.h>
#include <toneObj.h>
#include <cardIndex.h>
#include <flasher.h>
#include <label.h>


#define	X_CARDS		3
#define	Y_CARDS		4
#define	CARD_H		74
#define	TOTAL_CARDS	81
#define	MAX_CARDS	15
#define	MAX_GROUPS	6

class selectQ;
class settCard;



// **************************************************
// ******************* cardGroup ********************
// **************************************************


class cardGroup {

	public:
				cardGroup(void);
	virtual	~cardGroup(void);
	
				void	setCards(settCard* card1,settCard* card2,settCard* card3);
				void	clearCards(void);
				bool	foundCard(settCard* aCard);
				bool	isSame(cardGroup* aSet);
				
				settCard*	groupCards[3];
};



// **************************************************
// ********************** sett **********************
// **************************************************


class sett :  public panel {

	public:
				sett(int ourAppID);
	virtual	~sett(void);
				
	virtual	void	setup(void);
				void	reloadGame(void);
				void	createSounds(void);
	virtual	void	drawSelf(void);
				void	dealCards(int srtX,int srtY);
				void	clearSelect(void);
				bool	duplicate(void);
				void	addSet(void);
				void	selectMe(settCard* inCard);
				void	unSelectMe(settCard* inCard);
				void	setMsg(const char* text,float ms);
	virtual	void	loop(void);
	virtual	void	closing(void);
			
				char*	cardPath;
				char*	selectPath;
				
	protected:
				enum states { pregame, waiting, playing, scoring, winning, error };
				
				label*		mesgPtr;
				timeObj		msgTimer;
				settCard*	cardPtrs[MAX_CARDS];	// The guys on the screen.
				cardIndex*	cardIndexList;
				cardGroup	groupList[MAX_GROUPS];
				int			groupIndex;
				selectQ*		selectList;
				states		ourState;
				toneObj*		ourPlayer;
				tune			winTune;
};


				
// **************************************************
// ******************* reloadBtn *******************
// **************************************************	


class reloadBtn :	public iconButton {	

	public:
				reloadBtn(int xLoc,int yLoc,const char* path,sett* inGame);
	virtual	~reloadBtn(void);
	
	virtual	void	doAction(void);
	
				sett*	gamePtr;
};



// **************************************************
// ******************* cardListObj ******************
// **************************************************


class cardListObj :	public linkListObj {

	public:
				cardListObj(settCard* inCard);
	virtual	~cardListObj(void);
	
				settCard*	cardPtr;
};



// **************************************************
// ******************** selectQ *********************
// **************************************************


class selectQ :	public queue {

	public:
				selectQ(void);
	virtual	~selectQ(void);
	
	virtual	void	push(linkListObj* newObj);
				void	deSelectObj(settCard* aCard);
				bool	isSett(void);
				
	protected:
				bool	colorFail(settCard* selected[]);
				bool	shapeFail(settCard* selected[]);
				bool	fillFail(settCard* selected[]);
				bool	numFail(settCard* selected[]);		
};			


	
// **************************************************
// ******************** settCard ********************
// **************************************************


class settCard :	public bmpFlasher {

	public:
				settCard(int cardNum,const char* cardPath,const char* selectPath,sett* inGame);
	virtual	~settCard(void);
	
	virtual	void	doAction(void);
	virtual 	void	drawSelf(void);
				void	toggleSelect(void);
				void	deSelect(void);
				
	 			enum cardColors	{ Red, Green, Blue };
				enum cardShapes	{ Diamond, Oval, Swishy };
				enum cardFills		{ Solid, Half, Open };
				
				sett*			ourGame;
				cardColors	ourColor;
				cardShapes	ourShape;
				cardFills	ourFill;
				int			ourItemCount;
				bool			selected;
};


#endif