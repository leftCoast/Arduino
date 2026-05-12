#include <SD.h>

//#undef FILE_WRITE
//#define FILE_WRITE (O_READ|O_WRITE)
#define SD_CS     4

void setup() {
   
  File testFile;
  char strA[] = {"aaaaaaaaaaaaaaaaaaa"};
  char  strB[] = {"XXXXX"};
  char  buff[25];

  Serial.begin(9600);
  if (SD.begin(SD_CS)) {
    
    testFile = SD.open("/testFile", /*FILE_WRITE*/(O_READ | O_WRITE | O_CREAT));
    if (testFile) {
      testFile.seek(0);
      testFile.write(strA,20);
      testFile.close();
      Serial.print("opend test file, wrote [");
      Serial.print(strA);
      Serial.println("] to it.");

      testFile = SD.open("/testFile", FILE_READ);
      if (testFile) {
        testFile.read(buff,20);
        buff[20] = '\0';
        Serial.print("opend test file, read back out [");
        Serial.print(buff);
        Serial.println("]");
        testFile.close();
      } else {
        Serial.println("Open for reading failed!");
      }

      testFile = SD.open("/testFile", FILE_WRITE);  // Open for write..
      if (testFile.seek(5)) {                       // <<== seek() to 5..     
        testFile.write(strB,5);
        testFile.close();

        testFile = SD.open("/testFile", FILE_READ);
        testFile.read(buff,20);
        buff[20] = '\0';
        Serial.println(buff);
      } else {
        Serial.println("seek() failed.");
      }
    } else {
      Serial.println("Can't open the file.");
    }
  } 
}

void loop() {  }
