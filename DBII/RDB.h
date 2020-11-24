#include <Arduino.h>


class dataField {

   public:
               dataField(char* fieldName);
   virtual     ~dataField(void);

               setText(char* inText);
               getAsText
               getAsNumber
               getAsFunction
               getAsID

               
       
};





class dataSet  : public linkedList {

   public:
               dataSet(char* setName);
   virtual     ~dataSet(void);
};


class dataRecord {
   
};


class RDB  : public linkedList {

   public:
               RDB(char* RDBName);
   virtual     ~RDB(void);
};
