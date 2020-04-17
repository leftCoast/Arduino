#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>
#include <lilParser.h>

#include <colorRect.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>
#include <textView.h>
//#include <editField.h>
#include <editLabel.h>
#include <IOandKeys.h>
#include <panel.h>
#include <blockFile.h>
#include <fileBuff.h>

#include "gameNode.h"




class qGame : public  cellOSPanel {

  public:

  enum  commands { noCommand, start, y, yes, ok, sure, n, no, nope, naw, quit, stopx, resetx, swap, exitGame };
  enum  gameStates { waitingToStart, needYNAnswer, needNewObj, needNewQ, newQYNAnswer, needRestart };

          qGame(void);
  virtual ~qGame(void);



  virtual void  setup();
          void  setupSeedFile(void);          // Setup a new file and seed it with a few nodes.
          void  setupParser(void);            // List of things the parser can "see".
          void  setupScreen(void);
          void  startGame(void);
          void  dealWithYes(void);
          void  dealWithNo(void);
          void  swapAnswers(void);
          void  newObjChar(char inChar);
          void  newQChar(char inChar);
          void  hookinNewNodes(bool yesNo);
  virtual void	 close(void);
  virtual void  loop();

          textView*   theTextView;        // The text window.
          editLabel*  theEditField;       // The text edit field.
          colorRect*	 theEditBase;			// Background of text edit window.
          IOandKeys*  ourKeyboard;        // Just like it sounds, our texting keyboard.

          blockFile*  theFile;
          gameNode*   lastNode;
          gameNode*   currentNode;
          gameNode*   objNode;
          gameNode*   qNode;
          char        qBuff[200];
          int         buffIndex;
          char        inBuff[200];  // When the user hits return, we'll stuff the answer here.
          int         cIndex;
          lilParser   mParser;
          gameStates  gameState;
};
