#ifndef idlers_h
#define idlers_h

#include "lists.h"

class idler : 
public linkListObj {

public: 
  idler(void);
  ~idler(void);

  void hookup(void);
  virtual void idle(void);

protected:
  boolean hookedIn;
  
};


class idlers : 
public linkList {

public:
  idlers(void);
  ~idlers(void);

  void idle(void);
};

extern idlers theIdlers;
extern void idle(void);

#endif