#ifndef cellListener_h
#define cellListener_h

#include <idlers.h>


class cellListener :  public idler {

  public:
          cellListener(void);
  virtual ~cellListener(void);

          void  begin(int answerPanelID);
  virtual void  idle(void);

          int    answerID;
};

extern bool           callIncoming;
extern cellListener   ourListener;

#endif
