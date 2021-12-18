#ifndef LM35_H
#define LM35_H

/*
 This is a little class to run the TI-LM35 temp
 sensing chip from an analog pin on an Arduino.
 
 Constructor takes a pin number. 
 For example : LM35 mySensor(A2);
 When you want a reading just call either tempC()
 for centigrade or tempF() for Farenheight.
 
 This assumes you have the sensor hooked directly to
 5V & Ground with the data out pin conected to the 
 analog input on your Arduino. No fancy circuits, 
 just the 3 wires.
 
 Enjoy!
 
 -jim lee
 */

class LM35 {
  
  public:
    LM35(int inPin);
    ~LM35(void);
    
    float centToDegF(float cent);
    float tempC(void);
    float tempF(void);
    
 private:
    int     pin;
};

#endif
