
#include <mapper.h>
#include <multiMap.h>
#include <colorObj.h>



colorObj  color2(&red);
colorObj  color1(RED);

void setup() {
  Serial.begin(9600);

#ifdef PRINT_COLOR
  Serial.println("Color1:");
  color1.printRGB();
  Serial.println(""); Serial.println("Color2:");
  color2.printRGB();
#endif

}


void loop() {
  // put your main code here, to run repeatedly:

}
