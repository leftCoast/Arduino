  
colormapItem::colormapItem(double posX, double blendPercentY, colorObj* inColor) 
  : mapItem(posX,blendPercentY) {
  
  color = *inColor;
  
}
  
  
colormapItem::~colormapItem(void) {  }


colorObj colormapItem::Map(double inVal) {
  
}


colorObj colormapItem::doMap(double inVal) {
  
}
  

class colorMultiMap : public multiMap {
  
  public :
              colorMultiMap(void);
              ~colorMultiMap(void);
            
    void      addColor(double posX, double blendPercentY, colorObj* color);  // At some numeric value we resolve to this color and an alpha value.
    colorObj  Map(double inVal);
};

