#include <iconButton.h>


void setupUI(void);

class setAngleBtn :  public iconButton {
   
   public:
            setAngleBtn(int xLoc,int yLoc,const char* path);
   virtual  ~setAngleBtn(void);
   virtual  void  doAction(void);        
};


class clearAngleBtn :  public iconButton {
   
   public:
            clearAngleBtn(int xLoc,int yLoc,const char* path);
   virtual  ~clearAngleBtn(void);
   virtual  void  doAction(void);        
};

extern clearAngleBtn*   ourCAngleBtn;
extern setAngleBtn*     ourSAngleBtn;
extern float            offsetX;
extern float            offsetY;
extern float            offsetZ;
