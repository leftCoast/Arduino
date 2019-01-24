#ifndef PNLabel_h
#define PNLabel_h

#include <label.h>


class PNLabel : public label {

  public:
          PNLabel(int inLocX, int inLocY, int inWidth,int inHeight,int textSize);
  virtual ~PNLabel(void);

  virtual void  setValue(char* str);
          void  formatPN(void);
          void  formatOne(void);
          void  formatStar(void);
          void  formatHash(void);
          void  formatStd(void);

          char* mFormattedPN;
          char* mRawPN;
};


#endif
