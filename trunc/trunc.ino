

void setup() {
  
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Hello?");
  
  for(int i=20;i>-20;i--) {
    float f = i/10.0;
    int tf = trunc(f);
    Serial.print("f: ");Serial.print(f);Serial.print("  trunc(f) : ");Serial.println(tf);
  }
  
}

void loop() {
  

}
