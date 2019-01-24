#ifndef contactList_h
#define contactList_h

#include <lists.h>
#include <fileBuff.h>

// **************************************************
// ********************  contact ********************
// **************************************************


class contact :	public linkListObj,
						public fileBuff {

	public:
				contact(blockFile* inFile,unsigned long blockID,unsigned long msgID,char* PN);	// Creation constructor.
				contact(blockFile* inFile,unsigned long blockID);											// Re-creation constructor.
	virtual	~contact(void);

				void	setPN(char* PN);
				void  setNickName(char* nickName);
				void  setFirstName(char* firstName);
				void  setLastName(char* lastName);
				void  setCompanyName(char* companyName);
				
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
	virtual	unsigned long	calculateBuffSize(void);
	virtual	bool				saveSubFileBuffs(void);
	virtual	void				writeToBuff(char* buffPtr,unsigned long maxBytes);
	virtual	unsigned long	loadFromBuff(char* buffPtr,unsigned long maxBytes);
};


#endif
