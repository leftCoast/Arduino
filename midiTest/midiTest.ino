#include <SD.h>
#include <SDTools.h>
#include <adafruit_1947.h>

#define DSP_CS      10
#define SD_CS       4

//https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html

void setup() {
   
   bool haveScreen;

   Serial.begin(57600);
   haveScreen = false;
   screen = (displayObj*) new adafruit_1947(DSP_CS,-1);
   if (screen) {
      if (screen->begin()) {
         screen->setRotation(PORTRAIT);
         haveScreen = true;
      }
   }
   if (!haveScreen) {
      Serial.println("NO SCREEN!");                               
      Serial.flush();                                             
      while(true);                                                
   }
   if (!SD.begin(SD_CS)) {                                        
      Serial.println("NO SD CARD!");                              
      Serial.flush(); 
      while(true);                                                                           
   }
   readMIDIFIle();
}

struct MIDIHeader {

   char     chunkID[4];
   uint32_t chunkSize;
   uint16_t formatType;
   uint16_t numTracks;
   uint16_t timeDiv;
};


struct trackHeader {
   
   char     chunkID[4];
   uint32_t chunkSize;
};


struct eventHeader {
   
   uint32_t deltaTime;
   byte     eventType;
   byte     channel;
   byte     param1;
   byte     param2;
};

void readEventHeader(eventHeader* header,File MIDIFile);


char  buff[5];

char* chunkID(char* rawBytes) {
   
   for(int i=0;i<4;i++) {
      buff[i] = rawBytes[i];
   }
   buff[4] = '\0';
   return buff;
}


void readMIDIFIle(void) {

   BYTE_SWAP
   File        MIDIFile;
   MIDIHeader  theMIDIHeader;
   trackHeader theTrackHeader;
   eventHeader  anEventHeader;
    
   MIDIFile = SD.open("MIDI.MID",FILE_READ);
   if (MIDIFile) {
      MIDIFile.read(&theMIDIHeader.chunkID,4);
      theMIDIHeader.chunkSize = read32(MIDIFile);
      theMIDIHeader.formatType = read16(MIDIFile);
      theMIDIHeader.numTracks = read16(MIDIFile);
      theMIDIHeader.timeDiv = read16(MIDIFile);

      MIDIFile.read(&theTrackHeader.chunkID,4);
      theTrackHeader.chunkSize = read32(MIDIFile);
      readEventHeader(&anEventHeader,MIDIFile);
      MIDIFile.close();
      
      Serial.print("Chunk ID     :");Serial.println(chunkID(theMIDIHeader.chunkID));
      Serial.print("Chunk size   :");Serial.println((unsigned long)theMIDIHeader.chunkSize);
      Serial.print("Format type  :");Serial.println(theMIDIHeader.formatType);
      Serial.print("Num tracks   :");Serial.println(theMIDIHeader.numTracks);
      Serial.print("Time division:");Serial.println(theMIDIHeader.timeDiv);
      Serial.println();
      Serial.print("Chunk ID     :");Serial.println(chunkID(theTrackHeader.chunkID));
      Serial.print("Chunk size   :");Serial.println((unsigned long)theTrackHeader.chunkSize);
      Serial.println();
      Serial.print("Delta  time  :");Serial.print(anEventHeader.deltaTime);Serial.print('\t');Serial.println(anEventHeader.deltaTime,HEX);
      Serial.print("EventType    :");Serial.print(anEventHeader.eventType);Serial.print('\t');Serial.println(anEventHeader.eventType,HEX);
      Serial.print("Channel      :");Serial.print(anEventHeader.channel);Serial.print('\t');Serial.println(anEventHeader.channel,HEX);
      Serial.print("Param 1      :");Serial.print(anEventHeader.param1);Serial.print('\t');Serial.println(anEventHeader.param1,HEX);
      Serial.print("Param 2      :");Serial.print(anEventHeader.param2);Serial.print('\t');Serial.println(anEventHeader.param2,HEX);
   }
}

uint32_t varLenToValue(File MIDIFile) {

   bool     done;
   byte     readByte;
   uint32_t result;

   result = 0;
   done = false;
   while(!done) {
      if (MIDIFile.read(&readByte,1)!=1) {   // If we can't read a byte from the file..
         Serial.println("Can't read file.");
         done = true;                        // Something broke, we're done.
      } else {                               // Esle, we did get a byte..
         Serial.print("Var byte :");
         Serial.println(readByte);
         if (!bitRead(readByte,7)) {         // If high bit was 0.. (Last byte)
            Serial.println("Last byte");
            result = readByte & 0x000000FF;  // Stamp in these bits.
            done = true;                     // We are done!
         } else {                            // Else, high bit was set! Oh ohh..
            Serial.println("Not last byte.");
            result = readByte & 0x0000007F;  // Stamp in these bits.
            result<<7;                       // Move 'em over.
         }
      } 
   }
   return result;
}


void readEventHeader(eventHeader* header,File MIDIFile) {

   byte twoNibbles;
   
   header->deltaTime = varLenToValue(MIDIFile);
   if (MIDIFile.read(&twoNibbles,1)!=1) {   // If we can't read a byte from the file..
      return;
   } else {
      Serial.println(twoNibbles,BIN);
      header->eventType = twoNibbles;
      header->eventType>>4;
      header->eventType = header->eventType & 0x0F;
      header->channel = 0;
      header->channel = twoNibbles & 0x0F;
      if (MIDIFile.read(&twoNibbles,1)!=1) {
         return;
      } else {
         header->param1 = twoNibbles;
      }
      if (MIDIFile.read(&twoNibbles,1)!=1) {
         return;
      } else {
         header->param2 = twoNibbles;
      }
   }
}


void loop() {
   

}
