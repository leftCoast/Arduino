#include <lilOS.h> 
#include <cardIndex.h>
#include <sliderPuzzleBits.h>	

class sliderPuzzle;



class nextImage : public iconButton {

	public:
				nextImage(int inX,int inY,const char* path);
				~nextImage(void);
				
				void	setGame(sliderPuzzle* inPtr);
	virtual	void  doAction(void);
	
				sliderPuzzle* ourGame;
};



class scramble : public iconButton {

	public:
				scramble(int inX,int inY,const char* path);
				~scramble(void);
				
				void	setGame(sliderPuzzle* inPtr);
	virtual	void  doAction(void);
	
				sliderPuzzle* ourGame;
};



class sliderPuzzle :  public panel {

	public:
				sliderPuzzle(int ourAppID);
	virtual	~sliderPuzzle(void);
          
	virtual	void	setup(void);
				void	buildPixIndex(void);
	virtual	void	drawSelf(void);
				void	doNextImage(void);
				void	doScramble(void);
	virtual	void	loop(void);
	virtual	void	closing(void);
	
	sliderBoard*	ourBoard;		// Our game board.
	cardIndex*		ourFileIndex;
	  
};



