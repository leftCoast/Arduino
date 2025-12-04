#ifndef songList_h
#define songList_h

#include <label.h>
#include <runningAvg.h>

#include "scrollingList.h"
#include "pushPotGUI.h"

// ************************************
// ********** songListItem ************
// ************************************


class songListItem : public label {

public:
          songListItem(drawList* myList,const char* name);
  virtual ~songListItem(void);

  virtual void drawSelf(void);
  virtual void doAction(void);
  
          drawList* mList;
};



// ************************************
// ************* songList *************
// ************************************


class songList :  public scrollingList,
                  public pushPotGUI {

  public:
          songList(int x, int y, word width,word height);
  virtual ~songList(void);

  virtual void        reset(void);
  virtual void        offList(void);
  virtual void        doPotVal(int aVal);
  virtual void        drawSelf(void);

          runningAvg* potSmoother;
          mapper*     potToList;
};


#endif
