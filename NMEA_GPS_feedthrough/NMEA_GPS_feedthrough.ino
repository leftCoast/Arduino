void setup() {
   Serial.begin(9600);
   Serial1.begin(9600);
   

}

void loop() {

   char aChar;
   
   if (Serial1.available()) {
      aChar = Serial1.read();
      Serial.print(aChar);
   }
}
