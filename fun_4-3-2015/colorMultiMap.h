#ifndef colormapItem_h
#define colormapItem_h

class colormapItem : public mapItem {
  
  public :
  colormapItem(double posX, double blendPercentY, colorObj* inColor);
  ~colormapItem(void);

  colorObj Map(double inVal);
  colorObj doMap(double inVal);
  
  colorObj    color;
  colorMapper ourMapper;
};


class colorMultiMap : public multiMap {
  
  public :
              colorMultiMap(void);
              ~colorMultiMap(void);
            
    void      addColor(double posX, double blendPercentY, colorObj* color);  // At some numeric value we resolve to this color and an alpha value.
    colorObj  Map(double inVal);
};

#endif
