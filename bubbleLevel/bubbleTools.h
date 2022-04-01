#include <triDBase.h>
#include <mapper.h>
#include <arrayList.h>
#include <triDRender.h>

#define BOUND_X      20
#define BOUND_Y      40
#define BOUND_DIA    200
#define BOUND_CX     BOUND_DIA/2+BOUND_X
#define BOUND_CY     BOUND_DIA/2+BOUND_Y

#define TARGET_DIA   40
#define TARGET_X     BOUND_CX - TARGET_DIA/2
#define TARGET_Y     BOUND_CY - TARGET_DIA/2


class bubbleRender;

double radToDeg(double x); // Humans like degrees. These two do the traslantions.
double degToRad(double x);

extern colorObj      backColor;
extern bubbleRender* renderMan;   // triDRender is the engine that reads the model to do the 2D drawing.

extern triDRotation  new_angle;   // This sets the oreantation of the model's drwaing on the screen.
extern triDRotation  my_angle;    // Saves the last location.


triDTriangle*  createCircleThing(int slices,double coneAngle,double radius);
void           setupModel(void);


class bubble : public arrayList {

   public :
               bubble(triDTriangle* inList,long numItems);
   virtual     ~bubble(void);
};


class bubbleRender :   public triDRender {

      public :
            bubbleRender(int inX,int inY,int inWidth,int inHeight);
   virtual  ~bubbleRender(void);
   
   virtual  void  drawSelf(void);
};


class grid : public drawObj {
    
    public:
            grid(int centerX,int centerY);
   virtual  ~grid(void);

            void  setColor(colorObj* inColor);
   virtual  void  drawSelf(void);

            point    center;
            colorObj gridColor;
};
