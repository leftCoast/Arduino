#include <cardIndex.h>


// Creator, give us a deck size and away we go.
cardIndex::cardIndex(int inNumCards)
	: linkList() {
	
	numCards = inNumCards;		// Save off our deck size.
	loadList();						// Load up a new list of car indexes.
}


// Destructor, nothing for us to do. Ancestors do it all.
cardIndex::~cardIndex(void) { }


// Your basic reshuffle. Dump this list, reload a fresh one.
void cardIndex::loadList(void) {
	
	indexObj*	newIndex;
	
	dumpList();									// Dump out remaining card indexes.
	for (int i=1;i<=numCards;i++) {		// For the amount of cards we have..
		newIndex = new indexObj(i);		// Create a new card index node.
		addToTop(newIndex);					// Add it to the top of the list (very fast)
	}												//
	numRemain = numCards;					// Reset the amount of cards we have left to deal.
}
	

// Like it says, we deal a card. Returns the index of the card we delt. Or 0 for no card.	
int cardIndex::dealCard(void) {

	int			select;
	indexObj*	theNode;
	int			theIndex;
	
	theIndex = NO_CARD;										// Default for fail. Or, no cards left.
	if (numRemain) {											// If we have cards to deal..
		select = random(0,numRemain);						// Choose a random card.
		theNode = (indexObj*)getByIndex(select);		// Grab this one out of the list.
		if (theNode) {											// If we actually got one..
			unlinkObj(theNode);								// Unlink it from the deck.
			theIndex = theNode->getIndex();				// Save off the index value.
			delete(theNode);									// Recycle the card.
			numRemain--;										// Bump down our deck count.
		}															//
	}																//
	return theIndex;											// Return the card's index. Or 0 for no card.
}			


int cardIndex::getNumRemain(void) { return numRemain; }



// *************** indexObj ***************
	
	
//	indexObj basically a link list node that carries an int representing where it fell in
// the initial filling of it's linked list. As cards are delt, gaps appear in this list.
indexObj::indexObj(int inIndex) { index = inIndex; }


// Destructor, nothing to do here. Ancestors do it all.
indexObj::~indexObj(void) {  }


// Pass back your index. You been delt.
int indexObj::getIndex(void) { return index; }


