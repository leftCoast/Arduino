#include "timestamp32bits.h"

timestamp32bits::timestamp32bits(void){
  timestamp32bits(1970);
}

timestamp32bits::timestamp32bits(uint16_t epoch_year){
  _offset = 2000 - epoch_year;
}

unsigned long timestamp32bits::timestamp(char year, char month, char day, char hour, char min, char seg){
  unsigned long _timestamp=(year+_offset)*YEAR;
  _timestamp += (day + _days[month - 1] + ((year + _offset + 2) / 4) - 1) * DAY;
	_timestamp += hour * HOUR;
	_timestamp += min * MINUTE;
	_timestamp += seg;
	return ((year % 4 == 0) && (month < 3)) ? _timestamp - DAY : _timestamp;
}
