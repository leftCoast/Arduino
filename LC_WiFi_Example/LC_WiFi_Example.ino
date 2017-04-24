#include <WiFiObj.h>

#include <LC_SPI.h>
#include <mapper.h>

#define CS  6
#define RST 5
#define IRQ 4

mapper signalMapper(-120,0,0,100);
WiFiObj network(CS, IRQ, RST);

void setup() {

  Serial.begin(9600); while (!Serial);

  Serial.println("Try blank begin()");
  network.begin();
  Serial.println("Mac address");
  printMacAddress();
  Serial.println("Take down network");
  network.end();
  
  Serial.println("Calling begin with login..!");
  if (network.begin("The Garage","ytrewq12")) {
    Serial.println("SUCCESS!!! A CONNECTION!");
    printWiFiStatus();
    printMacAddress();
  } else {
    Serial.println("No connection.. Sad day.");
  }
  Serial.println("Take down network");
  //network.end();
  //Serial.println("Network should be down now.");


  // scan for existing networks:
  Serial.println("Scanning available networks...");
  listNetworks();
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(network.getSSID());

  // print your WiFi shield's IP address:
  IPAddress ip = network.getLocalIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = network.getRSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void loop() {
  
  delay(10000);
  // scan for existing networks:
  Serial.println("Scanning available networks...");
  listNetworks();
}

void printMacAddress() {
  // the MAC address of your WiFi shield
  byte mac[6];

  // print your MAC address:
  network.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}


void listNetworks() {
  // scan for nearby networks:
  int numSsid = network.scanNetworks();
  Serial.println("** Scan Networks **");
  if (numSsid == -1)
  {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    printinField(network.SSID(thisNet),15);
    Serial.print("\tSignal: ");
    int dbm = network.RSSI(thisNet);
    Serial.print(dbm);
    Serial.print(" dBm\t");
    int percent = signalMapper.Map(dbm);
    Serial.print(percent);Serial.print("%");
    Serial.print("\tChannel: ");
    Serial.print(network.channel(thisNet));
    byte bssid[6];
    Serial.print("\t\tBSSID: ");
    printBSSID(network.BSSID(thisNet));
    Serial.print("\tEncryption: ");
    printEncryptionType(network.encryptionType(thisNet));
  }
}


void printBSSID(byte bssid[]) {

  byte i;
  
  for(i=0;i<5;i++) {
  print2Digits(bssid[i]);
  Serial.print(":");
  }
  print2Digits(bssid[i]);
}


void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}


void print2Digits(byte thisByte) {
  if (thisByte <= 0xF) {
    Serial.print("0");
  }
  Serial.print(thisByte, HEX);
}


#define FIELD_MAX 32
char outStr[FIELD_MAX+1];

void printinField(char* inStr,byte field) {

  if (field>FIELD_MAX) {
    field = FIELD_MAX;
  }
  memset(outStr,' ',FIELD_MAX);
  if (inStr[0]=='\0') {
    strcpy(outStr,"- Hidden -");
    outStr[10] = ' ';
  } else {
    strcpy(outStr,inStr);
    outStr[strlen(inStr)] = ' '; 
  }
  outStr[field] = '\0';
  Serial.print(outStr);
}

