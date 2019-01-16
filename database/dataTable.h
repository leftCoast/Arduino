#ifndef dataTable_h
#define dataTable_h

#include <lists.h>
#include "dataField.h"


class dataTable : public IDnameBuff {

    public:
            dataTable(blockFile* inFile,unsigned long blockID,char* inName);
            dataTable(blockFile* inFile,unsigned long blockID);
    virtual ~dataTable(void);

    virtual unsigned long calculateBuffSize(void);
    virtual void          writeToBuff(char* buffPtr,unsigned long maxBytes);
    virtual unsigned long loadFromBuff(char* buffPtr,unsigned long maxBytes);
  
            void  addField(char* fieldName,fieldType newType);
            void  renameField(char* oldName,char* newName);
            void  retypeField(char* fieldName,,fieldType newType);
            void  deleteField(char* fieldName);

            bool          mDelete;
            int           mNextFieldID;
            linkList      mFieldList;
            unsigned long mTempateListID;
            linkList      mRecordSet;
              unsigned long mRecordListID;
};

#endif
