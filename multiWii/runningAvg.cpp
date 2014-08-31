#include "runningAvg.h"

// constructor with initial data amount.
runningAvg::runningAvg(int inNumData) {

   
   theDataPtr = NULL;         // Where the data is stored.
   full = false;              // we full?
   index = 0;                 // Where to store the next data point.
   avarage = 0;
   if (inNumData<1) {
      setNumData(DEFAULT_NUM_DATA);     // number of data points we use.
   } else {
      setNumData(inNumData);
} 
}


runningAvg::~runningAvg() {
   
   if(theDataPtr) free(theDataPtr);   // If we have some, let it go!
}
 
 
// Given two float arrays pump the data from one to the other. Different sizes too.  
void runningAvg::transferData(float* toPtr, int toLength,float* fromPtr,int fromLenth) {
   
   int i;
   int len;
   
   if (toPtr && fromPtr) {
      len = min(toLength,fromLenth);
      for (int i=0;i<len;i++) {
         toPtr[i] = fromPtr[i];
      }
       
   }
}


// Sets the data amount.
// This SHOULD do the right thing no matter if the amount goes up or down.
// It SHOULD retain all the data possible. Although the avarage value will NOT
// be valid 'till another data point is added causing a recalculation.
void runningAvg::setNumData(int inNumData) {

   float* tempPtr;


   if (inNumData!=numData && inNumData > 0) {
      if (theDataPtr) {
         tempPtr = theDataPtr;
         theDataPtr = (float*)malloc((sizeof(float)) * inNumData);
         transferData(theDataPtr,inNumData,tempPtr,numData);
         free(tempPtr);
         if(numData < inNumData) {
            full = false;
         }
      }  else {
         theDataPtr = (float*)malloc((sizeof(float)) * inNumData);
      }
      numData = inNumData;
   }
}


// Write in the next data point. Calculates the avarage because first, it sets the 
// index and full parameters and second, we know we at least we have some data.
// Returns the avarage.
float runningAvg::writeData(float inData) {
   
   float sum;
   int   num;
   
   if (index>=numData) {              // Check if we're over the edge.. 
      full = true;                    // We are, so this means we're full..
      index = 0;                      // And we reset the index.
   }
   theDataPtr[index] = inData;        // Stuff in the data point
   index++;                           // increment the index
   
   if (!full)                         // How much data we have? Are we full?
      num = index;                    // Not full, then index holds the amount of data we have.
   else                               // We are full?
      num = numData;                  // Then numData holds the amount of data we have.
   sum = 0;                           // Clear the sum thing.
   for(int i=0;i<num;i++)             // Run through the data..
     sum = sum + theDataPtr[i];       // Summing it all up..
   avarage = sum/num;                 // Calculate the avarage..
}


// Returns the stored avarage value.
float runningAvg::readAve() { return(avarage); }


// Reset the data.
void runningAvg::resetData() {

   index = 0;
   full = false;
   avarage = 0;
}   
