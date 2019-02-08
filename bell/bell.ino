void setup() {
  // put your setup code here, to run once:
  Serial.begin(96000);
  while(true) {
  delay(1000);
  Serial.print("X\a");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
