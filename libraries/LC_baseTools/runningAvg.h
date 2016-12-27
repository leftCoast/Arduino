#ifndef runningAvg_h
#define runningAvg_h

class runningAvg {
  
  public:
    runningAvg(int inNumData);
    ~runningAvg(void);
    
    float addData(float inData);
    float getAve(void);
    
  protected:
    int    maxData;     // Total amount allowed.
    int    numValues;   // The amount we have.
    int    index;       // Write the next value, here.
    float  *theValues;  // The array of values.
    float  result;      // Just in case they ask, we'll keep a copy.
};

#endif
