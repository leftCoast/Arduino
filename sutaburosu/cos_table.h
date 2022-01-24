#pragma once
#include "Array.h"

//////////////////////////////////////////////////////////////////////
// Create a lookup table at compile time and put it in Flash
// https://trina.si/2017/07/25/cosine-lookup-table-c-constexpr/

// the compiler will barf when >900 templates are nested
constexpr auto cos_entries_log2 = 9;
constexpr auto cos_entries = 1 << cos_entries_log2;

template<typename T>
constexpr T cos_table_elem (int i) {
    return {};
}

template<>
constexpr int16_t cos_table_elem (int i) {
  // store only the first quadrant
  return round(cos(static_cast <double>(i) * 3.14159265 / 2. / cos_entries) * 32767);
}

template<typename T, int... N>
struct cos_table_expand{};

template<typename T, int... N>
struct cos_table_expand<T, 1, N...> {
  static constexpr Array<T, sizeof...(N) + 1> values =
    {{ cos_table_elem<T>(0), N... }};
};

template<typename T, int L, int... N> 
struct cos_table_expand<T, L, N...>:
  cos_table_expand<T, L-1, cos_table_elem<T>(L-1), N...> {};

template<typename T, int... N>
constexpr Array<T, sizeof...(N) + 1>
  cos_table_expand<T, 1, N...>::values;
 
PROGMEM const Array<int16_t, cos_entries> cos_table =
  cos_table_expand<int16_t, cos_entries>::values;

//////////////////////////////////////////////////////////////////////
// functions to read from the lookup table

const int16_t cos16s(uint16_t angle) {
  // extract top two bits as the quadrant
  uint8_t quadrant = angle >> 14;
  // constrain the remainder to the range of the lookup table
  angle = (angle & 0x3fff) >> (14 - cos_entries_log2);
  // odd quadrants run backwards through the table
  if (quadrant & 1)
    angle = cos_entries - 1 - angle;
  int16_t val = pgm_read_word_near(&cos_table[angle]);
  switch (quadrant) {
    case 1:  return -val; break;
    case 2:  return -val; break;
    default: return val;
  }
}

inline const int8_t cos8s(uint16_t angle) {
  // extract top two bits as the quadrant
  uint8_t quadrant = angle >> 14;
  // constrain the remainder to the range of the lookup table
  angle = (angle & 0x3fff) >> (14 - cos_entries_log2);
  // odd quadrants run backwards through the table
  if (quadrant & 1)
    angle = cos_entries - 1 - angle;
  int8_t *val_ptr = (int8_t *) &cos_table[angle];
  int8_t val = pgm_read_byte_near(val_ptr + 1); // depends on little-endianness
  switch (quadrant) {
    case 1:  return -val; break;
    case 2:  return -val; break;
    default: return val;
  }
}

inline const uint16_t cos16u(const uint16_t angle) {
  int16_t val = cos16s(angle);
  return 32767 + val;
}

inline const uint8_t cos8u(const uint16_t angle) {
  int8_t val = cos8s(angle);
  return 127 + val;
}


inline const int16_t sin16s(uint16_t angle) {
  return cos16s(angle + 0xc000);
}
inline const int8_t sin8s(uint16_t angle) {
  return cos8s(angle + 0xc000);
}
inline const uint16_t sin16u(uint16_t angle) {
  return cos16u(angle + 0xc000);
}
inline const uint8_t sin8u(uint16_t angle) {
  return cos8u(angle + 0xc000);
}
