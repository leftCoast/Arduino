#include <tracer.h>

tracer::tracer(char* traceText, int* inErrPtr) {

    errPtr = inErrPtr;
    mText = (char*)malloc(strlen(traceText)+1);
    strcpy(mText,traceText);
    Serial.print("Entering ");Serial.print(mText);
    if (errPtr) {
      Serial.print(" With Error: "); Serial.print(*errPtr);
    }
    Serial.println();
}

tracer::~tracer(void) {

    Serial.print("Exiting ");Serial.print(mText);
    if (errPtr) {
      Serial.print(" With Error: "); Serial.print(*errPtr);
    }
    Serial.println();
    free(mText);
}
