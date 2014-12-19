#include "colorObj.h"

// ****** colorObj ******

colorObj::colorObj(byte inRed, byte inGreen, byte inBlue) { 
  setColor(inRed,inGreen,inBlue); 
}


colorObj::colorObj(word inColor16) { 
  setColor(inColor16); 
}


colorObj::colorObj(void) { 
  setColor(0,0,0); 
}


void colorObj::setColor(byte inRed, byte inGreen, byte inBlue) {

  red = inRed;
  green = inGreen;
  blue = inBlue;
  //Serial.println("setColor(byte inRed, byte inGreen, byte inBlue)");
  //Serial.print( "ColorObj RGB : ");Serial.print(red);Serial.print(", ");Serial.print(green);Serial.print(", ");Serial.println(blue);
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


word colorObj::getColor16(void) { 
  return convertRGB(red,green,blue); 
}


byte colorObj::getRed(void) { 
  return red; 
}


byte colorObj::getGreen(void) { 
  return green; 
}


byte colorObj::getBlue(void) { 
  return blue; 
}


void colorObj::printRGB(void) {

  Serial.print( "ColorObj RGB : ");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);
}

//word  colorObj::convertRGB(byte inRed, byte inGreen, byte inBlue) { return screen->color565(inRed,inGreen,inBlue); }


// ****** colorMapper ******

colorMapper::colorMapper(colorObj* inStart, colorObj* inEnd) {

  //Serial.println("colorMapper::colorMapper(colorObj* inStart, colorObj* inEnd)");
  redMapper = new mapper(0,100,inStart->getRed(),inEnd->getRed());
  greenMapper = new mapper(0,100,inStart->getGreen(),inEnd->getGreen());
  blueMapper = new mapper(0,100,inStart->getBlue(),inEnd->getBlue());
}


colorMapper::colorMapper(word startC16,word endC16) {


  //Serial.println("colorMapper::colorMapper(word startC16,word endC16)");
  colorObj startColor(startC16);
  colorObj endColor(endC16);

  redMapper = new mapper(0,100,startColor.getRed(),endColor.getRed());
  greenMapper = new mapper(0,100,startColor.getGreen(),endColor.getGreen());
  blueMapper = new mapper(0,100,startColor.getBlue(),endColor.getBlue());
}

colorMapper::~colorMapper(void) {
   
   delete(redMapper);
   delete(greenMapper);
   delete(blueMapper);
}


/*
word colorMapper::Map(float percent) {

  //Serial.println("colorMapper::Map(float percent)");
  colorObj tempColor(
  (byte)round(redMapper->Map(percent)),
  (byte)round(greenMapper->Map(percent)),
  (byte)round(blueMapper->Map(percent))
    );
  return tempColor.getColor16();
}
*/

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
  (byte)round(redMapper->Map(0)),
  (byte)round(greenMapper->Map(0)),
  (byte)round(blueMapper->Map(0))
    );
  colorObj endColor(
  (byte)round(redMapper->Map(100)),
  (byte)round(greenMapper->Map(100)),
  (byte)round(blueMapper->Map(100))
    );
  Serial.println("Mapper start / end");
  startColor.printRGB();
  endColor.printRGB();
}


