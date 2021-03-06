#ifndef contactList_h
#define contactList_h

#include <lists.h>
#include <fileBuff.h>
#include <PNLabel.h>

// Contact list. This is the bases of your contact database. It reads your
// contact list from long term storage to RAM. It also keeps your contact
// list up to date in long term storage. You use this to create, edit, call
// or text contacts. Well, at least choosing contacts to call, or text. Its
// good at updating contact edits.
//
// Hopefully it'll be good to search and sort contacts as well. But that'll
// have to wait for another time.


// Handy function to strip any un-dial-able characters out of a
// c string. I'll just leave it here in case someone wants to use it.
// 
// This function is actually written in PNLabel.cpp so you have to have
// that linked in somewhere for this to work. (If your using this, there
// is a good chance it is linked in.)
void filterPNStr(char* str);


// **************************************************
// ********************  contact ********************
// **************************************************


class contact :	public linkListObj,
						public fileBuff {

	public:
				contact(blockFile* inFile,unsigned long blockID,unsigned long msgID,char* PN);	// Creation constructor.
				contact(blockFile* inFile,unsigned long blockID);											// Re-creation constructor.
	virtual	~contact(void);

				void				setPN(char* PN);
				void  			setNickName(char* nickName);
				void				setPNNick();							// If it has no nickname, this'll make the nick the phone number.
				void  			setFirstName(char* firstName);
				void  			setLastName(char* lastName);
				void  			setCompanyName(char* companyName);
				
	virtual bool				isGreaterThan(linkListObj* compObj);	// Are we greater than the obj being passed in? Primary sorting function.
	virtual bool				isLessThan(linkListObj* compObj);
    			
				unsigned long	sizeOfMsgBlock(void);
				bool				readMsgBlock(char* buff,unsigned long bytes);
				bool				saveMsgBlock(char* buff,unsigned long bytes);
				void				deleteMsgBlock(void);
				void				addMsg(char* buff,bool us);
				
	virtual	unsigned long	calculateBuffSize(void);
	virtual	void				writeToBuff(char* buffPtr,unsigned long maxBytes);
	virtual	unsigned long	loadFromBuff(char* buffPtr,unsigned long maxBytes);
	virtual	void				eraseFromFile(void);
				void				printContact(void);
					
				char*				mPN;
				char*				mNickName;
				char*				mFirstName;
				char*				mLastName;
				char*				mCompanyName;
				unsigned long	mMsgID;
				bool				mHaveMSG;
				bool				mChanged;
				bool				mErased;
};



// **************************************************
// *****************  contactList *******************
// **************************************************


class contactList :	public linkList,
							public fileBuff {

	public:
				contactList(blockFile* inFile);
	virtual	~contactList(void);

				contact*			findByPN(char* phoneNum);
				contact*			findOrAddContact(char* phoneNum);
				void				deleteContact(contact* theDoomed);
				void				addMessage(char* PN, char* msg);
	virtual	unsigned long	calculateBuffSize(void);
	virtual	bool				saveSubFileBuffs(void);
	virtual	void				writeToBuff(char* buffPtr,unsigned long maxBytes);
	virtual	unsigned long	loadFromBuff(char* buffPtr,unsigned long maxBytes);
};


#endif
