extern unsigned long _heap_start;
extern unsigned long _heap_end;
extern char *__brkval;

int freeram() {
  return (char *)&_heap_end - __brkval;
}


void setup() {
  while (!Serial) ;
  Serial.print("freeram = ");
  Serial.println(freeram());
  volatile char *p = (char *)malloc(152000);
  *p = 0;
  Serial.print("freeram = ");
  Serial.println(freeram());
}

void loop() {
}