#ifndef textComObj_h
#define textComObj_h

#include <timeObj.h>
#include <idlers.h>
#include <lilParser.h>

class textComObj : public idler {

  public:
          textComObj(void);
  virtual ~textComObj(void);

          void  begin(void);
          void  textOut(char aChar);
          void  textOut(char* buff);
          void  textOut(uint32_t val,int format);
          void  textOutln(uint32_t val,int format);
          void  textOutln(char* buff=NULL);
          void  checkTextCom(char inChar='\0');
          void  handleTextCom(char* buff);

  protected:
          void  initParams(void);
          void  printParams(void);
          void  printReadings(void);
          void  printGReadings(void);
          void  doPrintReadings(void);
          void  doPrintGReadings(void);
          void  setDryLimit(void);
          void  setWaterTime(void);
          void  setSoakTime(void);
          void  setPlantName(void);
          void  setPWMPercent(void);
          void  setPWMPeriod(void);
          void  setDry(void);
          void  setMud(void);
          void  turnPump(void);
          void  logCommand(void);
          void  listDirectory(void);
          
  virtual void  idle(void);
          
          lilParser mParser;
          bool      mAutoRead;
          timeObj   readTimer;
          Print*    ourPort;
};

extern textComObj textComs;

#endif
