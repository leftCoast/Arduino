/*

#ifndef ringTone_h
#define ringTone_h

#include <lists.h>
#include <ceCommon.h>


class toneObj : public linkListObj,
                public idler,
                public timeObj {

  public: 

  enum toneState { inQueue, loading, playing, complete }
    
          toneObj(tone newNote);
  virtual ~toneObj(void);

          void  playTone(void);
          bool  finished(void);
          void  stopTone(void);

  virtual void  idle(void);

          toneState   mState;
          tone        mNote;
};


class ringTone :  public queue {
                  
  public:
          ringTone(void);
  virtual ~ringTone(void);

          void  addTone(tone newNote);
          void  playTones(void);
          void  stopPlaying(void);
};


#endif

*/
