
#include <timeObj.h>
#include <idlers.h>


#define DOT_MS    50			// How long in ms is a dot sound.
#define DASH_MS   150		// How long in ms is a dash sound.
#define PAUSE_MS  100		// How long we wait, in ms, between ket presses.
#define SPACE_MS  200		// How long we wait, in ms, between words.


class letter :	public idler {

	public:
				enum key {
				  nothing,
				  aDot,
				  aDash
				};
				
				enum letterState {
				  waiting,
				  starting,
				  waitForKey,
				  waitKeyPause,
				  waitForSpace
				};

				letter(void);
	virtual	~letter(void);
	
				void	setCallback(void (*funct)(bool));
				void	setLetter(char aLetter);
	virtual	void	setKey(bool down);
				bool	letterDone(void);
				
	protected:
				void	setupStrokes(void);
				bool	startStroke(void);
	virtual	void	idle();

				void    		(*callback)(bool);
				char        ourLetter;
				key			stroke[5];
				byte        strokeIndex;
				letterState ourState;
				timeObj     ourTimer;
};


