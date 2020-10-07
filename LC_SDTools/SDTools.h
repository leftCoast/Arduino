#ifndef SDTools_h
#define SDTools_h

#include <SD.h>
#include <lists.h>


bool filterFile(char* fileName);

class dirItem :   public linkListObj {

   public:
            dirItem(File inItem);
   virtual  ~dirItem(void);
   
            char*          getName();
            unsigned long  getNumBytes();
            bool           getIsDir();
            
            char*          mName;
            unsigned long  mNumBytes;
            bool           mIsDir;
};


class dirList :   public linkList {

   public:
                  dirList(void);
   virtual        ~dirList(void);
   
                  void           readDir(char* path);
                  dirItem*       findItem(int index);
                  char*          getItemName(int index);
                  unsigned long  getItemSize(int index);
                  bool           getItemIsDir(int index);
};


class sdObj {

   public:
               sdObj(int inCS);
   virtual     ~sdObj(void);

            bool  begin(void);
            bool  setWD(char* newWd);
            char* getWD(void);
            bool  changeWD(char* dirName);   // Pass in ".." to go up a level.
            

            int   mCS;
            bool  mInit;
            char* mWd;
}



#endif
