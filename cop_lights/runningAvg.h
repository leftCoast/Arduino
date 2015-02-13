#ifndef runningAvg_h
#define runningAvg_h

class runningAvg {
  
  public:
    runningAvg(int inNumData);
    ~runningAvg(void);
    
    float addData(float inData);
    
  protected:
    int    maxData;
    int    numValues;
    int    index;
    float  *theValues;
};

#endif
