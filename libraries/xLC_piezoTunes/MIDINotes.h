#ifndef MIDINotes_h
#define MIDINotes_h


#include <SD.h>



// ***************************************************************************************
// Meta tags and what we know about them. There are probably more out there.
// ***************************************************************************************


#define META_SEQUENCE_NUM  0x00
// FF 00 02 SequenceNumber
//
// Doc's say..
// This optional event, which must occur at the beginning of a track, before any nonzero
// delta-times, and before any transmittable MIDI events, specifies the number of a
// sequence. In a format 2 MIDI File, it is used to identify each "pattern" so that a
// "song" sequence using the Cue message can refer to the patterns. If the ID numbers are
// omitted, the sequences' locations in order in the file are used as defaults. In a
// format 0 or 1 MIDI File, which only contain one sequence, this number should be
// contained in the first (or only) track. If transfer of several multitrack sequences is
// required, this must be done as a group of format 1 files, each with a different
// sequence number.


#define META_TEXT          0x01
// FF 01 lenTextEvent
//
// Doc's say..
// Any amount of text describing anything. It is a good idea to put a text event right at
// the beginning of a track, with the name of the track, a description of its intended
// orchestration, and any other information which the user wants to put there. Text events
// may also occur at other times in a track, to be used as lyrics, or descriptions of cue
// points. The text in this event should be printable ASCII characters for maximum
// interchange. However, other character codes using the high-order bit may be used for
// interchange of files between different programs on the same computer which supports an
// extended character set. Programs on a computer which does not support non-ASCII
// characters should ignore those characters.
//
// Meta-event types 01 through 0F are reserved for various types of text events, each of
// which meets the specification of text events (above) but is used for a different
// purpose:


#define COPYWRITE          0x02
// FF 02 lenCopyrightNotice
//
// Doc's say..
// Contains a copyright notice as printable ASCII text. The notice should contain the
// characters (C), the year of the copyright, and the owner of the copyright. If several
// pieces of music are in the same MIDI File, all of the copyright notices should be
// placed together in this event so that it will be at the beginning of the file. This
// event should be the first event in the track chunk, at time 0.

 
#define TRACK_NAME         0x03
// FF 03 lenSequenceTrackName
//
// Doc's say..
// If in a format 0 track, or the first track in a format 1 file, the name of the
// sequence. Otherwise, the name of the track.


#define INST_NAME          0x04
// FF 04 lenInstrumentName
//
// Doc's say..
// A description of the type of instrumentation to be used in that track. May be used with
// the MIDI Prefix meta-event to specify which MIDI channel the description applies to, or
// the channel may be specified as text in the event itself.


#define  LYRIC             0x05
// FF 05 lenLyric
//
// Doc's say..
// A lyric to be sung. Generally, each syllable will be a separate lyric event which
// begins at the event's time.


#define MRKER              0x06
// FF 06 lenMarker
//
// Doc's say..
// Normally in a format 0 track, or the first track in a format 1 file. The name of that
// point in the sequence, such as a rehearsal letter or section name ("First Verse", etc.)


#define CUE_POINT          0x07
// FF 07 lenCuePoint
//
// Doc's say..
// A description of something happening on a film or video screen or stage at that point
// in the musical score ("Car crashes into house", "curtain opens", "she slaps his face",
// etc.)


#define  CHANNEL_PREFIX    0X20
// FF 20 01 cc MIDIChannelPrefix. One Byte meaning.. who knows?
//
// Doc's say..
// The MIDI channel (0-15) contained in this event may be used to associate a MIDI channel
// with all events which follow, including System exclusive and meta-events. This channel
// is "effective" until the next normal MIDI event (which contains a channel) or the next
// MIDI Channel Prefix meta-event. If MIDI channels refer to "tracks", this message may be
// put into a format 0 file, keeping their non-MIDI data associated with a track. This
// capability is also present in Yamaha's ESEQ file format.


#define  END_OF_TRACK      0x2F
// FF 2F 00 End of Track. This event is not optional.
//
// Doc's say..
// This event is not optional. It is included so that an exact ending point may be
// specified for the track, so that an exact length is defined, which is necessary for
// tracks which are looped or concatenated.


#define  SET_TEMPO         0x51
// FF 51 03 tttttt Set Tempo (in microseconds per MIDI quarter-note. 24 bit number)
// My ass! I'm pretty sure is milliseconds. But, we'll see..
//
// Doc's say..
// This event indicates a tempo change. Another way of putting "microseconds per
// quarter-note" is "24ths of a microsecond per MIDI clock". Representing tempos as time
// per beat instead of beat per time allows absolutely exact long-term synchronisation
// with a time-based sync protocol such as SMPTE time code or MIDI time code. The amount
// of accuracy provided by this tempo resolution allows a four-minute piece at 120 beats
// per minute to be accurate within 500 usec at the end of the piece. Ideally, these
//events should only occur where MIDI clocks would be located -- this convention is
// intended to guarantee, or at least increase the likelihood, of compatibility with other
// synchronisation devices so that a time signature/tempo map stored in this format may
// easily be transferred to another device.


