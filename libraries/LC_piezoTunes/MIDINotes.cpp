#include <MIDINotes.h>
#include <toneObj.h>
#include <SDTools.h>


// MIDI file reading and decoding tools. What a twisted web they wove. I hate old file
// formats! It's like they were designed by the IRS. Weird twitchy encodings. Esoteric
// descriptions. Anyway, almost all of the info I gleaned was from this web page.
//
// https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html
//
// It was, for me, the most clear and understandable of anything I could find. And a lot
// of the documents seem to have been erased for some reason.
//
// Odd that.


void fileError(const char* function) {

	Serial.print("File error in : ");
	Serial.print(function);
	Serial.println("()");
}


char  buff[5];	// Used by the file rippers to do the 4 char string stuff.


// Take a raw 4 char text buffer and output a 4 char c string WITH '\0'. This doesn't
// change the ID, it's just for showing it. You need the trialing '\0' to print it.
char* chunkID(char* rawBytes) {

   for (int i = 0; i < 4; i++) {
      buff[i] = rawBytes[i];
   }
   buff[4] = '\0';
   return buff;
}


// MIDI has Bizzarre multi byte encoding. Point this at one. It decodes it for you. But, 
// Don't bother in Meta tags because it seems they forgot to encode those. I roll my eyes..
bool varLenToValue(uint32_t* result,File MIDIFile) {

   bool     done;
   byte     readByte;

   *result = 0;											// Clear result bits.
   done = false;											// Not done yet.
   while (!done) {										// And while we're not done..
      if (!MIDIFile.available()) {					// If we DON'T have a byte to read..
      	fileError(__func__);							// Show error..
			return false;									// Bail out.
		}														//
      if (MIDIFile.read(&readByte, 1)==1) {		// If we can grab a byte..
			if (bitRead(readByte, 7)) {        		// If high bit was set! Oh ohh..
				readByte = readByte & 0b01111111;	// Clip off high bit.
				*result = *result | readByte;			// Set the lower 7 bits..
				*result = *result << 7;            	// Move 'em over.
			} else {                            	// Else, high bit was 0.. (Last byte)
				*result = *result | readByte;			// Stamp in these last 7 bits.			
				done = true;                     	// We are done!
			}													//
		} else {												// Else, we got a file problem.
			fileError(__func__);							// Pass in our function name.
			return false;									// Return a fail.
		}														//
   }															//
   return true;											// Pass back success.
}


// First thing I found in the MIDI files was a header. This reads and decodes it.
bool readMIDIHeader(MIDIHeader* theHeader, File MIDIFile) {

	if (MIDIFile.available()) {											// If we have any bytes left..
		if (MIDIFile.read(&theHeader->chunkID, 4)==4) {				// If we can grab the bytes..
			if (read32(&theHeader->chunkSize,MIDIFile)) {			// Chunk size.
				if (read16(&theHeader->formatType,MIDIFile)) {		// Format type.
					if (read16(&theHeader->numTracks,MIDIFile)) {	// Num tracks.
						if (read16(&theHeader->timeDiv,MIDIFile)) {	// Read time divider.
							return true;										// We get all these? return success!
						}															//
					}																//
				}																	//
			}																		//
		}																			//
	}																				//
	fileError(__func__);														// We got an error, note it here.								
	return false;																// And return a fail.
}


// Next in line is tracks. The files we can deal with have (1) track. (So far)
bool readTrackHeader(trackHeader* theHeader, File MIDIFile) {

   if (MIDIFile.available()) {									// If we have any bytes left..
		if (MIDIFile.read(&theHeader->chunkID, 4)==4) {		// If we can grab the bytes..
			if (read32(&theHeader->chunkSize,MIDIFile)) {	// Can we grab chunk size?
				return true;											// If we made it here, we're a success!
			}																//
		}																	//
	}																		//
	fileError(__func__);												// We got an error, note it here.
	return false;														// And return a fail.
}


