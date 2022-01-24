#include "cos_table.h"
#include "fastmath.h"
#include "operators.h"

struct Plasma {
  using CompositeOperator = void (*) (struct OperatorPixels *);
  uint16_t accum[9];
  uint8_t delta[9];
  uint8_t dither;
  uint16_t wyhash16_x;
  uint16_t seed = 0x533d;
  struct OperatorPixels p;
  CompositeOperator compositeoperator = alphamask_op;

  // https://github.com/littleli/go-wyhash16/blob/master/wyhash16.go
  uint16_t hash16(uint16_t input, uint16_t key) {
    uint32_t hash = (uint32_t) input * key;
    return ((hash >> 16) ^ hash) & 0xFFFF;
  }


  uint16_t wyhash16() {
    wyhash16_x += 0xfc15;
    return hash16(wyhash16_x, 0x2ab);
  }


  void reset() {
    dither = 0x2d;
    wyhash16_x = seed;
    for (auto i = 0; i < 9; i++)
      delta[i] = wyhash16(),  accum[i] = wyhash16();
  }


  void randomise() {
    seed ^= rand();
  }


  void ystep() {
    dither ^= 0xff;
    for (auto i = 0; i < 3; i++) {
      accum[i] += delta[i];
      accum[i + 3] = accum[i];
      accum[i + 6] += delta[i + 6];
    }
  }


  inline void ystepn(int16_t n) {
    if (n <= 0)
      return;

    if (n & 1)
      dither ^= 0xff;

    for (auto i = 0; i < 3; i++) {
      accum[i] += delta[i] * n;
      accum[i + 3] = accum[i];
      accum[i + 6] += delta[i + 6] * n;
    }
  }


  void xstep() {
    dither = (dither >> 4) | (dither << 4);
    for (auto i = 3; i < 6; i++)
      accum[i] += delta[i];
  }


  void xstepn(int16_t n) {
    if (n <= 0)
      return;

    if (n & 1)
      dither = (dither >> 4) | (dither << 4);
    for (auto i = 3; i < 6; i++)
      accum[i] += delta[i] * n;
  }


  void composite_row(uint16_t width, uint16_t *buffer) {
    uint16_t *mixbuf = buffer;
    // ystep();
    for (auto x = 0; x < width; x++) {
      xstep();

      // generate the "plasma" background for this pixel
      p.pix0.r = cos8u(accum[3] + cos16s(accum[6]));
      p.pix0.g = cos8u(accum[4] + cos16s(accum[7]));
      p.pix0.b = cos8u(accum[5] + cos16s(accum[8]));

      // read a buffer pixel and extract RGB565 components
      // into the high bits of RGB888
      uint16_t src = *mixbuf;
      p.pix1.r = (src & 0xf800) >> 8;
      p.pix1.g = (src & 0x07e0) >> 3;
      p.pix1.b = (src & 0x1f) << 3;

      // extend the limited RGB565 range to 0-255
      p.pix1.r |= p.pix1.r >> 5;
      p.pix1.g |= p.pix1.g >> 6;
      p.pix1.b |= p.pix1.b >> 5;

      // green has double the weight in this simple brightness
      uint16_t rawbright = 64 * (p.pix1.r + 2 * p.pix1.g + p.pix1.b);
      p.brightness = rawbright >> 8;

      // call the operator to combine the pair of pixels
      // and modify the first one
      compositeoperator(&p);

      // apply dithering and truncate to RGB565
      uint8_t dstep = dither & 2;
      uint16_t out;
      out = ((qadd8(dstep << 1, p.pix0.r) & 0xf8) << 8);
      out |= ((qadd8(dstep << 0, p.pix0.g) & 0xfc) << 3);
      out |= qadd8(dstep << 1, p.pix0.b) >> 3;
      *mixbuf++ = out;
    }
  }
};
