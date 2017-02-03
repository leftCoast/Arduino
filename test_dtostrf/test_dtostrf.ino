double aFloat;
char textBuff[20];

void setup() {
  Serial.begin(9600);while(!Serial);
  aFloat = 0.0000706;
  Serial.print("The float: ");Serial.println(aFloat,10);
  sprintf(textBuff,"%0.0f",aFloat);
  Serial.print("Result fix 0: ");Serial.println(textBuff);
  sprintf(textBuff,"%0.1f",aFloat);
  Serial.print("Result fix 1: ");Serial.println(textBuff);
  sprintf(textBuff,"%0.2f",aFloat);
  Serial.print("Result fix 2: ");Serial.println(textBuff);
}

void loop() {
  // put your main code here, to run repeatedly:

}
/*
float aFloat;
char textBuff[20];

void setup() {
  Serial.begin(9600);while(!Serial);
  aFloat = 0.0000706;
  Serial.print("The float: ");Serial.println(aFloat,10);
  dtostrf(aFloat,0,0,textBuff);
  Serial.print("Result fix 0: ");Serial.println(textBuff);
  dtostrf(aFloat,0,1,textBuff);
  Serial.print("Result fix 1: ");Serial.println(textBuff);
   dtostrf(aFloat,0,2,textBuff);
  Serial.print("Result fix 2: ");Serial.println(textBuff);
}

void loop() {
  // put your main code here, to run repeatedly:

}

*/
