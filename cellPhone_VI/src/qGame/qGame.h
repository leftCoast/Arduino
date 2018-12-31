#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
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
#include <editField.h>
#include <IOandKeys.h>

#include "blockFile.h"
#include "fileBuff.h"
#include "gameNode.h"
#include "../../panel.h"



class qGame : public  panel {

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
          void  shutDown(void);
  virtual void  loop();

          colorRect*  theTextBase;        // White rect behind the text indow.
          textView*   theTextView;        // The text window.
          colorRect*  theEditBase;        // White rect behind the text edit field.
          editField*  theEditField;       // The text edit field.
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
