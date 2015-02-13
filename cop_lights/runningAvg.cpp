#include "runningAvg.h"


runningAvg::runningAvg(int inNumData) {
  
  theValues = (float*) malloc(sizeof(float)*inNumData);
  maxData = inNumData;
  numValues = 0;
  index = 0;
}

 
runningAvg::~runningAvg(void) {
  
  free(theValues);
  theValues = NULL;
}



float runningAvg::addData(float inData) {
  
  float sum;
  
  if (numValues<maxData) {
    theValues[index++] = inData;
    numValues++;
  } else {
    if (index==maxData) {
      index = 0;
    }
    theValues[index++] = inData;
  }
  sum = 0;
  for (int i=0;i<numValues;i++) {
    sum = sum + theValues[i];
  }
  return sum/numValues; 
}

  
