
// COmes from https://www.digitaltown.co.uk/components14ESP32GT911Screen.php


#include "Wire.h"

//Screen address for GT911
uint8_t addr  = 0x5d;  //CTP IIC ADDRESS
//Pins
const int GT911_RESET = 26;  //15;   //CTP RESET
const int GT911_INT =   24;  //2;   //CTP  INT


//I have included debounce as it makes the output easier to read and means it is set up for those building model railway control panels
unsigned long captouchbounce = 0;//like button bounce for touch
int captouched = 0;//1 means there has been a touch
//X and Y positions that will be used in further programming
int captouchx = 0;
int captouchy = 0;
int lastpixeltouched = 1000;//keeps track of the last touched pixel to prevent excess bounce

//This is some data that is needed to configure the GT911 touch screen...not my work.
unsigned char  GTP_CFG_DATA[] =
{
  0x5A, 0x20, 0x03, 0xE0, 0x01, 0x05, 0x0D, 0x00,
  0x01, 0x08, 0x28, 0x08, 0x50, 0x32, 0x03, 0x05,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x88, 0x29, 0x0A, 0x35, 0x37,
  0xD3, 0x07, 0x00, 0x00, 0x01, 0x81, 0x02, 0x1D,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x64, 0x32,
  0x00, 0x00, 0x00, 0x28, 0x5A, 0x94, 0xC5, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x98, 0x2B, 0x00, 0x84,
  0x33, 0x00, 0x74, 0x3C, 0x00, 0x67, 0x46, 0x00,
  0x5C, 0x53, 0x00, 0x5C, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10,
  0x12, 0x14, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x04, 0x06, 0x08, 0x0F, 0x10, 0x12, 0x16, 0x18,
  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x25, 0x01
};

struct TouchLocation
{
  uint16_t x;
  uint16_t y;
};


//Various variables required for the GT911 screen

TouchLocation touchLocations[5];

void inttostr(uint16_t value, uint8_t *str);
uint8_t GT911_Send_Cfg(uint8_t * buf, uint16_t cfg_len);
void writeGT911TouchRegister( uint16_t regAddr, uint8_t *val, uint16_t cnt);
uint8_t readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len );
uint8_t readGT911TouchLocation( TouchLocation * pLoc, uint8_t num );
uint32_t dist(const TouchLocation & loc);
uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2);
bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 );
uint8_t buf[80];

//Function to initialise the GT911 touch screen
void gt911setup() {

  delay(300);
  pinMode(GT911_RESET, OUTPUT);
  pinMode     (GT911_INT, OUTPUT);
  digitalWrite(GT911_RESET, LOW);
  delay(20);
  digitalWrite(GT911_INT, LOW);
  delay(50);
  digitalWrite(GT911_RESET, HIGH);
  delay(100);
  pinMode     (GT911_RESET, INPUT);
  delay(100);
  uint8_t re = GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));
  pinMode(GT911_RESET, OUTPUT);
  pinMode     (GT911_INT, OUTPUT);
  digitalWrite(GT911_RESET, LOW);
  delay(20);
  digitalWrite(GT911_INT, LOW);
  delay(50);
  digitalWrite(GT911_RESET, HIGH);
  delay(100);
  pinMode     (GT911_INT, INPUT);
  delay(100);
  re = GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));
  uint8_t bb[2];
  readGT911TouchAddr(0x8047, bb, 2);
  while (bb[1] != 32) {
    Serial.println("Capacitive touch screen initialized failure");
    pinMode(GT911_RESET, OUTPUT);
    pinMode     (GT911_INT, OUTPUT);
    digitalWrite(GT911_RESET, LOW);
    delay(20);
    digitalWrite(GT911_INT, LOW);
    delay(50);
    digitalWrite(GT911_RESET, HIGH);
    delay(100);
    pinMode     (GT911_INT, INPUT);
    delay(100);
    uint8_t re = GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));
  }
  Serial.println("Capacitive touch screen  initialized success");

}

//Not required, just in sketch to test connections
void scani2c() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}



uint8_t GT911_Send_Cfg(uint8_t * buf, uint16_t cfg_len)
{
  uint8_t retry = 0;
  for (retry = 0; retry < 5; retry++)
  {
    writeGT911TouchRegister(0x8047, buf, cfg_len);
    //if(ret==0)break;
    delay(10);
  }
  return retry;
}

