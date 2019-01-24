#ifndef dataField_h
#define dataField_h

enum fieldType { textFld, numberFld };

class dataField {

  public:
            dataField(void);
            dataField(int16_t fieldID,char* fieldName);
  virtual   ~dataField(void);

            void  getText(char* fieldText);
            void  setText(char* fieldText);
            int   getValue(void);
            float getValue(void);
            void  setValue(int fieldVal);
            void  setValue(float fieldVal);
            int   calculateBuffSize(void);
            void  writeToBuff(uint8_t* buffPtr,int numBytes);
            int   loadFromBuff(uint8_t* buffPtr);
            
            int16_t   mFieldID;
            fieldType mFieldType;
            char*     mFieldText;
};
