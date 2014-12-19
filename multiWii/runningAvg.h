#ifndef runningAve_h 
#define runningAve_h

#include <Arduino.h>

#define DEFAULT_NUM_DATA 5

class runningAvg {
 
public:  
   runningAvg(int inNumData=DEFAULT_NUM_DATA);   // constructor with initial data amount.
   ~runningAvg();
   
   void  transferData(float* toPtr, int toLength,float* fromPtr,int fromLenth); // Helper function, kinda like block move.
   void  setNumData(int inNumData);            // Sets the data amount.
   float writeData(float inData);              // write in the next data point. Retunrs the avarage of all the data.
   float readAve();                            // Retunrs the avarage of all the data without a write.
   void  resetData();                          // Reset the data.
   
 private:  
   int      numData;               // number of data points we use.
   float*   theDataPtr;            // Where the data is stored.
   boolean  full;                  // we full?
   int      index;                 // Where to store the next data point.
   float    avarage;               // Duh!
};
   


#endif // runningAve_h
