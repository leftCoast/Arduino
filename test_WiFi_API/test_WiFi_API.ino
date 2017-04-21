void setup() {
  int retVal;

  Serial.begin(9600); while(!Serial);
  retVal = nm_bsp_init();
  Serial.println("called nm_bsp_init(); got back: ");Serial.println(retVal);
}

void loop() {
  // put your main code here, to run repeatedly:

}
