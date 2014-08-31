#ifndef runningAve_h 
#define runningAve_h

class runningAve {
 
public:  
   runningAve(int inNumData);         // constructor with initial data amount.
   ~runningAve();
   
   setNumData(int inNumData);         // Sets the data amount.
   float writeData(float inData);     // write in the next data point. Retunrs the avarage of all the data.
   float readAve();                   // Retunrs the avarage of all the data without a write.
   void  resetData();                 // Reset the data.
   
 private:  
   int      numData;               // number of data points we use.
   float*   theDataPtr;            // Where the data is stored.
   boolean  full;                  // we full?
   int      index;                 // Where to store the next data point.
};
   


#endif // runningAve_h
