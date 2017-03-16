#ifndef fList_h
#define fList_h

#include <bmpLabel.h>
#include <mapper.h>
#include <drawObj.h>

#define FL_X    10
#define FL_Y    10
#define FL_H    110
#define FL_W    100

#define FLI_X   0
#define FLI_Y   0
#define FLI_H   12
#define FLI_W   100

#define NUM_FLI FL_H/FLI_H


void startEditor(void);
void buttonClick(void);
void potVal(int aVal);


class fListItem;

// First we have the list itself..

class fList : public drawList {

  public:
              fList(void);

      void    setHighlite(int index);
      void    potVal(int aVal) ;
      void    buttonClick(void);
      void    selectFile(void);
      void    setupMapper(void);
      bool    visible(fListItem* theItem);
virtual void  drawSelf(void);
      mapper  listMapper;
};


// And here the list items..

class fListItem : public bmpLabel {

  public:
                fListItem(char* fileName);
                ~fListItem(void);

          void   highlite(bool onOff);       // Show we are the choosen one!
  virtual bool   wantRefresh(void);
  virtual void   doAction(void);             // For starters, this will be to play the file.
  virtual void   drawSelf();

          bool   choosen;
};

#endif
