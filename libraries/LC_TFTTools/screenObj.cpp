#include "screenObj.h"

screenObj* screen;  // Now we have a global screen so we don't have
                    // to pass the silly thing around all the time.

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

/*
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
    
  return

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

word  colorObj::convertRGB(byte inRed, byte inGreen, byte inBlue) { return screen->color565(inRed,inGreen,inBlue); }


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


word colorMapper::Map(float percent) {

  //Serial.println("colorMapper::Map(float percent)");
  colorObj tempColor(
  (byte)round(redMapper->Map(percent)),
  (byte)round(greenMapper->Map(percent)),
  (byte)round(blueMapper->Map(percent))
    );
  return tempColor.getColor16();
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

*/

// ****** screenObj ******

//Sets up the global screen object for us to use.
boolean initScreen(int inRotation) {

  screen = new screenObj();
  if (screen)
    return(screen->init(inRotation));
   else
   return(false);
}


screenObj::screenObj(void) 
:
Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, 0)
{
  ts = new TouchScreen(XP, YP, XM, YM, OHMS);
  xMapper  = new mapper(TS_MINX, TS_MAXX, width(), 0);
  yMapper  = new mapper(TS_MINY, TS_MAXY, height(), 0);
}


boolean screenObj::init(int inRotation) {

  uint16_t identifier;

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  reset();
  identifier = readID();
  if (identifier == 0x9325 || identifier == 0x9328) {
    begin(identifier); 
    setRotation(inRotation);
    return true;
  } 
  else
    return false;
}


void screenObj::fillRectGradient(word inX,word inY,word inXSize,word inYSize,word startColor16,word endColor16,boolean vertical) {

  colorObj startColor(startColor16);
  colorObj endColor(endColor16);

  fillRectGradient(inX,inY,inXSize,inYSize,&startColor,&endColor,vertical);
}


void screenObj::fillRectGradient(word inX,word inY,word inXSize,word inYSize,colorObj* startColor,colorObj* endColor,boolean vertical) {

  colorMapper gradient(startColor, endColor);
  mapper posMapper(1,2,1,2);
  if (vertical) {
    posMapper.setValues(inY,inY+inYSize,0,100);
    for (int y=inY;y<inY+inYSize;y++) {
        drawFastHLine(inX, y, inXSize, (gradient.Map(posMapper.Map(y)).getColor16()));
    }
  } 
  else {
    posMapper.setValues(inX,inX+inXSize,0,100);
    for (int x=inX;x<inX+inXSize;x++) {
      drawFastVLine(x, inY, inYSize, (gradient.Map(posMapper.Map(x)).getColor16()));
    }
  }
}

void screenObj::fillScreenGradient(word startColor16,word endColor16,boolean vertical) {

  fillRectGradient(0,0,_width,_height,startColor16,endColor16,vertical);
}


void screenObj::fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical) {

  fillRectGradient(0,0,_width,_height,startColor,endColor,vertical);
}


void screenObj::drawPixelInvert(word x,word y) {

  word color;

  
  color = readPixel(x,y);
  color = ~color;
  drawPixel(x,y,color);
}

void screenObj::frameRectInvert(Point loc, word width,word height) {

  word traceX;
  word traceY;
  word endX;
  word endY;

  
  endX = loc.x + width;
  endY = loc.y + height;
  traceX = loc.x;
  traceY = loc.y;
  while(traceX<endX) {
    drawPixelInvert(traceX,loc.y);
    drawPixelInvert(traceX,endY);
    traceX++;
  }
  while(traceY<endY) {
    drawPixelInvert(loc.x,traceY);
    drawPixelInvert(endX,traceY);
    traceY++;
  }
}


Point screenObj::getPoint(void) {

  Point pt;

  pt = ts->getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pt.x = xMapper->Map(pt.x);
  pt.y = yMapper->Map(pt.y);
  switch (getRotation()) {
  case 0:
    break;
  case 1:
    swap(pt.x,pt.y);
    pt.x = pt.x;
    pt.y = height() - pt.y;
    break;
  case 2:
    pt.x = width() - pt.x;
    pt.y = height() - pt.y;
    break;
  case 3:
    swap(pt.x,pt.y);
    pt.x = width() - pt.x;
    pt.y = pt.y;
    break;
  }
  return(pt);
}


boolean screenObj::pressed(Point inPt) { return(inPt.z > MINPRESSURE && inPt.z < MAXPRESSURE); }