#define  SMPTE_OFFSET      0x54
// FF 54 05 hr mn se fr ff SMPTE Offset.
//
// Doc's say..
// This event, if present, designates the SMPTE time at which the track chunk is supposed
// to start. It should be present at the beginning of the track, that is, before any
// nonzero delta-times, and before any transmittable MIDI events. the hour must be
// encoded with the SMPTE format, just as it is in MIDI Time Code. In a format 1 file, the
// SMPTE Offset must be stored with the tempo map, and has no meaning in any of the other
// tracks. The ff field contains fractional frames, in 100ths of a frame, even in
// SMPTE-based tracks which specify a different frame subdivision for delta-times.

 
#define TIME_SIG           0x58
// FF 58 04 nn dd cc bb Time Signature.
//
// Doc's say..
// The time signature is expressed as four numbers. nn and dd represent the numerator and
// denominator of the time signature as it would be notated. The denominator is a negative
// power of two: 2 represents a quarter-note, 3 represents an eighth-note, etc. The cc
// parameter expresses the number of MIDI clocks in a metronome click. The bb parameter
// expresses the number of notated 32nd-notes in a MIDI quarter-note (24 MIDI clocks).
// This was added because there are already multiple programs which allow a user to
// specify that what MIDI thinks of as a quarter-note (24 clocks) is to be notated as, or
// related to in terms of, something else.
//
// Therefore, the complete event for 6/8 time, where the metronome clicks every three
// eighth-notes, but there are 24 clocks per quarter-note, 72 to the bar, would be
// 
// (in hex): FF 58 04 06 03 24 08
//
// That is, 6/8 time (8 is 2 to the 3rd power, so this is 06 03), 36 MIDI clocks per
// dotted-quarter (24 hex!), and eight notated 32nd-notes per quarter-note.


#define KEY_SIG				0x59
// FF 59 02 sf mi Key Signature..
//
// Doc's say..
// sf = -7: 7 flats
// sf = -1: 1 flat
// sf = 0: key of C
// sf = 1: 1 sharp
// sf = 7: 7 sharps
// mi = 0: major key
// mi = 1: minor key
//
// I don't even pretend to understand all of these. Right now I'm woking with SET_TEMPO
// and END_OF_TRACK. The rest I've been skipping.


// ***************************************************************************************
// MIDI note numbers. comes from the docs.
// ***************************************************************************************


enum MIDINums {
   MIDI_B0 = 11,
   MIDI_C1, 
   MIDI_CS1,
   MIDI_D1,  
   MIDI_DS1, 
   MIDI_E1,  
   MIDI_F1,  
   MIDI_FS1,
   MIDI_G1,  
   MIDI_GS1, 
   MIDI_A2,  
   MIDI_AS2, 
   MIDI_B2,  
   MIDI_C2,  
   MIDI_CS2, 
   MIDI_D2,  
   MIDI_DS2, 
   MIDI_E2,  
   MIDI_F2,  
   MIDI_FS2, 
   MIDI_G2,  
   MIDI_GS2, 
   MIDI_A3,  
   MIDI_AS3, 
   MIDI_B3,  
   MIDI_C3,  
   MIDI_CS3, 
   MIDI_D3,  
   MIDI_DS3, 
   MIDI_E3,  
   MIDI_F3,
   MIDI_FS3,
   MIDI_G3,
   MIDI_GS3,
   MIDI_A4,
   MIDI_AS4,
   MIDI_B4,
   MIDI_C4,
   MIDI_CS4,
   MIDI_D4,
   MIDI_DS4,
   MIDI_E4,
   MIDI_F4,
   MIDI_FS4,
   MIDI_G4,
   MIDI_GS4,
   MIDI_A5,
   MIDI_AS5,
   MIDI_B5,
   MIDI_C5,
   MIDI_CS5,
   MIDI_D5,
   MIDI_DS5,
   MIDI_E5,
   MIDI_F5,
   MIDI_FS5,
   MIDI_G5,
   MIDI_GS5,
   MIDI_A6,
   MIDI_AS6,
   MIDI_B6,
   MIDI_C6,
   MIDI_CS6,
   MIDI_D6,
   MIDI_DS6,
   MIDI_E6,
   MIDI_F6,
   MIDI_FS6,
   MIDI_G6,
   MIDI_GS6,
   MIDI_A7,
   MIDI_AS7,
   MIDI_B7,
   MIDI_C7,
   MIDI_CS7,
   MIDI_D7,
   MIDI_DS7,
   MIDI_E7,
   MIDI_F7,
   MIDI_FS7,
   MIDI_G7,
   MIDI_GS7,
   MIDI_A8,
   MIDI_AS8,
   MIDI_B8,
   MIDI_C8,
   MIDI_CS8,
   MIDI_D8,
   MIDI_DS8
};



// ***************************************************************************************
// Some common MIDI event types
// ***************************************************************************************

#define MIDI_KEY_DN	0x9
#define MIDI_KEY_UP	0x8


// ***************************************************************************************
// Different MIDI section headers. Also from the docs.
// ***************************************************************************************


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


struct metaEvent {

   byte     metaType;
   uint32_t numBytes;
   uint32_t location;
};



// ***************************************************************************************
// MIDI file decoding functions for busting up MIDI files into their building blocks.
// ***************************************************************************************


extern bool readMIDIHeader(MIDIHeader* theHeader, File MIDIFile);
extern bool readEventHeader(eventHeader* header, File MIDIFile);
extern bool readTrackHeader(trackHeader* theHeader, File MIDIFile);
extern void readMetaEvent(eventHeader* theHeader, metaEvent* theEvent, File MIDIFile);
extern bool jumpMetaEvent(metaEvent* theEvent, File MIDIFile);

extern bool readAndShowMetaData(metaEvent* theEvent, File MIDIFile);

extern void showMIDIHeader(MIDIHeader* theMIDIHeader);
extern void showTrackHeader(trackHeader* theHeader);
extern void showEventHeader(eventHeader* theHeader);

extern bool isMetaTag(eventHeader* theHeader);

extern int MIDI2Freq(int MIDINote);

extern void decodeFile(const char* filePath);

#endif