#include <Adafruit_NeoPixel.h>
#include "colorObj.h"

// ****************** liteLine ******************
//   Basic mechanix for drawing a line segment.
// **********************************************

class liteLine {
   
   public:
      liteLine(int inLength);
      ~liteLine(void);
      
      int      getLength(void);
      void     setLength(int inLength);
      virtual colorObj calcColor(int index,int i);     // index our head position, i our internal pixel
      colorObj getColor(int index);
      void     setLights(Adafruit_NeoPixel* lites,int index,boolean wrap=false);
      
   private:   
      int       length;
};



// *************** monoColorLine ****************
//    One color for the entire line. Simple.
// **********************************************


class monoColorLine : public liteLine {
   
   public :
   monoColorLine(int inLength);
   
   virtual colorObj calcColor(int index,int i);
   void    setColor(word inColor);
   void    setColor(byte red,byte green,byte blue);
   void    setColor(colorObj aColor);
   
   private :
   colorObj theColor;
};



// *************** multiColorLine ****************
//       A color for each pixel. Fancier..
// **********************************************


class multiColorLine : public liteLine {
   
    public :
    multiColorLine(int inLength);
    ~multiColorLine(void);
    
   virtual colorObj calcColor(int index,int i);               
   void    setColor(byte red,byte green,byte blue,int index);
   void    setColor(colorObj* inColor,int index);
   
   private :
   colorObj   defColor;
   colorObj*  colors;
};
   
