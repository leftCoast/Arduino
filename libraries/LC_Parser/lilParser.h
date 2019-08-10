#ifndef lilParser_h
#define lilParser_h

#include <lists.h>

// 8/2019 - I must have been on something when I wrote this. It all works, but WOW I can't
// make heads or tails of it now.


// A Command is: CMD params.. \n
// Parameters are seperated by whitespace.
// Succesful parsed commands return positive integers.
// Commands "in process" return 0. IE not end of text.
// Unparsable commands return -1
// Meaning? Your enum of commands should start with noCommand. IE a 0 to skip over in your
// switch statment.

// I think I'm going to do a buff = realloc(buff,size); thing here.
// Need to add numBytes for params.


#define EOL '\n'            // Set this to match your system.
#define PARAM_BUFF_SIZE 40  // One buff to fit them all..

class cmdTemplate;

class lilParser : public linkList {

	public:
				lilParser(void);
	virtual	~lilParser(void);
		
				void	addCmd(int inCmdNum, const char* inCmd);
				int	addChar(char inChar);
				int	numParams(void);
				int	getParamSize(void);
				char*	getParam(void);			// YOU HAVE TO FREE THIS WHEN YOUR DONE!
				char*	getParamBuff(void);		// YOU HAVE TO FREE THIS WHEN YOUR DONE!
				void	reset(void);
    
				cmdTemplate*	currentCmd;
				bool				firstLetter;
				bool				sawEOL;
				int				paramIndex;    
};


class cmdTemplate : public linkListObj {

	public:
				cmdTemplate(int inCmdNum, const char* inCmd);
	virtual	~cmdTemplate(void);

				void	addChar(char inChar);
				void  endParse(void);
				bool  parsing(void);
				bool  validCmd(void);
				int   cmdNumber(void);
				void	reset(void);

				int   cmdNum;
				char*	cmd;
				bool  parsingCmd;
				int   cmdIndex;
				bool  badChar;
				bool  cmdOK;
				bool  parsingParam;
				byte	paramIndex;
};

#endif

