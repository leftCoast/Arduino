#ifndef testApp_h
#define testApp_h

#include <lilOS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <calculator.h>
#include <label.h>


class testApp;

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


class setAngleBtn :  public iconButton {
   
   public:
            setAngleBtn(int xLoc,int yLoc,const char* path,testApp* ourApp);
   virtual  ~setAngleBtn(void);
   
   virtual  void  doAction(void);
   
            testApp* ourApp;          
};


class clearAngleBtn :  public iconButton {
   
   public:
            clearAngleBtn(int xLoc,int yLoc,const char* path,testApp* ourApp);
   virtual  ~clearAngleBtn(void);
   
   virtual  void  doAction(void);
   
            testApp* ourApp;          
};


class testApp :  public panel {

   public:
            testApp(lilOS* ourOS,int ourAppID);
   virtual  ~testApp(void);

   virtual  void     setup(void);
            void     showValues(float x,float y,float z);
            void     showOffsets(float x,float y);
            char*    iconPath(int appID,const char* iconName);
            void     setOffsets(bool setClear);
            void     displaySensorDetails(void);
   virtual  void     loop(void);

   virtual  void drawSelf(void);

            colorObj          scrColor;
            Adafruit_BNO055*  bno;
            calculator        theCalc;
            float             sinMult;
            float             offsetX;
            float             offsetY;
            point             center;
            point             oldLoc;
            bubble*           theBubble;
            grid*             theGrid;
            timeObj           bubbleTimer;
            colorMultiMap     gridMap;
            char              pathBuff[80];
            label*            xLabel;
            label*            yLabel;
            label*            zLabel;
            label*            offsetXLabel;
            label*            offsetYLabel;
            label*            x2Label;
            label*            y2Label;
            
};


#endif
