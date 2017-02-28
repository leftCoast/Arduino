#ifndef lilParser_h
#define lilParser_h

#include <lists.h>

// A Command is: CMD params.. \n
// Parameters are seperated by whitespace.
// Succesful parsed commands return positive integers.
// Commands "in process" return 0. IE not end of text.
// Unparsable commands return -1

// I think I'm going to do a buff = realloc(buff,size); thing here.
// Need to add numBytes for params.
#define EOL '\n'            // Set this to match your system.
#define PARAM_BUFF_SIZE 40  // One buff to fit them all..

class cmdTemplate;

class lilParser : public linkList {

  public:
    lilParser(void);
		~lilParser(void);
		
    void  addCmd(int inCmdNum, char* inCmd);
    int   addChar(char inChar);
    int   numParams(void);
    int		getParamSize(void);
    char* getParam(void);
    void  reset(void);
    
    cmdTemplate*  currentCmd;
    bool          firstLetter;
    bool          sawEOL;
    int           paramIndex;    
};


class cmdTemplate : public linkListObj {

  public:
    cmdTemplate(int inCmdNum, char* inCmd);
    ~cmdTemplate(void);

    void  addChar(char inChar);
    void  endParse(void);
    bool  parsing(void);
    bool  validCmd(void);
    int   cmdNumber(void);
    void  reset(void);

    int   cmdNum;
    char* cmd;
    bool  parsingCmd;
    int   cmdIndex;
    bool  badChar;
    bool  cmdOK;
    bool  parsingParam;
    byte   paramIndex;
};

#endif

