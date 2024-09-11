#include "nmeaParser.h"


void addMessage(char* inStr) {

  switch(getType(inStr)) {
    case noType : break;
    case GPRMC  : theGPRMCParser.parseStr(inStr); break;
  }
}



dataTypes getType(char* inStr) {

  char buff[10];
  int  index;
  
  if (inStr) {
    if (inStr[0]!='$') return noType;
    if (inStr[6]!=',') return noType;
    index = 0;
    for(int i=1;i<6;i++) {
      buff[index] = inStr[i];
      index++;
    }
    buff[index] = '\0';
    if (!strcmp(buff,"GPRMC")) return GPRMC;
    //if (!strcmp(buff,"GPGSV")) return GPGSV;
  }
  return noType;
}



// *******************************************
//               baseNMEAParser
// *******************************************


baseNMEAParser::baseNMEAParser(void) 
  : idler() {
  
  secTimer.setTime(100,false);
  secs = 0;
  parseType = noType; 
}


baseNMEAParser::~baseNMEAParser(void) {  }


// In order to parse the string, we have to know what parser to call upon.
// This means passing the string into getType() to get the type. That allows
// one to choose the parser and actually parse the info she wants.
bool  baseNMEAParser::parseStr(char* inStr) {

  hookup();
  if (doParse(inStr)) {
    secTimer.start();
    secs = 0;
    return true;
  }
  return false;
}


void baseNMEAParser::idle(void) {

  if (secTimer.ding()) {
    secs = secs + 0.1;
    secTimer.stepTime();
  }
}


float baseNMEAParser::getDataAge(void) { return secs; }

void baseNMEAParser::showData(void) {

  Serial.print("Data age\t: ");
  Serial.print(getDataAge());
  Serial.println(" seconds");
}



// *******************************************
//               GPRMCParser
// *******************************************


GPRMCParser::GPRMCParser(void)
  : baseNMEAParser() { parseType = GPRMC; }

   
GPRMCParser::~GPRMCParser(void) {  }

  
bool GPRMCParser::doParse(char* inStr) {

  char* token;
  char  temp[20];

  token = strtok(inStr,",");        // Split the string.
  token = strtok(NULL,",");         // Jump the header.
   if (token) {                     // If we got a token..
      temp[0] = token[0];           // First two chars are the hour.
      temp[1] = token[1];
      temp[2] = '\0';
      hour = atoi(temp);            // Record it.
      temp[0] = token[2];           // Second two are the minute.
      temp[1] = token[3];
      min = atoi(temp);             // Record it.
      temp[0] = token[4];           // Last two are the seconds. (Actually sometimes seconds with decimal but for now, we ignore this.)
      temp[1] = token[5];
      sec = atoi(temp);             // Record it.
   }
   token = strtok(NULL,",");        // Check data status (A = OK, V = warning)
   if (token) {                     // If we got a token..
      valid = !strcmp(token,"A");   // Lets just chekc for an A, meaning OK. Otherwise fail.
   }
   if (!valid) return false;        // If its not ok, why bother? Bail.
   token = strtok(NULL,",");        // Parse out Latitude of fix.
   if (token) {                     // If we got a token..
      temp[0] = token[0];
      temp[1] = token[1];
      temp[2] = '\0';
      degLat = atoi(temp);
      strcpy(temp,&(token[2]));
      minLat = atof(temp);
   }
   token = strtok(NULL,",");        // N or S
   if (token) {                     // If we got a token..
      if (!strcmp(token,"N")) {
         latQuad = north;
      } else {
         latQuad = south;
      }
   }
   token = strtok(NULL,",");        // Longitude of fix
   if (token) {                     // If we got a token..
      temp[0] = token[0];
      temp[1] = token[1];
      temp[2] = '\0';
      degLon = atoi(temp);
      strcpy(temp,&(token[2]));
      minLon = atof(temp);
   }
   token = strtok(NULL,",");                    // E or W
   if (token) {                                 // If we got a token..
      if (!strcmp(token,"E")) {
         lonQuad = east;
      } else {
         lonQuad = west;
      }
   }
   token = strtok(NULL,",");                    // Speed over ground in knots   
   if (token) {
      knots = atof(token);
   }
   token = strtok(NULL,",");                    // Track made good in degrees True   
   if (token) {                                 // If we got a token..
      course = atof(token);
   }
   token = strtok(NULL,",");                    // UT date
   if (token) {                                 // If we got a token..
      temp[0] = token[0];
      temp[1] = token[1];
      temp[2] = '\0';
      day = atoi(temp);
      temp[0] = token[2];
      temp[1] = token[3];
      month = atoi(temp);
      temp[0] = token[4];
      temp[1] = token[5];
      year = atoi(temp); 
   }
   token = strtok(NULL,",");                    // Magnetic variation degrees (Easterly var. subtracts from true course)
   if (token) {                                 // If we got a token..
      magVar = atof(token);
   }
   token = strtok(NULL,",");                    // E or W
   if (token) {                                 // If we got a token..
      if (token[0]=='E') {
         varDir = east;
      } else {
         varDir = west;
      }
   }
   return true;                                 // At this point we'll call it a success.
}

void GPRMCParser::showData(void) {

   baseNMEAParser::showData();
   Serial.print("Time (zulu)        : ");
   Serial.print(hour);
   Serial.print(":");
   Serial.print(min);
   Serial.print(":");
   Serial.println(sec);
  
   Serial.print("Valid fix          : ");
   if (valid) {
      Serial.println("True");
   } else {
       Serial.println("False");
   }
   
   Serial.print("Lattitude          : ");
   Serial.print(degLat);
   Serial.print(":");
   Serial.print(minLat,4);
   Serial.print(" Deg:Min ");
   if (latQuad==north) {
      Serial.println("North");
   } else {
       Serial.println("South");
   }
   
   Serial.print("Longitute          : ");
   Serial.print(degLon);
   Serial.print(":");
   Serial.print(minLon,4);
   Serial.print(" Deg:Min ");
   if (latQuad==east) {
      Serial.println("East");
   } else {
       Serial.println("West");
   }

   Serial.print("Speed              : ");
   Serial.print(knots);
   Serial.println(" knots");
   Serial.print("True heading       : ");
   Serial.print(course);
   Serial.println(" Deg.");

   Serial.print("Date (zulu)        : ");
   Serial.print(month);
   Serial.print(":");
   Serial.print(day);
   Serial.print(":");
   Serial.println(year);

   Serial.print("Magnetic variation : ");
   Serial.print(magVar);
   Serial.print(" Deg. ");
   if (varDir==east) {
       Serial.println("East");
   } else {
       Serial.println("West");
   }
   Serial.println();
   Serial.println("*****");
   Serial.println();
}


GPRMCParser theGPRMCParser;


// *******************************************
//               GPGGAParser
// *******************************************



// *******************************************
//               NEXTParser
// *******************************************
