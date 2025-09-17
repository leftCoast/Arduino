#include <navTest.h>


// **************************************************
// ***************** navTest ************************
// **************************************************

enum testCmds {
	
	noCommand,
	addWP,
	editWP,
	listWP,
	deleteWP
};
	
	

navTest::navTest(int ourAppID)
	: panel(ourAppID) {
	
	cmdParser = NULL;		// Donno' just in case.
}

navTest::~navTest(void) {

	if (cmdParser) {
		delete(cmdParser);
		cmdParser = NULL;
	}
}


void navTest::setup(void) {

	screen->fillScreen(&black);
	screen->setCursor(5,40);     // Move cursor to the top left somewhere.
   screen->setTextColor(&white); // Drawing in white..
   screen->setTextSize(1);       // Big enought to notice.
	screen->drawText("Type ? into the serial monitor.");
	
	cmdParser = new lilParser();
	if (!cmdParser) {
		Serial.println("Can't allocate our parser.");
		close();
	}
	cmdParser->addCmd(addWP,"add");
	cmdParser->addCmd(editWP,"edit");
	cmdParser->addCmd(listWP,"list");
	cmdParser->addCmd(deleteWP,"delete");
}


void navTest::loop(void) {

	char 	aChar;
	int	comVal;
	
	if (Serial.available()) {								// If the user has typed a char..
      aChar = Serial.read();								// Read and save it.
      Serial.print(aChar);									// Echo it back to the serial monitor.
      aChar = tolower(aChar);								// Make it all lower case. Makes sorting it out easier.
      comVal = cmdParser->addChar(aChar);				// Parse this char and see what we get back.
      switch(comVal) {										// If we find a command, we call a hanlder for it.
      	case noCommand		:					break;	// You know, still parsing.
      	case addWP			: doAddWP();	break;
      	case editWP  		: doEditWP();	break;
      	case listWP			: doListWP();	break;
         case deleteWP		: doDeteWP();	break;
      	default				: printHelp();	break;	// Tell 'em what it's all about.
      }
   }
}


void navTest::closing(void) {
	Serial.println("bye.");
}



// **************************************************
// 						COMMAND HANDLERS
// **************************************************


void navTest::doAddWP(void) {
	Serial.println("Add a way point");
}


void navTest::doEditWP(void) {
	Serial.println("Edit a way point");
}


void navTest::doListWP(void) {
	Serial.println("list all way points");
}


void navTest::doDeteWP(void) {
	Serial.println("Delete a way point");
}


void navTest::printHelp(void) {

	Serial.println("This is the testbed for developing the waypoint database.");
	Serial.println();
	Serial.println("Commands :");
	Serial.println("add - followed by something, adds a new waypoint.");
	Serial.println("edit - followed by something, adds a new waypoint.");
	Serial.println("list - prints out a list of the waypoints.");
	Serial.println("delete - followed by something, deletes a waypoint.");
}


      	
      	
      	
      	
      	