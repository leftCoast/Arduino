#ifndef lilParser_h
#define lilParser_h

#include <lists.h>

// A Command is: CMD params.. \n
// Parameters are seperated by whitespace.
// It will resolve to a positive integer.
// Commands "in process" return 0. IE not end of text.
// Unparsable commands return -1

#define EOL '\n'            // Set this to match your system.
#define PARAM_BUFF_SIZE 40  // One buff to fit them all..

class cmdTemplate;

class lilParser : public linkList {

  public:
    lilParser(void);

    void  addCmd(int inCmdNum, char* inCmd);
    int   addChar(char inChar);
    int   numParams(void);
    bool  getParam(char* buff);
    void  reset(void);
    
    cmdTemplate*  currentCmd;
    bool          firstLetter;
    bool          sawEOL;
    int           paramIndex;    
};


class cmdTemplate : public linkListObj {

  public:
    cmdTemplate(int inCmdNum, char* inCmd);

    void  addChar(char inChar);
    void  endParse(void);
    bool  parsing(void);
    bool  validCmd(void);
    int   cmdNumber(void);
    void  reset(void);
    bool  getParam(char* buff);

    int   cmdNum;
    char* cmd;
    bool  parsingCmd;
    int   cmdIndex;
    bool  badChar;
    bool  cmdOK;
    bool  parsingParam;
    int   paramIndex;
};

#endif

