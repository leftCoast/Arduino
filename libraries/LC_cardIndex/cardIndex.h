#ifndef cardIndex_h
#define cardIndex_h

// Let's say you have a stack of playing cards. You want to deal them out but don't want
// to spend a ton of time at the end of the deck doing random numbers, trying to land on
// one you haven't delt yet. Heck, you don't even want to keep track of the ones you delt
// already. What to do? Here's your answer. The card index.
//
// How to use? Easy peasy! Lets say playing cards.
// 
// 1) You have 52 cards. Figure out a numbering for them 1..52. Meaning? I give you a
// number say.. 26. And you will be able to tell me what card that is. However you like
// to order them is up to you.
//
// 2) Create the cardIndex object with the number of cards you have. 
// For example : cardIndex myCards(52);
//
// 3) Deal cards from your deck. 
// For example : aCardNum = myCards.dealCard();
//
// 4) When the hand is over, just call loadList() to reshuffle the cards.
//
// 5) myCards.getNumRemain(); Will return the number of cards remaining to be delt.
//
// That's all there is to it. 
//
// But it doesn't have to be playing cards. It could be custom game cards. It could be
// picking words from a list? Whatever list of things you need to be chosen from, one by
// one.


#include <lists.h>

#define NO_CARD	0	// What to pass back if there are no more cards.


// Ignore this class, its used by the class you actually will interact with.
class indexObj : public linkListObj {

	public:
				indexObj(int inIndex);
	virtual	~indexObj(void);
	
				int getIndex(void);
				
				int index;
};
	

// This is the one you create and use.	
class cardIndex :	public linkList {

	public:
				cardIndex(int inNumCards);
	virtual	~cardIndex(void);
	
				void	loadList(void);
				int	dealCard(void);
				int	getNumRemain(void);
				
	protected:
				int	numCards;
				int	numRemain;
};


#endif