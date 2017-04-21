
#include <SPI.h>
#include <WiFi101.h>

#define CS  6
#define RST 5
#define IRQ 4

char ssid[] = "The Garage";      //  your network SSID (name)
char pass[] = "ytrewq12";           // your network password

WiFiServer server(23);


void setup() {

  int status;
  
  //Configure pins for Adafruit ATWINC1500 Breakout
  WiFi.setPins(CS, IRQ, RST);


  Serial.begin(9600); while (!Serial);

  // check for the presence of the hardware:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // attempt to connect to WiFi network:
  status = WL_IDLE_STATUS;
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);  // Connect.

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin(); // start the server:
  printWiFiStatus(); // you're connected now, so print out the status:
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void loop() {

  WiFiClient  aClient;
  char        aChar;
  
  aClient = server.available();       // Anyone have data?
  if (aClient) {                      // True means yes I got data.
    if (aClient.available()>0) {
      while(aClient.available()>0) {    // Just like Serial, lets read 'till gone.
        aChar = aClient.read();         // Read a byte.
        server.write(aChar);            // Broadcast it to everyone.
        Serial.print(aChar);            // Show it on the monitor.
      }
    } else {
      Serial.print("aClient.available() returns:");Serial.println(aClient.available());
      aClient.write("Well hello there!");
    }
  }
}





