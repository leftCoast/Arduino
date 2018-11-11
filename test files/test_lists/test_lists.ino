
#include <lists.h>


int countLinkListObj(linkListObj* listHead) {

  int           count;
  linkListObj*  trace;
  
  count = 0;
  if (listHead) {
    trace = listHead->getNext();
    while(trace) {
      count++;
      trace = trace->getNext();
    }
  }
  return count;
}

void deleteLinkListObjs(linkListObj* listHead,int numToKill) {

  linkListObj*  trace;
  int           i;

  if (listHead) {
    for(i=1;i<=numToKill;i++) {
      trace = listHead->getNext();
      if (trace) {
        listHead->next = trace->next;
        delete(trace);
      }
    }
  } else {
    Serial.println("Hey! deleteLinkListObjs() got passed in a NULL pointer!");
  }
}


void testLinkListObj(void) {
  
  linkListObj   listPtr;
  linkListObj*  newlistObj;
  linkListObj*  trace;
  int           i;
  int           count;
  
  Serial.println("creating 100 nodes.");
  for (i=0;i<=99;i++) {
    newlistObj = new linkListObj();
    if (newlistObj) {
      newlistObj->linkAfter(&listPtr);
    } else {
      Serial.println("I was given a bloody NULL pointer!");
      Serial.print("i = ");Serial.println(i);
      while(true);  // Lock up on errors.
    }
  }
  Serial.println("That was quick wasn't it?");
  Serial.println("Lets count them.");
  count = countLinkListObj(&listPtr);
  Serial.print("count = ");Serial.println(count);
  Serial.println("Lets delete 50 of them.");
  deleteLinkListObjs(&listPtr,50);
  Serial.println("Lets count them.");
  count = count = countLinkListObj(&listPtr);
  Serial.print("count = ");Serial.println(count);
  Serial.println("Lets delete 51 of them.");
  deleteLinkListObjs(&listPtr,51);
   Serial.println("Lets count them.");
  count = count = countLinkListObj(&listPtr);
  Serial.print("count = ");Serial.println(count);

  Serial.println("creating 100 more nodes adding to the end.");
  for (i=0;i<=99;i++) {
    newlistObj = new linkListObj();
    if (newlistObj) {
      newlistObj->linkToEnd(&listPtr);
    } else {
      Serial.println("I was given a bloody NULL pointer!");
      Serial.print("i = ");Serial.println(i);
      while(true);  // Lock up on errors.
    }
  }
  Serial.println("That was still quick wasn't it?");
  Serial.println("Lets count them.");
  count = countLinkListObj(&listPtr);
  Serial.print("count = ");Serial.println(count);
  Serial.println("Lets delete 50 of them.");
  deleteLinkListObjs(&listPtr,50);
  Serial.println("Lets count them.");
  count = count = countLinkListObj(&listPtr);
  Serial.print("count = ");Serial.println(count);
  Serial.println("Lets delete 51 of them.");
  deleteLinkListObjs(&listPtr,51);
   Serial.println("Lets count them.");
  count = count = countLinkListObj(&listPtr);
  Serial.print("count = ");Serial.println(count);
}


void setup() {

  Serial.begin(9600);
  Serial.println("Ready to go!");Serial.println();
  testLinkListObj();
}


void loop() {
  // put your main code here, to run repeatedly:

}
