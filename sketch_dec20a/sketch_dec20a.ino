void setup() {
  
  char* buff;
  
  Serial.begin(9600);
  while(!Serial);
  Serial2.begin(9600);
  int count = Serial2.write(buff,10000);
  Serial.println(count);
}

void loop() {
  // put your main code here, to run repeatedly:

}
