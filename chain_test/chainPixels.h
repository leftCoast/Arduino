#ifndef chainPixels_h
#define chainPixels_h

#include <colorObj.h>
#include <neoPixel.h>
#include <idlers.h>
#include <lists.h>

class pixelGroup;


class chainPixels : public queue, public idler {
  
  public:
            chainPixels(byte inPin);
            ~chainPixels(void);
              
              void      resetChain(void);
     virtual  void      push(linkListObj* item);
     virtual  void      pop(void);
              void      addGroup(pixelGroup* inGroup);
     virtual  void      idle(void);
              colorObj  getPixel(word index);
              void      setPixel(word index,colorObj* inColor);
             
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
            
              void       setIndex(word inIndex);
              void       setChain(chainPixels* inChain);
              word       getNumPixels(void);
              
              colorObj   getPixel(word pixelNum);                   // What color is THIS pixel now?
              void       setPixel(word pixelNum, colorObj* color);  // Set THIS pixle this color.
              void       setPixels(colorObj* color);                // Set ALL our pixles this color.
              
      virtual void       draw(void);                                // This will be called repeatedly.
    
    protected:
      chainPixels*  ourChain;
      word          numPixels;
      word          index;
};

#endif
