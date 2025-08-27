#ifndef numStream_h
#define numStream_h


#include <idlers.h>


//#define SHOW_DATA

#define DEF_IN_PORT		&Serial1	// Change to fit your hardware.
#define SYNK_CHAR    	'$'		// Marker of the start of a data set.
#define DELEM_CHAR   	','		// Marker between data items.
#define END_CHAR      	'\n'		// Noting the end of a data set.
#define DEF_TOKEN_BYTES	20			// Size of the token buffer.
#define MAX_MS				50			// How long we allow a search before calling it.
#define MAX_MSG_BYTES	200		// Max bytes for complete message. For checksum.

/* One day..
class numStreamOut {

   public:
            numStreamOut(int numParams);
   virtual  ~numStreamOut(void);

   virtual  void  writeStream(void);
   virtual  void  writeVar(int index);

            int   mNumParams;
};
*/

// **************************************  //

 
enum exitStates {
	noData,
	completed,
	finalValue,
	erroredOut
};


enum errType {
	unknownErr,
	synkTimOut,
	tokenTimOut,
	noHandler,
	addValueFail,
	badChecksum	,	
	tokenOverflow,
	msgBufOverflow
};


class numStreamIn : public idler {

   public:
				enum states {
					lookForSynk,
					readingType,
					readingParam
				};
				
            numStreamIn(Stream* inStream=DEF_IN_PORT,int tokenBuffBytes=DEF_TOKEN_BYTES);
   virtual  ~numStreamIn(void);

	virtual	void			begin(void);
	virtual	void  		reset(void);
	virtual	bool			canHandle(const char* inType);
	virtual  bool  		addValue(char* param,int paramIndex,bool isLast);	
	virtual	bool			dataChar(char inChar);
	virtual	bool			checkTheSum(void);
	virtual	exitStates	findSynkChar(void);
	virtual	exitStates	readToken(void);
   virtual  void  		idle(void);
				void			errMsg(errType inErr);
	
				Stream*	ourPort;
            bool		synk;
            states	ourState;
            char*		tokenBuff;
            int		tokenBytes;
            int		tokenIndex;
            int		paramIndex;
            char* 	msgBuf;
            int		msgBufBytes;
				int		msgIndex;
};


#endif
