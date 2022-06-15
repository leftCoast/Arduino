class tBlock {

   public:
            
            
            tBlock(void);
   virtual  ~tBlock(void);
   
};


class tGroup {

   public:
            enum tGroupRot { horz, ninty, oneEighty, negNinty };
            
            tGroup(void);
   virtual  ~tGroup(void);

   virtual  void rotateCW(void);
   virtual  void rotateCCW(void);
   virtual  void moveDown(void);
   virtual  void dropDown(void);

            int   posX;
            int   posY;
            
};
