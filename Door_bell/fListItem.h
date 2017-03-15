#ifndef fListItem_h
#define fListItem_h

#include <bmpLabel.h>

#define F_LIST_H      20
#define F_LIST_W      100
#define F_LIST_TXT_X  20

extern  void  editTime(void);

class fListItem : public drawObj {

  public:
                fListItem(char* fileName,bmpPipe* inBackdrop);
                ~fListItem(void);

          void  highlite(bool onOff);       // Show we are the choosen one!
  virtual void  doAction(void);             // For starters, this will be to pay the file.
  virtual void  drawSelf();

          bool      choosen;
          bmpLabel* ourText;
};




#endif
