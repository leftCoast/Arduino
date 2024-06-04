#ifndef sett_h
#define sett_h

#include <lilOS.h>
#include <toneObj.h>
#include <cardIndex.h>


#define	X_CARDS	3
#define	Y_CARDS	4


class settCard;


class sett :  public panel {

	public:
				sett(int ourAppID);
	virtual	~sett(void);
				
	virtual	void	setup(void);
				void	createSounds(void);
	virtual	void	drawSelf(void);
				void	dealCards(int srtX,int srtY);
	virtual	void	loop(void);
	virtual	void	closing(void);
	
	protected:
				enum states { pregame, waiting, playing, scoring, winning, error };
				
				cardIndex*		cardList;
				states			ourState;
				toneObj*			ourPlayer;
				tune				winTune;
};



class settCard :	public bmpObj {

	public:
				settCard(int cardNum,const char* ourPath,sett* inGame);
	virtual	~settCard(void);
	
	virtual 	void	drawSelf(void);
	
	protected:		
	 			enum cardColors	{ Red, Green, Blue };
				enum cardShapes	{ Diamond, Oval, Swishy };
				enum cardFills		{ Solid, Half, Open };
				
				sett*			ourGame;
				cardColors	ourColor;
				cardShapes	ourShape;
				cardFills	ourFill;
				int			ourItemCount;
};


#endif