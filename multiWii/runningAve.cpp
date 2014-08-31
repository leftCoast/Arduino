#include "runningAve.h"

// constructor with initial data amount.
runningAve::runningAve(int inNumData) {

   
   theDataPtr = null;         // Where the data is stored.
   full = false;              // we full?
   index = 0;                 // Where to store the next data point.
   setNumData(inNumData);     // number of data points we use.
} 


runningAve::~runningAve() {
   
   if(theDataPtr) free(theDataPtr);
}
   
   
// Sets the data amount.
void runningAve::setNumData(int inNumData) {

   float* tempPtr;
   
   tempPtr = theDataPtr;
   theDataPtr = malloc(sizeof(float)*inNumData);
   
}


   float writeData(float inData);     // write in the next data point. Retunrs the avarage of all the data.
   float readAve();                   // Retunrs the avarage of all the data without a write.
   void  resetData();                 // Reset the data.
   
private:  
   
