#ifndef testApp_h
#define testApp_h

#include <lilOS.h>

class grid : public drawObj {
    
    public:
            grid(int centerX,int centerY);
   virtual  ~grid(void);

            void  setColor(colorObj* inColor);
   virtual  void  drawSelf(void);

            point    center;
            colorObj gridColor;
};


class bubble : public drawObj {
   
    public:
            bubble(int startX,int startY,grid* inGrid);
   virtual  ~bubble(void);

   virtual  void  eraseSelf(void);
   virtual  void  drawSelf(void);
   
            grid* ourGrid;
};


class testApp :  public panel {

   public:
            testApp(lilOS* ourOS,int ourAppID);
   virtual  ~testApp(void);

   virtual  void setup(void);
   virtual  void loop(void);

   virtual  void drawSelf(void);

            point          center;
            point          oldLoc;
            bubble*        theBubble;
            grid*          theGrid;
            timeObj        bubbleTimer;
            colorMultiMap  gridMap;
};


#endif
