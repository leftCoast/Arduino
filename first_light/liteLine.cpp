
#include "liteLine.h"


 liteLine::liteLine(int inLength) { length = inLength; }
    
    
 liteLine::~liteLine(void) { }
     
      
int liteLine::getLength(void) { return length; }


colorObj liteLine::getColor(int index) { return calcColor(index); }
 
 
colorObj  liteLine::calcColor(int index) { 

   colorObj aColor(10,10,10);
  return aColor;
  }


void liteLine::setLights(Adafruit_NeoPixel* lites,int index,boolean wrap) {

   int liteIndex;
   colorObj color;
   if (wrap) {
      for(int i=0;i<length;i++) {
         color = calcColor(i);
         liteIndex = index-i;
         if (liteIndex<0) {
            liteIndex = liteIndex + lites->numPixels();
         }
         lites->setPixelColor(liteIndex, color.getRed(), color.getGreen(), color.getBlue());
      }
   } else {
      for(int i=0;i<length;i++) {
         color = calcColor(i);
         lites->setPixelColor(index-i, color.getRed(), color.getGreen(), color.getBlue());
      }
   }
   lites->show();
}


// *************** monoColorLine ****************
//    One color for the entire line. Simple.
// **********************************************



 monoColorLine::monoColorLine(int inLength)
 
    : liteLine(inLength)
    { }
    

colorObj monoColorLine::calcColor(int index) { return theColor; }


void monoColorLine::setColor(word inColor) { theColor.setColor(inColor); }
   
   
void monoColorLine::setColor(byte red,byte green,byte blue) { theColor.setColor(red,green,blue); }


void monoColorLine::setColor(colorObj aColor) { theColor = aColor; }
   
   
   
// *************** multiColorLine ****************
//       A color for each pixel. Fancier..
// **********************************************


 multiColorLine::multiColorLine(int inLength) 
    
    :liteLine(inLength)
    { 
       defColor.setColor(255,0,0);
       colors = (colorObj*)malloc(sizeof(colorObj)*getLength());
    }
   
    
 multiColorLine::~multiColorLine(void) {
    
    if (colors) {
       free(colors);
       colors = NULL;
    }
 }
 
    
colorObj multiColorLine::calcColor(int index) { 
   
   if (colors) {
      return colors[index];
   } else {
      return defColor;
   }
}


void multiColorLine::setColor(byte red,byte green,byte blue,int index) {
   
   colorObj aColor(red,green,blue);
   setColor(&aColor,index);
}


void multiColorLine::setColor(colorObj* inColor,int index) { 
   
   if (index>=0 && index<getLength()) {
      if (colors) {
         colors[index] = *inColor;
      } else {
         defColor = *inColor;
      }
   }
}