//function that writes to the GT911...do not edit
void writeGT911TouchRegister( uint16_t regAddr, uint8_t *val, uint16_t cnt)
{
  uint16_t i = 0;

  Wire.beginTransmission(addr);
  Wire.write( regAddr >> 8 ); // register 0
  Wire.write( regAddr);  // register 0
  for (i = 0; i < cnt; i++, val++) //data
  {
    Wire.write( *val );  // value
  }
  uint8_t retVal = Wire.endTransmission();

}

//function that reads from the GT911...do not edit
uint8_t readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len )
{
  uint8_t i;
  uint8_t returned;
  uint8_t retVal;
  Wire.beginTransmission(addr);
  Wire.write( regAddr >> 8 ); // register 0
  Wire.write( regAddr);  // register 0
  retVal = Wire.endTransmission();

  returned = Wire.requestFrom(addr, len);    // request 1 bytes from slave device #2
  for (i = 0; (i < len) && Wire.available(); i++)

  {
    pBuf[i] = Wire.read();
  }
  return i;
}



////function that works out the touch coordinates for GT911...do not edit
uint8_t readGT911TouchLocation( TouchLocation * pLoc, uint8_t num )
{
  uint8_t retVal;
  uint8_t i;
  uint8_t k;
  uint8_t  ss[1];
  do
  {
    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one
    ss[0] = 0;

    readGT911TouchAddr( 0x814e, ss, 1);
    uint8_t status = ss[0];

    if ((status & 0x0f) == 0) break; // no points detected
    uint8_t hitPoints = status & 0x0f;
    //Serial.print("number of hit points = ");
    //  Serial.println( hitPoints );
    
    uint8_t tbuf[40];//changed to 40 as that is number called for in  readGT911TouchAddrTest( 0x8150, tbuf, 40);
    uint8_t tbuf1[8];
    readGT911TouchAddr( 0x8150, tbuf, 40);
    readGT911TouchAddr( 0x8150 + 32, tbuf1, 8);

    for (k = 0, i = 0; (i <  4 * 8) && (k < num); k++, i += 8)
    {
      pLoc[k].x = tbuf[i + 1] << 8 | tbuf[i + 0];
      pLoc[k].y = tbuf[i + 3] << 8 | tbuf[i + 2];
    }
    pLoc[k].x = tbuf1[1] << 8 | tbuf1[0];
    pLoc[k].y = tbuf1[3] << 8 | tbuf1[2];

    retVal = hitPoints;

  } while (0);

  ss[0] = 0;
  writeGT911TouchRegister( 0x814e, ss, 1);

  delay(2);
  return retVal;
}


//Function that uses the functions above to give a simple X/Y position to be used in your code
void checkfortouchscreen() {

  captouched = 0;
  pinMode     (GT911_INT, INPUT);
  uint8_t st = digitalRead(GT911_INT);
  if (!st)  //Hardware touch interrupt
  {
    //Serial.println("Touch: ");

    //This line gives the number of touch points
    //Screen can deal with 5 touches at once
    uint8_t count = readGT911TouchLocation( touchLocations, 5 );
    /*
      if(count > 0){
      Serial.print("Touches: ");
      Serial.println(count);
      }
    */

    if (count) {
      static TouchLocation caplastTouch = touchLocations[0];// only take single touch, not dealing with multitouch
      caplastTouch = touchLocations[0];
      if (millis() > captouchbounce) { //cuts out multitouch
        captouchbounce = millis() + 250;//250 is the debounce, lower makes more sensitive but more false touches

        //only using first touch for now
        if (touchLocations[0].x > 0 && touchLocations[0].y > 0) { //only do something if there is an x and Y value
          captouched = 1;
          captouchx = touchLocations[0].x;
          captouchy = touchLocations[0].y;
          //X and Y positions sent to serial monitor
          Serial.print(captouchx);
          Serial.print(" : ");
          Serial.println(captouchy);

        }
      }
    }

  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("GT911_ESP32v1");

  Wire.begin();

  scani2c();//Not required, just in sketch to test connections

  //set up the screen
  gt911setup();

}

void loop() {
  checkfortouchscreen();//keep looking for screen touches

}
