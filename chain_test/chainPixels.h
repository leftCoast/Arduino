#include <neoPixel.h>
#include <idlers.h>
#include "lists.h"

class pixelGroup;


class chainPixels : public queue, public idler {
  
  public:
            chainPixels(byte inPin);
            ~chainPixels(void);
              
              void resetGroups(void);
              void addGroup(pixelGroup* inGroup);
              void needRefresh(void);
     virtual  void idle(void);
             
  protected:
    byte      pin;
    word      numPixels;
    neoPixel* pixelDriver;
    boolean   dirtyBit;
};


class pixelGroup : public linkListObj {

    public:
            pixelGroup(word inNumPixels);
            ~pixelGroup(void);
            
              void setIndex(word inIndex);
              void setPixelDriver(neoPixel* inDriver);
              void setChain(chainPixels* inChain);
              word getNumPixels(void);
      virtual void setPixels(void);                      // This will be called repeatedly.
    
    protected:
      chainPixels*  ourChain;
      neoPixel*     pixelDriver;
      word          index;
      word          numPixels;
};
