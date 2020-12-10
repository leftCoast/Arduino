#include "GPS_NMEA.h"
#include <Arduino.h>

GPS_NMEA::GPS_NMEA(void) {  }


GPS_NMEA::~GPS_NMEA(void) {  }


bool GPS_NMEA::parseStr(char* inStr) {

   if (strstr(inStr,"$GPRMC")) return decodeGPRMC(inStr);
   return false;
}


bool GPS_NMEA::decodeGPRMC(char* inStr) {
   
   char* token;
   char  temp[20];
   
   //Serial.println(inStr);
   token = strtok(inStr,",");                   // The header.
   if (strcmp(token,"$GPRMC")) return false;    // If we are parsing the wrong thing? bail.
   token = strtok(NULL,",");                    // Parse out UTC.
   if (token) {                                 // If we got a token..
      temp[0] = token[0];                       // First two chars are the hour.
      temp[1] = token[1];
      temp[2] = '\0';
      $GPRMC.hour = atoi(temp);                 // Record it.
      temp[0] = token[2];                       // Second two are the minute.
      temp[1] = token[3];
      $GPRMC.min = atoi(temp);                  // Record it.
      temp[0] = token[4];                       // Last two are the seconds. (Actually sometimes seconds with decimal but for now, we ignore this.)
      temp[1] = token[5];
      $GPRMC.sec = atoi(temp);                  // Record it.
   }
   token = strtok(NULL,",");                    // Check data status (A = OK, V = warning)
   if (token) {                                 // If we got a token..
      $GPRMC.valid = !strcmp(token,"A");        // Lets just chekc for an A, meaning OK. Otherwise fail.
   }
   if (!$GPRMC.valid) return false;             // If its not ok, why bother? Bail.
   token = strtok(NULL,",");                    // Parse out Latitude of fix.
   if (token) {                                 // If we got a token..
      temp[0] = token[0];
      temp[1] = token[1];
      temp[2] = '\0';
      $GPRMC.degLat = atoi(temp);
      strcpy(temp,&(token[2]));
      $GPRMC.minLat = atof(temp);
   }
   token = strtok(NULL,",");                    // N or S
   if (token) {                                 // If we got a token..
      if (!strcmp(token,"N")) {
         $GPRMC.latQuad = north;
      } else {
         $GPRMC.latQuad = south;
      }
   }
   token = strtok(NULL,",");                    // Longitude of fix
   if (token) {                                 // If we got a token..
      temp[0] = token[0];
      temp[1] = token[1];
      temp[2] = '\0';
      $GPRMC.degLon = atoi(temp);
      strcpy(temp,&(token[2]));
      $GPRMC.minLon = atof(temp);
   }
   token = strtok(NULL,",");                    // E or W
   if (token) {                                 // If we got a token..
      if (!strcmp(token,"E")) {
         $GPRMC.lonQuad = east;
      } else {
         $GPRMC.lonQuad = west;
      }
   }
   token = strtok(NULL,",");                    // Speed over ground in knots   
   if (token) {
      $GPRMC.knots = atof(token);
   }
   token = strtok(NULL,",");                    // Track made good in degrees True   
   if (token) {                                 // If we got a token..
      $GPRMC.course = atof(token);
   }
   token = strtok(NULL,",");                    // UT date
   if (token) {                                 // If we got a token..
      temp[0] = token[0];
      temp[1] = token[1];
      temp[2] = '\0';
      $GPRMC.day = atoi(temp);
      temp[0] = token[2];
      temp[1] = token[3];
      $GPRMC.month = atoi(temp);
      temp[0] = token[4];
      temp[1] = token[5];
      $GPRMC.year = atoi(temp); 
   }
   token = strtok(NULL,",");                    // Magnetic variation degrees (Easterly var. subtracts from true course)
   if (token) {                                 // If we got a token..
      $GPRMC.magVar = atof(token);
   }
   token = strtok(NULL,",");                    // E or W
   if (token) {                                 // If we got a token..
      if (token[0]=='E') {
         $GPRMC.varDir = east;
      } else {
         $GPRMC.varDir = west;
      }
   }
   return true;                                 // At this point we'll call it a success.
}


void GPS_NMEA::showGPRMC(void) {

   Serial.print("Time (zulu)        : ");
   Serial.print($GPRMC.hour);
   Serial.print(":");
   Serial.print($GPRMC.min);
   Serial.print(":");
   Serial.println($GPRMC.sec);
  
   Serial.print("Valid fix          : ");
   if ($GPRMC.valid) {
      Serial.println("True");
   } else {
       Serial.println("False");
   }
   
   Serial.print("Lattitude          : ");
   Serial.print($GPRMC.degLat);
   Serial.print(":");
   Serial.print($GPRMC.minLat,4);
   Serial.print(" Deg:Min ");
   if ($GPRMC.latQuad==north) {
      Serial.println("North");
   } else {
       Serial.println("South");
   }
   
   Serial.print("Longitute          : ");
   Serial.print($GPRMC.degLon);
   Serial.print(":");
   Serial.print($GPRMC.minLon,4);
   Serial.print(" Deg:Min ");
   if ($GPRMC.latQuad==east) {
      Serial.println("East");
   } else {
       Serial.println("West");
   }

   Serial.print("Speed              : ");
   Serial.print($GPRMC.knots);
   Serial.println(" knots");
   Serial.print("True heading       : ");
   Serial.print($GPRMC.course);
   Serial.println(" Deg.");

   Serial.print("Date (zulu)        : ");
   Serial.print($GPRMC.month);
   Serial.print(":");
   Serial.print($GPRMC.day);
   Serial.print(":");
   Serial.println($GPRMC.year);

   Serial.print("Magnetic variation : ");
   Serial.print($GPRMC.magVar);
   Serial.print(" Deg. ");
   if ($GPRMC.varDir==east) {
       Serial.println("East");
   } else {
       Serial.println("West");
   }
   Serial.println();
   Serial.println("*****");
   Serial.println();
}
