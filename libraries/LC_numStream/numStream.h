#ifndef numStream_h
#define numStream_h

#include <Arduino.h>

#define DEF_IN_PORT		&Serial1	// Change to fit your hardware.
#define DEF_OUT_PORT		&Serial1	// Change to fit your hardware.
#define SYNK_CHAR    	'$'
#define DELEM_CHAR   	','
#define END_CHR      	'\n'
#define DEF_TOKEN_BYTES	20

/*
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


class numStreamIn {

   public:
            numStreamIn(Stream* inStream=DEF_IN_PORT,int tokenBuffBytes=DEF_TOKEN_BYTES);
   virtual  ~numStreamIn(void);

				void	copyStream(numStreamIn* inNumStream);
				void  reset(void);
   virtual  void  readStream(void);
   virtual  void  readVar(int index,bool lastField);

				Stream*	mStream;
            bool  	mSynk;
            char* 	mTokenBuff;
            int		mNumTokenBytes;
            int   	mIndex;
};


#endif
