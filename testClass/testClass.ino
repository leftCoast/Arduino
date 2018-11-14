#include "lists.h"

// Base class..
class baseClass {

  public:
    baseClass(char* string);
    virtual ~baseClass(void);

    virtual void doIt(void);
    
    char* mStr;
};


baseClass::baseClass(char* string) {

  Serial.println("Calling baseClass constructor");
  mStr = (char*)malloc(strlen(string)+1);
  if (mStr) {
    strcpy(mStr,string);
  }
}


baseClass::~baseClass(void) {

  Serial.println("Calling baseClass destructor");
  if (mStr) {
    free(mStr);
    mStr = NULL;
  }
}

void baseClass::doIt(void) {
  
  Serial.println("Calling baseClass's method doIt();");
  Serial.println(mStr);
}


// derivedClass

class derivedClass : public baseClass {

  public:
    derivedClass(char* string);
    virtual ~derivedClass(void);

    virtual void doIt(void);
    
    char* mStr2;
};


derivedClass::derivedClass(char* string) : baseClass((char*)"Sneaky!") {

  Serial.println("Calling derivedClass constructor");
  mStr2 = (char*)malloc(strlen(string)+1);
  if (mStr2) {
    strcpy(mStr2,string);
  }
}


derivedClass::~derivedClass(void) {

  Serial.println("Calling derivedClass destructor");
  if (mStr2) {
    free(mStr2);
    mStr2 = NULL;
  }
}

void derivedClass::doIt(void) {

  Serial.println("Calling derivedClass's method doIt();");
  Serial.println(mStr);
  Serial.println(mStr2);
}

class derivedListObj : public linkListObj {

  public:
              derivedListObj(char* inStr);
      virtual ~derivedListObj(void);

              void doIt(void);

      char* mStr;
};


derivedListObj::derivedListObj(char* inStr) {

  Serial.println("Calling derivedListObj constructor");
  mStr = (char*)malloc(strlen(inStr)+1);
  if (mStr) {
    strcpy(mStr,inStr);
  }
}


derivedListObj::~derivedListObj(void) {

  Serial.println("Calling derivedListObj destructor");
  if (mStr) {
    free(mStr);
    mStr = NULL;
  }
}

void derivedListObj::doIt(void) {
  
  Serial.println("Calling derivedListObj's method doIt();");
  Serial.println(mStr);
}


derivedClass*   testObj;
derivedListObj* testListObj;
linkList        listHead;

void setup() {

  pinMode(13, OUTPUT);
  Serial.begin(9600);
  testObj = new derivedClass((char*)"Hello baaaby!");
  testObj->doIt();
  delete (baseClass*)testObj;

  testListObj = new derivedListObj((char*) "First node");
  listHead.addToTop(testListObj);
  testListObj = new derivedListObj((char*) "Another node");
  listHead.addToTop(testListObj);
  testListObj = new derivedListObj((char*) "A third node");
  listHead.addToTop(testListObj);
  testListObj = new derivedListObj((char*) "Who's playing silly buggers?");
  listHead.addToTop(testListObj);

  testListObj = (derivedListObj*)listHead.getList();
  while(testListObj) {
    testListObj->doIt();
    testListObj = (derivedListObj*)testListObj->getNext();
  }
  delay(1000);          // Without this, the output fails.
  listHead.dumpList();
}


void loop() {

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("On");
  delay(1000);               // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Off");
  delay(1000);               // wait for a second
}
