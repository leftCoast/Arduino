# timestamp32bits
Generate 32 bits timestamps using UNIX format.

A library for calculating UNIX-like timestamps for dates AFTER January 1, 1970 or whichever year is specified as EPOCH.

### API

##### timestamp32bits(uint16_t epoch_year)
* epoch_year: four digit year that should be used as the EPOCH for a custom timestamp.

##### unsigned long timestamp(char year, char month, char day, char hour, char min, char seg)
* year: number ranging from 0 to 99.
* month: number ranging from 1 to 12.
* day: number ranging from 1 to 31.
* hour: number in 24 hour format ranging from 0 to 23.
* min: number ranging from 0 to 59.
* seg: number ranging from 0 to 59.
