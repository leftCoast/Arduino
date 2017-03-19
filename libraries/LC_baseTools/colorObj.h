#ifndef colorObj_h
#define colorObj_h

#include "mapper.h"
#include "multiMap.h"
// uncomment for debug stuff.
#define PRINT_COLOR

// I'm not the only one to define the colors. This list was better than mine,
// So I added it to mine. Thank you "Sumotoy". If it see's his OLED lib the #ifdef
// Should stop them from conflicting. else, you can use his.
// I didn't want this library dependant on his so this is the fix.

#ifndef _COMMON_16BIT_COLORS_H_
#define _COMMON_16BIT_COLORS_H_

// Color definitions
static const uint16_t	BLACK		=	0x0000;
static const uint16_t	WHITE   	=	0xFFFF;
static const uint16_t	NAVY   		=	0x000F;
static const uint16_t	DARKGREEN   =	0x03E0;
static const uint16_t	DARKCYAN   	=	0x03EF;
static const uint16_t	MAROON   	=	0x7800;
static const uint16_t	PURPLE   	=	0x780F;
static const uint16_t	ORANGE   	=	0xFD20;
static const uint16_t	PINK   		=	0xF81F;
static const uint16_t	OLIVE   	=	0x7BE0;
static const uint16_t	BLUE    	=	0x001F;
static const uint16_t	RED     	=	0xF800;
static const uint16_t	GREEN   	=	0x07E0;
static const uint16_t	CYAN    	=	0x07FF;
static const uint16_t	MAGENTA 	=	0xF81F;
static const uint16_t	YELLOW  	=	0xFFE0;
static const uint16_t	BRIGHT_RED	=	0xF810;
static const uint16_t	LIGHT_GREY	=	0xC618;//0x8410;
static const uint16_t	DARK_GREY	=	0x7BEF;//0x4208;
static const uint16_t	GREENYELLOW	=	0xAFE5;

#endif


class colorObj {

public:
  colorObj(byte inRed, byte inGreen, byte inBlue);
  //colorObj(colorObj* inColor);						// Wanted this one, but the compiler mixes it up with color16.
  colorObj(word color16);
  colorObj(void);

  void setColor(byte inRed, byte inGreen, byte inBlue);
  void setColor(word color16);
  void setColor(colorObj* inColor);						// Why doesn't this one get confused? Who knows?
  word getColor16(void);
    
  byte getRed(void);
  byte getGreen(void);
  byte getBlue(void);

  colorObj mixColors(colorObj* mixinColor,byte mixPercent);  // Create a new color by mixing. (Like the old blend)
  void     blend(colorObj* mixinColor,byte mixPercent);      // Just blend with myself. Percent 0% -> 100% of new color.
    
#ifdef PRINT_COLOR    
  void printRGB(void);
#endif

private :
  byte red;
  byte green;
  byte blue;
};

extern colorObj red;
extern colorObj blue;
extern colorObj white;
extern colorObj black;
extern colorObj green;
extern colorObj cyan;
extern colorObj magenta;
extern colorObj yellow;


// ****** colorMapper ******

class colorMapper {

public:
  colorMapper(void);
  colorMapper(colorObj* inStart, colorObj* inEnd);
  colorMapper(word startC16,word endC16);
  ~colorMapper(void);
  
  void setColors(colorObj* inStart, colorObj* inEnd);

  colorObj Map(float percent);

#ifdef PRINT_COLOR
  void     printColors(void);
#endif
  
private :
  mapper* redMapper;
  mapper* greenMapper;
  mapper* blueMapper;
};


// ****** colorMultiMap ******


class colorMultiMap {
    
    public :
    colorMultiMap(void);
    ~colorMultiMap(void);
    
    void      addColor(double inX, colorObj* color);  // At some numeric value we resolve to this color.
    void      clearMap(void);
    colorObj  Map(double inVal);
    
    protected :
    multiMap  redMap;
    multiMap  greenMap;
    multiMap  blueMap;
};


#endif
