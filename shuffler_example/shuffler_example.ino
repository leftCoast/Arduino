
#include "lists.h"
#include "timeObj.h"


// ********************************************


class shuffleItem : public linkListObj {

   public:
               shuffleItem(void);
   virtual     ~shuffleItem(void);
};


shuffleItem::shuffleItem(void)
   : linkListObj() {  }

   
shuffleItem::~shuffleItem(void) {  }



// ********************************************
   
class shuffler : public linkList {

   public:
                  shuffler(void);
   virtual        ~shuffler(void);

            void           addItem(shuffleItem* inItem);
            void           shuffle(long seed=0);
            shuffleItem*   peek(int inedx);
            shuffleItem*   pop(void);
            void           clearList(void);

            linkList shuffled;
};



shuffler::shuffler(void)
   : linkList() {  }

   
shuffler::~shuffler(void) {  }



void shuffler::addItem(shuffleItem* inItem) { addToTop(inItem); }


void shuffler::shuffle(long seed) {

   long           index;
   long           count;
   shuffleItem*   theItem;

   randomSeed(seed);
   count = getCount();
   while(!isEmpty()) {
      index = random(0,count);
      count--;
      theItem = getByIndex(index);
      unlinkObj(theItem);
      shuffled.addToTop(theItem);
   }
   theList = theItem;
   shuffled.looseList();
}


// Returns pointer to the item found at index, but does not unlink it.
shuffleItem* shuffler::peek(int index) { return getByIndex(index); }
/*
   shuffleItem* theItem;
   
   theItem =  getByIndex(inedx);
   if (!theItem) {
      Serial.println("NULL coming!!");
   }
   return theItem;
 }
*/

// Unlinks the top item of the list, hands back a pointer to it. But does not delete it
// from memory, You have to take care of that.
shuffleItem* shuffler::pop(void) {

   linkListObj*   topObj;
   
   topObj = getFirst();
   if (topObj) {
      unlinkTop();
   }
   return topObj;
}      


// Deletes all nodes from the list. Deletes them from memory as well.
void shuffler::clearList(void) { dumpList(); }



// ********************************************

class myData   : public shuffleItem {

   public:
               myData(int inData);
   virtual     ~myData(void);

            int   data;
};

myData::myData(int inData) 
   :shuffleItem() { data = inData; }
   
myData::~myData(void) {  }


timeObj  timer(5000);
shuffler ourShuffler;
int   numItems;

void setup() {
   
   myData* newItem;
   
   for (int i=1;i<53;i++) {
      newItem = new myData(i);
      //Serial.print("Adding item with data : ");//Serial.flush();
      Serial.print(newItem->data);//Serial.flush();
      Serial.print(" ");
      ourShuffler.addItem(newItem);
   }
   Serial.println();
   numItems = ourShuffler.getCount();
   Serial.print("We have ");
   Serial.print(numItems);
   Serial.println(" items");
   for (int i=0;i<52;i++) {
      newItem = ourShuffler.peek(i);
      if (newItem) {
         Serial.print(newItem->data);
         Serial.print(" ");
      } else {
         Serial.println("Got a NULL!");
      }
   }
   Serial.println();
}


void loop() {
   
   myData* theItem;
   
   if (timer.ding()) {
       Serial.print ("result : ");
      ourShuffler.shuffle(analogRead(A0));
      for (int i=0;i<numItems;i++) {
         theItem = (myData*)ourShuffler.peek(i);
         Serial.print(theItem->data);
         Serial.print(" ");
      }
      Serial.println();
      timer.start();
   }
   
}
