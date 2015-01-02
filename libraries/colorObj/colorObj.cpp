#include "colorObj.h"

// colormapper extremes..
#define START_COLOR     0
#define END_COLOR       100

colorMapper alphaMapper;

// ****** colorObj ******

colorObj::colorObj(byte inRed, byte inGreen, byte inBlue) { setColor(inRed,inGreen,inBlue); }


colorObj::colorObj(word inColor16) { setColor(inColor16); }


colorObj::colorObj(void) { setColor(0,0,0); }


void colorObj::setColor(byte inRed, byte inGreen, byte inBlue) {

  red = inRed;
  green = inGreen;
  blue = inBlue;
}


void colorObj::setColor(word color16) {

  switch(color16) {
  case BLACK :           //0x0000
    red = 0;
    green = 0;
    blue = 0;
    break;
  case BLUE :            //0x001F
    red = 0;
    green = 0;
    blue = 255;
    break;
  case RED :             //0xF800
    red = 255;
    green = 0;
    blue = 0;
    break;
  case GREEN :           //0x07E0
    red = 0;
    green = 255;
    blue = 0;
    break;
  case CYAN :            //0x07FF
    red = 0;
    green = 255;
    blue = 255;
    break;
  case MAGENTA :         //0xF81F
    red = 255;
    green = 0;
    blue = 255;
    break;
  case YELLOW :          //0xFFE0 
    red = 255;
    green = 255;
    blue = 0;
    break;
  case WHITE :           //0xFFFF
    red = 255;
    green = 255;
    blue = 255;
    break;
  default :
    red = highByte(color16);
    green = lowByte(color16>>5);
    blue = lowByte(color16)<<3;
  }
  //Serial.print("color16 : ");Serial.println(color16,BIN);
  //printRGB();
}


// Copied from Adafruit'
word colorObj::getColor16(void) { 
  return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}


byte colorObj::getRed(void) { return red; }


byte colorObj::getGreen(void) { return green; }


byte colorObj::getBlue(void) { return blue; }


colorObj colorObj::blend(colorObj* background,byte alpha) {
    
    if (alpha>=OPAQUE) return *this;
    else if (alpha<=TRANSPARENT) return *background;
    else {
        alphaMapper.setColors(background,this);
        return alphaMapper.Map(alpha);
    }
}


/*
 void colorObj::printRGB(void) {

  Serial.print( "ColorObj RGB : ");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);
}
*/


// ****** colorMapper ******

                       
colorMapper::colorMapper(void) {
    
    redMapper = new mapper(START_COLOR,END_COLOR,0,0);
    greenMapper = new mapper(START_COLOR,END_COLOR,0,0);
    blueMapper = new mapper(START_COLOR,END_COLOR,0,0);
}
     
                       
colorMapper::colorMapper(colorObj* inStart, colorObj* inEnd) {

  redMapper = new mapper(START_COLOR,END_COLOR,inStart->getRed(),inEnd->getRed());
  greenMapper = new mapper(START_COLOR,END_COLOR,inStart->getGreen(),inEnd->getGreen());
  blueMapper = new mapper(START_COLOR,END_COLOR,inStart->getBlue(),inEnd->getBlue());
}


colorMapper::colorMapper(word startC16,word endC16) {

  colorObj startColor(startC16);
  colorObj endColor(endC16);

  redMapper = new mapper(START_COLOR,END_COLOR,startColor.getRed(),endColor.getRed());
  greenMapper = new mapper(START_COLOR,END_COLOR,startColor.getGreen(),endColor.getGreen());
  blueMapper = new mapper(START_COLOR,END_COLOR,startColor.getBlue(),endColor.getBlue());
}

colorMapper::~colorMapper(void) {
   
   delete(redMapper);
   delete(greenMapper);
   delete(blueMapper);
}


void colorMapper::setColors(colorObj* inStart, colorObj* inEnd) {
    
    redMapper->setValues(START_COLOR,END_COLOR,inStart->getRed(),inEnd->getRed());
    greenMapper->setValues(START_COLOR,END_COLOR,inStart->getGreen(),inEnd->getGreen());
    blueMapper->setValues(START_COLOR,END_COLOR,inStart->getBlue(),inEnd->getBlue());
}
                                                   
                                                   
colorObj colorMapper::Map(float percent) {

   colorObj theColor(
  (byte)round(redMapper->Map(percent)),
  (byte)round(greenMapper->Map(percent)),
  (byte)round(blueMapper->Map(percent))
    );
  return theColor;
}
  
   
void colorMapper::printColors(void) {

  colorObj startColor(
  (byte)round(redMapper->Map(START_COLOR)),
  (byte)round(greenMapper->Map(START_COLOR)),
  (byte)round(blueMapper->Map(START_COLOR))
    );
  colorObj endColor(
  (byte)round(redMapper->Map(END_COLOR)),
  (byte)round(greenMapper->Map(END_COLOR)),
  (byte)round(blueMapper->Map(END_COLOR))
    );
  Serial.println("Mapper start / end");
  startColor.printRGB();
  endColor.printRGB();
}


