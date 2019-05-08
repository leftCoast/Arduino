
int led = 13;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("I'm here");
  Serial.flush();
  pinMode(led, OUTPUT);
  trash("123456");
  Serial.print("Still alive?!");Serial.flush();
}


void trash(char* inStr) {
  
  Serial.println("in trash");Serial.flush();
  //inStr[0] = 'x';
  Serial.println(inStr);Serial.flush();
}


void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second

}
