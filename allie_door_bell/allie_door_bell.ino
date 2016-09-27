
#define LED_PIN 13
#define BUTTON_PIN 12


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
}

void loop() {
  digitalWrite(LED_PIN, !digitalRead(BUTTON_PIN));
}
