#define BEEP_PIN    23

void setup() {
   pinMode(BEEP_PIN, OUTPUT);
   digitalWrite(BEEP_PIN, HIGH); //Means off.
   beep();
}

void beep() { tone(BEEP_PIN, 500,35); }

void loop() {
   // put your main code here, to run repeatedly:

}
