#ifndef numStream_h
#define numStream_h

#include <Arduino.h>

#define outPort      Serial1	// Change to fit your hardware.
#define inPort       Serial1	// Change to fit your hardware.
#define SYNK_CHAR    '$'
#define DELEM_CHAR   ' '
#define END_CHR      '\n'


class numStreamOut {

   public:
            numStreamOut(int numParams);
   virtual  ~numStreamOut(void);

   virtual  void  writeStream(void);
   virtual  void  writeVar(int index);

            int   mNumParams;
};


// **************************************  //


class numStreamIn {

   public:
            numStreamIn(int tokenBuffBytes=15);
   virtual  ~numStreamIn(void);

				void  reset(void);
   virtual  void  readStream(void);
   virtual  void  readVar(int index);

            bool  mSynk;
            bool	mComplete;
            char* mTokenBuff;
            int   mIndex;
};


#endif