// Tracks are made up of a list of events. This decodes an event.
bool readEventHeader(eventHeader* header, File MIDIFile) {

   byte twoNibbles;
   
	if (MIDIFile.available()) {										// If we have any bytes left..
		if (varLenToValue(&header->deltaTime,MIDIFile)) {		// Get the packed delta time.
			if (MIDIFile.read(&twoNibbles, 1)==1) {				// Grab a byte for event & channel.
				header->eventType = twoNibbles;						// Start unpacking the nibbles.
				header->eventType = header->eventType >> 4;		// I guess we want the high nibble.
				header->eventType = header->eventType & 0x0F;	// Clear the high nibble.
				header->channel = 0;										// Clear channel.
				header->channel = twoNibbles & 0x0F;				// Stamp the lower nibble here.
				if (MIDIFile.read(&twoNibbles, 1)==1) {			// Grab another byte.
					header->param1 = twoNibbles;						// Stuff it into param1.
					if (MIDIFile.read(&twoNibbles, 1)==1) {		// Grab our last byte..
						header->param2 = twoNibbles;					// Stuff it into param2.
						return true;										// If we got here, it's a success!
					}															//
				}																//
			}																	//
		}																		//
	}																			//
	fileError(__func__);													// Show error here.
	return false;															// And return a fail.
}


// There are basically note events and Meta events. And a third I've not seen. This looks
// at an event Header, and says whether its Meta or not.
bool isMetaTag(eventHeader* theHeader) {

   return theHeader->eventType == 0xF && theHeader->channel == 0xF;
}


// If it IS a meta event, this'll decode the meta header for you. Once you read the Meta
// header, you need to either go through the data, or jump past it. See next two functions
// for that choice.
void readMetaEvent(eventHeader* theHeader, metaEvent* theEvent, File MIDIFile) {

   theEvent->metaType = theHeader->param1;
   theEvent->numBytes = theHeader->param2;
   theEvent->location = MIDIFile.position();
}


// Nine times out of ten you just want to jump past Meta events. They typically have
// trailing data blocks. This calculates the hyperspace jump to land past the entire event
// from the point of just finishing reading the header (above).
bool jumpMetaEvent(metaEvent* theEvent, File MIDIFile) {

   unsigned long newLocation;
   
   newLocation = theEvent->location + theEvent->numBytes;
   return MIDIFile.seek(newLocation);
}


// This basically drags you through the meta event's data. Some have data, some of it's
// readable text. Some is binary. This will show you what it found. Byte by grueling byte.
bool readAndShowMetaData(metaEvent* theEvent, File MIDIFile) {

   char  metaByte;
   
   for(uint32_t i=0;i<theEvent->numBytes;i++) {
      if (MIDIFile.read(&metaByte, 1)!=1) return false;
      Serial.print(metaByte);
   }
   Serial.println();
   return true;
}


// Just read the MIDI header and wonder what it has in it? This'll print it out.
void showMIDIHeader(MIDIHeader* theMIDIHeader) {

   Serial.print("Chunk ID    :"); Serial.println(chunkID(theMIDIHeader->chunkID));
   Serial.print("Chunk size  :"); Serial.println((unsigned long)theMIDIHeader->chunkSize);
   Serial.print("Format type :"); Serial.println(theMIDIHeader->formatType);
   Serial.print("Num tracks  :"); Serial.println(theMIDIHeader->numTracks);
   Serial.print("Time div    :"); Serial.println(theMIDIHeader->timeDiv);
}


// Just read the track header and wonder what it has in it? This'll print it out.
void showTrackHeader(trackHeader* theHeader) {

   Serial.print("Chunk ID    :"); Serial.println(chunkID(theHeader->chunkID));
   Serial.print("Chunk size  :"); Serial.println((unsigned long)theHeader->chunkSize);
}


// Just like the others. Just read the event header and wonder what it has in it? This'll
// print it out.
void showEventHeader(eventHeader* theHeader) {

   Serial.print("Delta  time :"); Serial.print(theHeader->deltaTime); Serial.print("\t0x"); Serial.println(theHeader->deltaTime, HEX);
   Serial.print("EventType   :"); Serial.print(theHeader->eventType); Serial.print("\t0x"); Serial.println(theHeader->eventType, HEX);
   Serial.print("Channel     :"); Serial.print(theHeader->channel); Serial.print("\t0x"); Serial.println(theHeader->channel, HEX);
   Serial.print("Param 1     :"); Serial.print(theHeader->param1); Serial.print("\t0x"); Serial.print(theHeader->param1, HEX); Serial.print("\t0x");Serial.println(theHeader->param1, BIN);
   Serial.print("Param 2     :"); Serial.print(theHeader->param2); Serial.print("\t0x"); Serial.print(theHeader->param2, HEX); Serial.print("\t0x");Serial.println(theHeader->param2, BIN);
}


