#ifndef contact_h
#define contact_h

class contactList {

};
  
class contact {

  public:
          contact(char* phoneNum,char* nickName);
  virtual ~contact(void);

          
          char* mPhoneNum;
          char* mNickName;
          char* mMessageList;
};


class SMSmessage  public linkListObj,
                  public idler  {

  public:
          SMSmessage(char* phoneNum,char* message);
          ~SMSmessage(void);
          
  virtual void  idle(void);
  
          char* mPhoneNum;
          char* mMessage;
          bool  sent;
          bool  seen;
};


class SMSmessageList :  public contact,
                        public linkList

        void  addMessage(char* phoneNum,char* message);
        int   getNumUnseen(void);
};
                    
#endif
