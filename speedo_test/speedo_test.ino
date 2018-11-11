
#define LED_PIN     13
#define SENSOR_PIN  2

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  digitalWrite(SENSOR_PIN,0);
}

void loop() {

  //int sensorState;
  //delay(100);
  //sensorState = digitalRead(SENSOR_PIN);
  digitalWrite(LED_PIN, digitalRead(SENSOR_PIN));
}
