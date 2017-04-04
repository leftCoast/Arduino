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
              colorObj  getPixelColor(word index);
              void      setPixelColor(word index,colorObj* inColor);
             
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
              
              colorObj   getPixelColor(word pixelNum);                   // What color is THIS pixel now?
              void       setPixelColor(word pixelNum, colorObj* color);  // Set THIS pixel this color.
              void       setPixels(colorObj* color);                // Set ALL our pixles this color.
              colorObj   shiftPixels(bool toEnd=true);							// Even handier!
  						void		   roll(bool clockwise=true);
      virtual void       draw(void);                                // This will be called repeatedly.
    
    protected:
      chainPixels*  ourChain;
      word          numPixels;
      word          index;
};

#endif
