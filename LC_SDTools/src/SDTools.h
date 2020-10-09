#ifndef SDTools_h
#define SDTools_h

#include <SD.h>
#include <lists.h>

extern char nameBuff[];

bool filterFile(char* fileName);




// *******************************************************
//                      strItem class
// *******************************************************


class strItem :   public linkListObj {

   public:
            strItem(void);
            strItem(char* inStr);
   virtual  ~strItem(void);
   
   			bool	setStr(char* inStr);
   			char*	getStr(void);
   			int	getStrlen(void);
   			
   protected:
            char*	mStr;
            
};



// *******************************************************
//                      pathStr class
// *******************************************************


class pathStr :   public linkList {

   public:
				pathStr(void);
   virtual	~pathStr(void);
   
   			int	getPathBytes(void);
   			void	getPath(char* path);
   			char*	getPath(void);
   			void	setPath(char* itemStr);
};



// *******************************************************
//                      dirItem class
// *******************************************************

  
class dirItem :   public strItem {

   public:
            dirItem(File inItem);
   virtual  ~dirItem(void);
   
            char*          getName();
            unsigned long  getSize();
            bool           getIsDir();
            
            unsigned long  mNumBytes;
            bool           mIsDir;
};



// *******************************************************
//                      dirList class
// *******************************************************


class dirList :   public linkList {

   public:
                  dirList(void);
   virtual        ~dirList(void);
   
                  void           readDir(char* path);
                  dirItem*       findItem(int index);
                  char*          getItemName(int index);
                  unsigned long  getItemSize(int index);
                  bool           getItemIsDir(int index);
                  
                  int				mLastIndex;
                  dirItem*			mLastItem;
};

/*
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

*/

#endif
