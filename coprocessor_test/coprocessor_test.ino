void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.write("Help!!\n");
  delay(250);
}
