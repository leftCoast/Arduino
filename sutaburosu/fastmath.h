#pragma once

// 8-bit saturating add borrowed from FastLED
__attribute__ ((always_inline)) static inline uint8_t qadd8(uint8_t i, uint8_t j) {
#if defined(__AVR__)
  asm volatile(
      "add %0, %1    \n\t"
      "brcc L_%=     \n\t"
      "ldi %0, 0xFF  \n\t"
      "L_%=: "
      : "+a" (i)
      : "a"  (j)
  );
  return i;
#elif defined(__arm__)
  asm volatile( "uqadd8 %0, %0, %1" : "+r" (i) : "r" (j));
  return i;
#else
  unsigned int t = i + j;
  if (t > 255) t = 255;
  return t;
#endif
}


// from ben-xo's FastLED fork
// https://github.com/FastLED/FastLED/pull/1288/commits/f705f549d83d0d97530581e6470eed0490cc942a

/// blend a variable proproportion(0-255) of one byte to another
/// @param a - the starting byte value
/// @param b - the byte value to blend toward
/// @param amountOfB - the proportion (0-255) of b to blend
/// @returns a byte value between a and b, inclusive
__attribute__ ((always_inline)) static inline uint8_t bblend8( uint8_t a, uint8_t b, uint8_t amountOfB) {

  uint8_t amountOfA = 255 - amountOfB;

#if ! defined(__AVR__)
  uint16_t partial;
  partial = 256 * a;
  partial += b;
  
  partial -= a * amountOfB;
  partial += b * amountOfB;

  uint8_t result = partial / 256;

  return result;

#else
  // the algorithm was previously:
  // result = A*(255-amountOfB) + A + B*(amountOfB) + B

  // however, we can rearrange that to:
  // result = 256*A + B - A*amountOfB + B*amountOfB

  // 1 or 2 cycles depending on how the compiler optimises
  uint16_t partial;
  partial = (a << 8) + b;

  // 7 cycles
  asm volatile (
      "  mul %[a], %[amountOfB]        \n\t"
      "  sub %A[partial], r0           \n\t"
      "  sbc %B[partial], r1           \n\t"
      "  mul %[b], %[amountOfB]        \n\t"
      "  add %A[partial], r0           \n\t"
      "  adc %B[partial], r1           \n\t"
      "  clr __zero_reg__              \n\t"
      : [partial] "+r" (partial)
      : [amountOfB] "r" (amountOfB),
        [a] "r" (a),
        [b] "r" (b)
      : "r0", "r1"
  );
  uint8_t result = partial >> 8;
  return result;
#endif

}
