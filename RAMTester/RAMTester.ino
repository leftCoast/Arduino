

#include <label.h>
#include <fontLabel.h>
#include <navigation.h>
#include <RamMonitor.h>
#include <eventMgr.h>

extern	char* cleanName(const char* inName);

RamMonitor tRam;

void setup() {
  
  while(!Serial) delay(100);
  tRam.initialize();
  Serial.println(sizeof(label));
  Serial.println(sizeof(fontLabel));
  Serial.println(sizeof(navigation));
  Serial.println(sizeof(event));
  Serial.print(F("RAM   : "));
	Serial.println(tRam.adj_free());
  Serial.print(F("STACK : "));
	Serial.println(tRam.stack_total());
  Serial.println(cleanName("   virtual void valueBox::idle()"));
}

void loop() {
  // put your main code here, to run repeatedly:

}
