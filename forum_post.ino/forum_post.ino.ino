#include <mapper.h>

float myArray[100];
mapper myMapper(0,1023,0,100);
unsigned long startMicros;
unsigned long endMicros;

void setup() {
   startMicros = micros();
   for(int i=0;i<100;i++) {
      myArray[i] = analogRead(A0);
   }
   endMicros = micros();
   Serial.begin(9600);
   Serial.println("100 readings into float array from A0");
   Serial.println(String(startMicros));
   Serial.println(String(endMicros));
   Serial.print(String(endMicros-startMicros));
   Serial.println(" micros");
   Serial.println();
   
   startMicros = micros();
   for(int i=0;i<100;i++) {
      myArray[i] = map(analogRead(A0),0,1023,0,100);
   }
   endMicros = micros();
   Serial.println("100 readings INT MAPPED into float array from A0");
   Serial.println(String(startMicros));
   Serial.println(String(endMicros));
   Serial.print(String(endMicros-startMicros));
   Serial.println(" micros");
   Serial.println();

   startMicros = micros();
   for(int i=0;i<100;i++) {
      myArray[i] = myMapper.map(analogRead(A0));
   }
   endMicros = micros();
   Serial.println("100 readings FLOAT MAPPED into float array from A0");
   Serial.println(String(startMicros));
   Serial.println(String(endMicros));
   Serial.print(String(endMicros-startMicros));
   Serial.println(" micros");
   Serial.println();
}

void loop() {
   

}
