#ifndef colorObj_h
#define colorObj_h

#include "mapper.h"

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF


class colorObj {

public:
  colorObj(byte inRed, byte inGreen, byte inBlue);
  colorObj(word color16);
  colorObj(void);

  void setColor(byte inRed, byte inGreen, byte inBlue);
  void setColor(word color16);
  word getColor16(void);

  byte getRed(void);
  byte getGreen(void);
  byte getBlue(void);

  void printRGB(void);

private :

  word  convertRGB(byte red, byte green, byte bue);

  byte red;
  byte green;
  byte blue;
};


// ****** colorMapper ******

class colorMapper {

public:
  colorMapper(colorObj* inStart, colorObj* inEnd);
  colorMapper(word startC16,word endC16);
  ~colorMapper(void);
  
  //word     Map(float percent);
  colorObj Map(float percent);
  void     printColors(void);
  
private :
  mapper* redMapper;
  mapper* greenMapper;
  mapper* blueMapper;
};

#endif
