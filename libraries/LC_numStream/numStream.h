#ifndef numStream_h
#define numStream_h

#include <extSerial.h>

#define DEF_IN_PORT		&Serial1	// Change to fit your hardware.
#define SYNK_CHAR    	'$'		// Marker of the start of a data set.
#define DELEM_CHAR   	','		// Marker between data items.
#define END_CHAR      	'\n'		// Noting the end of a data set.
#define DEF_TOKEN_BYTES	20			// Size of the token buffer.
#define MAX_MS				100		// How long we allow a search before calling it.


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
class numStreamIn :	public extSerial {

   public:
				enum states {
					lookForSynk,
					readingType,
					readingParam
				};
				
            numStreamIn(Stream* inStream=DEF_IN_PORT,int tokenBuffBytes=DEF_TOKEN_BYTES);
   virtual  ~numStreamIn(void);

	virtual	void  		reset(void);
	virtual	bool			canHandle(const char* inType);
	virtual  bool  		addValue(char* param,int paramIndex,bool isLast);	
	virtual	bool			dataChar(char inChar);
	virtual	exitStates	findSynkChar(void);
	virtual	exitStates	readToken(void);
   virtual  void  		idle(void);

            bool		synk;
            states	ourState;
            char*		tokenBuff;
            int		tokenBytes;
            int		tokenIndex;
            int		paramIndex;
};


#endif
