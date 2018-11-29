class taggedBuff {

  public:
  
      taggedBuff(unsigned long blockID);

          unsigned long   getType(void);
  virtual unsigned long   calculateBuffSize(void);
  virtual unsigned long   writeToBuff(char* buffPtr,unsigned long maxBytes);
  virtual unsigned long   loadFromBuff(char* buffPtr);

  unsigned long ourTag;
};


class title : public taggedBuff {

  public:

          title(bool inIsPerson=true);

  virtual ~title(void);

          void  setPrefix(char* buff);
          void  setFirstName(char* buff);
          void  setMiddleName(char* buff);
          void  setLastName(char* buff);
          void  setOrgName(char* buff);

          void  getPrefix(char* buff);
          void  getFirstName(char* buff);
          void  getMiddleName(char* buff);
          void  getLastName(char* buff);
          void  getOrgName(char* buff);
          void  getCalulatedName(char* buff);
          bool  getIsPerson(void);

          char* prefix;
          char* firstName;
          char* middleName;
          char* lastName;
          char* orgName;
          bool  isPerson;
};


class address : public taggedBuff {

  public:
  address(void);
  address(char* inStreetAddr,
          char* inStreetName,
          char* inCityName,
          char* inStateName,
          char* inZipcode );
  virtual ~address(void);

          char* addrLine1(char* inName);
          char* addrLine2(void);
          char* addrLine3(void);
          
  char* streetAddr;
  char* streetName;
  char* cityName;
  char* stateName;
  char* zipcode;
};


class phoneNumber : public taggedBuff {

  public:
  
  phoneNumber(void);
  phoneNumber(char* inPhoneNumber);
  virtual ~phoneNumber(void);

  char*   getNumber(void);
  char*   setNumber(char* inNumber);
  
  char*   numberAsTyped;
};


class contact : public taggedBuff {

  public:

  contact(void);
  contact(title* inTitle);
  contact(address* inAddress);
  contact(phoneNumber* inNumber);

  virtual   ~contact(void);

  void      getName(char* buff);
  void      getAddress1(char* buff);
  void      getAddress2(char* buff);
  void      getAddress3(char* buff);
  void      getPhoneNumber(char* buff);
};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