// Same for this guy.
void showMetaEvent(metaEvent* theEvent) {

   Serial.print("Meta Type :"); Serial.print(" 0x");Serial.println(theEvent->metaType, HEX);
   Serial.print("NumBytes  :"); Serial.println(theEvent->numBytes);
   Serial.print("Location  :"); Serial.println(theEvent->location);
}


// This'll read through a MIDI file. And, using the functions above, show you what it
// thinks is in there. Basically a debugging tool
void decodeFile(const char* filePath) {

   BYTE_SWAP
   File        MIDIFile;
   MIDIHeader  theMIDIHeader;
   trackHeader theTrackHeader;
   eventHeader anEventHeader;
   metaEvent   aMetaEvent;
   byte        aByte;
   uint32_t		temp;
   uint32_t    microsPerBeat;
   float			MsPerTick;
   bool        done;
   
   MIDIFile = SD.open(filePath, FILE_READ);
   if (MIDIFile) {

      if (readMIDIHeader(&theMIDIHeader, MIDIFile)) {
      	showMIDIHeader(&theMIDIHeader);
      	Serial.println();
      } else {
      	Serial.println("fail MIDI header.");
      }
      if (readTrackHeader(&theTrackHeader, MIDIFile)) {
      	showTrackHeader(&theTrackHeader);
      	Serial.println();
      } else {
      	Serial.println("fail track header.");
      }
      done = false;
      SDFileErr = false;
      while(!done && !SDFileErr) {
      	while(!Serial.available());		// I think I recall this was basically a break point.
      	Serial.read();
         if (readEventHeader(&anEventHeader, MIDIFile)) {
         	showEventHeader(&anEventHeader);
         	Serial.println();
         } else {
      		Serial.println("fail event header.");
      	}
         if (isMetaTag(&anEventHeader)) {
            readMetaEvent(&anEventHeader, &aMetaEvent, MIDIFile);
            showMetaEvent(&aMetaEvent);
            Serial.println();
            switch(aMetaEvent.metaType) {
               case 0x2F : done = true; break;
               case 0x51 :
                  microsPerBeat = 0;
                  MIDIFile.read(&aByte, 1);
                  microsPerBeat = aByte;
                  microsPerBeat = microsPerBeat << 16;
                  MIDIFile.read(&aByte, 1);
                  temp = 0;
                  temp = aByte;
                  temp = temp << 8;
                  microsPerBeat = microsPerBeat | temp;
                  MIDIFile.read(&aByte, 1);
                  temp = 0;
                  temp = aByte;
                  microsPerBeat = microsPerBeat | aByte;
                  Serial.print("micros / beat : ");
                  Serial.println(microsPerBeat);
                  MsPerTick = (microsPerBeat/1000.0)/theMIDIHeader.timeDiv;
                  Serial.print("ms/tic : "); Serial.println(MsPerTick);
                  Serial.println();
               break;
               default :
                  done = !jumpMetaEvent(&aMetaEvent, MIDIFile);
               break;
            }
         } else {
            Serial.println();
         }
         //delay(2000);
      }
      Serial.println(SDFileErr);
      MIDIFile.close();
   }
}


