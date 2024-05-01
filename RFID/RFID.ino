/**************************************************************************/
/*!
    @file     readMifare.pde
    @author   Adafruit Industries
	@license  BSD (see license.txt)

    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.

    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:

    - Authenticate block 4 (the first block of Sector 1) using
      the default KEYA of 0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
    - If authentication succeeds, we can then read any of the
      4 blocks in that sector (though only block 4 is read here)

    If the card has a 7-byte UID it is probably a Mifare
    Ultralight card, and the 4 byte pages can be read directly.
    Page 4 is read by default since this is the first 'general-
    purpose' page on the tags.


   This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
   This library works with the Adafruit NFC breakout
   ----> https://www.adafruit.com/products/364

   Check out the links above for our tutorials and wiring diagrams
   These chips use SPI or I2C to communicate.

   Adafruit invests time and resources providing this open source code,
   please support Adafruit and open-source hardware by purchasing
   products from Adafruit!

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a software SPI connection (recommended):
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// Or use hardware Serial:
//Adafruit_PN532 nfc(PN532_RESET, &Serial1);

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
   
void setup(void) {
   Serial.begin(115200);
   while (!Serial) delay(10); // for Leonardo/Micro/Zero

   Serial.println("Hello!");

   nfc.begin();

   uint32_t versiondata = nfc.getFirmwareVersion();
   if (! versiondata) {
      Serial.print("Didn't find PN53x board");
      while (1); // halt
   }
   // Got ok data, print it out!
   Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
   Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
   Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

   Serial.println("Waiting for an ISO14443A Card ...");
}


void printBytes(byte* bytes,int numBytes) {

   for (int i=0;i<numBytes;i++) {
      Serial.print(bytes[i]);
      if (i+1<numBytes) Serial.print(".");
   }
}


void printChars(byte* bytes,int numBytes) {

   for (int i=0;i<numBytes;i++) {
      Serial.print((char)bytes[i]);
   }
}


bool authenticate(void) {

   byte knownKeys[8][6] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
   };
   
   bool  success;
   int   i;
   
   success = false;
   i = 0;
   while(!success && i<8) {
      Serial.print("Trying to authenticate block 4 with KEY: ");
      Serial.print(i);
      Serial.print("\t");
      success = nfc.mifareclassic_AuthenticateBlock(uid,uidLength,4,0,knownKeys[i]);
      if (success)
         Serial.println("Success!");
      else
         Serial.println("Fail.");
      i++;
   }
   return(success);
}


void readData(bool mifare) {

   uint8_t  data[32]; // Fits either.
   bool     success;

   for (int i=4;i<8; i++) {
      Serial.print("Reading Block ");
      Serial.print(i);
      Serial.print(":");
      if (mifare) {
         success = nfc.mifareultralight_ReadPage (i, data);
      } else {
         success = nfc.mifareclassic_ReadDataBlock(i, data);
      }
      if (success) {
         Serial.println((char*)data);
      } else {
         Serial.println("Fail to read block ");
      }
   }
}

   
void loop(void) {
   uint8_t success;
   
   // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
   // 'uid' will be populated with the UID, and uidLength will indicate
   // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
   success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

   if (success) {
      // Display some basic information about the card
      Serial.println("Found an ISO14443A card");
      Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);
      Serial.print("  UID Value: ");
      printBytes(uid, uidLength);
      Serial.println();
      Serial.print("  UID Value: ");
      printChars(uid, uidLength);
      Serial.println();
      Serial.println();
      if (uidLength == 4) {
         // We probably have a Mifare Classic card ...
         Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

         // Now we need to try to authenticate it for read/write access
         // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
         //Serial.println("Trying to authenticate block 4 with default KEYA value");
         //uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

         // Start with block 4 (the first block of sector 1) since sector 0
         // contains the manufacturer data and it's probably better just
         // to leave it alone unless you know what you're doing
         //success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
         success = authenticate();
         if (success) {
            Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
            readData(false);
            
            //uint8_t data[16];
            // If you want to write something to block 4 to test with, uncomment
            // the following line and this text should be read back in a minute
            //memcpy(data, (const uint8_t[]){ 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0 }, sizeof data);
            // success = nfc.mifareclassic_WriteDataBlock (4, data);
            
         } else {
            Serial.println("Ooops ... authentication failed: Try another key?");
         }
      }else if (uidLength == 7) {                  // We probably have a Mifare Ultralight card ...
         Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
         Serial.println("Reading data..");   
         readData(true);                    // Try to read the general-purpose user pages (#4..7)
      } else {
         Serial.println("UID lenght is off.");
      }
      delay(1000);
   }
}
