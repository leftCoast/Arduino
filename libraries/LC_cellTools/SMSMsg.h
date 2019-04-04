#ifndef SMSMsg_h
#define SMSMsg_h

#include <lists.h>
#include <fileBuff.h>


// Dialog is the manager of the set of text messages associated with a contact. The
// contact keeps saved on dik the file ID of its dialog block so it can re-create it when
// necessary.

// **************************************************
// ********************  dialog  ********************
// **************************************************


class dialog :	public linkList,
					public fileBuff {
					
	public:
				dialog(contact* inContact);		`												// New creation constructor.
				dialog(contact* inContact,blockFile* inFile,unsigned long blockID);	// Recreation from file constructor.											
	virtual	~dialog(void);
	
				void				addMsg(char* newMsg,bool reply=false);
				int				numUnread(void);
				
	virtual	unsigned long	calculateBuffSize(void);
	virtual	void				writeToBuff(char* buffPtr,unsigned long maxBytes);
	virtual	unsigned long	loadFromBuff(char* buffPtr,unsigned long maxBytes);
	virtual	void				eraseFromFile(void);
	
				contact*			mContact;
};



// **************************************************
// *******************  textMsg  ********************
// **************************************************

class textMsg : public linkListObj,
					 public fileBuff {
					
	public:
				textMsg(char* newMsg,bool reply=false);				// New creation constructor.
				textMsg(blockFile* inFile,unsigned long blockID);	// Recreation from file constructor.						
	virtual	~textMsg(void);

	virtual	unsigned long	calculateBuffSize(void);
	virtual	void				writeToBuff(char* buffPtr,unsigned long maxBytes);
	virtual	unsigned long	loadFromBuff(char* buffPtr,unsigned long maxBytes);
	virtual	void				eraseFromFile(void);
				
				char*				mMsg;
				bool				mBeenRead;
				bool				mIsReply;
};

#endif