// And this guy.. This mapps a MIDI note to a sound freq. Kinda' glue code for the Piezo
// buzzer.
int MIDI2Freq(int MIDINote) {

	switch(MIDINote) {
		case MIDI_B0	:	return NOTE_B0;
   	case MIDI_C1	:	return NOTE_C1;
   	case MIDI_CS1	:	return NOTE_CS1;
  		case MIDI_D1	:	return NOTE_D1;
   	case MIDI_DS1	:	return NOTE_DS1;
   	case MIDI_E1	:	return NOTE_E1; 
		case MIDI_F1	:	return NOTE_F1; 
		case MIDI_FS1	:	return NOTE_FS1;
		case MIDI_G1	:	return NOTE_G1; 
		case MIDI_GS1	:	return NOTE_GS1;
		case MIDI_A2	:	return NOTE_A2; 
		case MIDI_AS2	:	return NOTE_AS2;
		case MIDI_B2	:	return NOTE_B2;
		case MIDI_C2	:	return NOTE_C2;
		case MIDI_CS2	:	return NOTE_CS2;
		case MIDI_D2	:	return NOTE_D2; 
		case MIDI_DS2	:	return NOTE_DS2;
		case MIDI_E2	:	return NOTE_E2; 
		case MIDI_F2	:	return NOTE_F2; 
		case MIDI_FS2	:	return NOTE_FS2;
		case MIDI_G2	:	return NOTE_G2;
		case MIDI_GS2	:	return NOTE_GS2;
		case MIDI_A3	:	return NOTE_A3;
		case MIDI_AS3	:	return NOTE_AS3;
		case MIDI_B3	:	return NOTE_B3;
		case MIDI_C3	:	return NOTE_C3;
		case MIDI_CS3	:	return NOTE_CS3;
		case MIDI_D3	:	return NOTE_D3;
		case MIDI_DS3	:	return NOTE_DS3;
		case MIDI_E3	:	return NOTE_E3;
		case MIDI_F3	:	return NOTE_F3;
		case MIDI_FS3	:	return NOTE_FS3;
		case MIDI_G3	:	return NOTE_G3;
		case MIDI_GS3	:	return NOTE_GS3;
		case MIDI_A4	:	return NOTE_A4;
		case MIDI_AS4	:	return NOTE_AS4;
		case MIDI_B4	:	return NOTE_B4;
		case MIDI_C4	:	return NOTE_C4;
		case MIDI_CS4	:	return NOTE_CS4;
		case MIDI_D4	:	return NOTE_D4;
		case MIDI_DS4	:	return NOTE_DS4;
		case MIDI_E4	:	return NOTE_E4;
		case MIDI_F4	:	return NOTE_F4;
		case MIDI_FS4	:	return NOTE_FS4;
		case MIDI_G4	:	return NOTE_G4;
		case MIDI_GS4	:	return NOTE_GS4;
		case MIDI_A5	:	return NOTE_A5;
		case MIDI_AS5	:	return NOTE_AS5;
		case MIDI_B5	:	return NOTE_B5;
		case MIDI_C5	:	return NOTE_C5;
		case MIDI_CS5	:	return NOTE_CS5;
		case MIDI_D5	:	return NOTE_D5;
		case MIDI_DS5	:	return NOTE_DS5;
		case MIDI_E5	:	return NOTE_E5;
		case MIDI_F5	:	return NOTE_F5;
		case MIDI_FS5	:	return NOTE_FS5;
		case MIDI_G5	:	return NOTE_G5;
		case MIDI_GS5	:	return NOTE_GS5;
		case MIDI_A6	:	return NOTE_A6;
		case MIDI_AS6	:	return NOTE_AS6;
		case MIDI_B6	:	return NOTE_B6;
		case MIDI_C6	:	return NOTE_C6;
		case MIDI_CS6	:	return NOTE_CS6;
		case MIDI_D6	:	return NOTE_D6;
		case MIDI_DS6	:	return NOTE_DS6;
		case MIDI_E6	:	return NOTE_E6;
		case MIDI_F6	:	return NOTE_F6;
		case MIDI_FS6	:	return NOTE_FS6;
		case MIDI_G6	:	return NOTE_G6;
		case MIDI_GS6	:	return NOTE_GS6;
		case MIDI_A7	:	return NOTE_A7;
		case MIDI_AS7	:	return NOTE_AS7;
		case MIDI_B7	:	return NOTE_B7;
		case MIDI_C7	:	return NOTE_C7;
		case MIDI_CS7	:	return NOTE_CS7;
		case MIDI_D7	:	return NOTE_D7;
		case MIDI_DS7	:	return NOTE_DS7;
		case MIDI_E7	:	return NOTE_E7;
		case MIDI_F7	:	return NOTE_F7;
		case MIDI_FS7	:	return NOTE_FS7;
		case MIDI_G7	:	return NOTE_G7;
		case MIDI_GS7	:	return NOTE_GS7;
		case MIDI_A8	:	return NOTE_A8;
		case MIDI_AS8	:	return NOTE_AS8;
		case MIDI_B8	:	return NOTE_B8;
		case MIDI_C8	:	return NOTE_C8;
		case MIDI_CS8	:	return NOTE_CS8;
		case MIDI_D8	:	return NOTE_D8;
		case MIDI_DS8	:	return NOTE_DS8;
		default			:	return REST;
	}
}
	
