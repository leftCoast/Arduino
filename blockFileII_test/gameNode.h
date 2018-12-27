#ifndef gameNode_h
#define gameNode_h

#include "fileBuff.h"

class gameNode : public fileBuff {

  public:
    gameNode(blockFile* inFile, unsigned long blockID, char* inQuestion);
    gameNode(blockFile* inFile, unsigned long blockID);
    virtual ~gameNode(void);

    bool            isAnswer(void);
    void            getQuestion(char* buff);
    unsigned long   yesResponse(char* buff);
    unsigned long   noResponse(char* buff);
    unsigned long   getYes(void);
    unsigned long   getNo(void);
    void            setYes(unsigned long ID);
    void            setNo(unsigned long ID);

    virtual unsigned long   calculateBuffSize(void);
    virtual void            writeToBuff(char* buffPtr, unsigned long maxBytes);
    virtual unsigned long   loadFromBuff(char* buffPtr, unsigned long maxBytes);

    char*           question;
    unsigned long   yesID;
    unsigned long   noID;
};

#endif
