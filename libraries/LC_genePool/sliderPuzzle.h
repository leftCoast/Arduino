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


// pixFileItem


class pixFileItem : public linkListObj {

	public:
		pixFileItem(const char* inName);
		~pixFileItem(void);
		
		char*	getName(void);
		
		char*	fileName;
};

// pixIndex

class pixIndex : public linkList {

	public:
				pixIndex(void);
	virtual	~pixIndex(void);
	
				bool	isPixFile(const char* fileName);
				void	buildList(filePath* imagePath);
				char*	getName(int cardIndex);
};



class sliderPuzzle :  public panel {

	public:
				sliderPuzzle(int ourAppID);
	virtual	~sliderPuzzle(void);
          
	virtual	void	setup(void);
				
				void	buildPixIndex(void);
				bool	setImagePath(void);
				bool	chooseImage(void);
	virtual	void	drawSelf(void);
				void	doScramble(void);
	virtual	void	loop(void);
	virtual	void	closing(void);
	
	sliderBoard*	ourBoard;		// Our game board.
	pixIndex			ourPixIndex;	// Our list of image file names.
	cardIndex*		ourCardDeck;	// Our list deck of image ID's.
	filePath			ourImagePath;	// We build the path to our image here.	  
};



