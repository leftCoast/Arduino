#ifndef runningAvg_h
#define runningAvg_h

class runningAvg {
  
  public:
    runningAvg(int inNumData);
    ~runningAvg(void);
    
    float addData(float inData);
    
  protected:
    int    maxData;     // Total amount allowed.
    int    numValues;   // The amount we have.
    int    index;       // Write the text value, here.
    float  *theValues;  // The arrag of values.
};

#endif
