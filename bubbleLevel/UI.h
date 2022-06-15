#include <iconButton.h>
#include <label.h> 

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
extern label*           rotationAngleX;
extern label*           rotationAngleY;
extern label*           rotationAngleZ;
extern label*           modelAngleX;
extern label*           modelAngleY;
extern label*           modelAngleZ;
