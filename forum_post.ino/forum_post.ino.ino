int greenLed      = 2;
int yellowLed     = 3;
int redLed        = 4;
int potent        = A0;
int potState      = 0;

void setup() {
  Serial.begin(9600);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(potent, INPUT);
}

void loop() {
   
  potState = analogRead(potent);
  Serial.println(potState);
  if (potState > 1 && potState < 250) {
    digitalWrite(greenLed, HIGH);
  }
  if (potState > 251 && potState < 749) {
    digitalWrite(yellowLed, HIGH);
  }
  if (potState > 750) {
    digitalWrite(redLed, HIGH);
  }
  delay(50);
  digitalWrite(yellowLed,LOW);
  digitalWrite(greenLed,LOW);
  digitalWrite(redLed,LOW);
}
