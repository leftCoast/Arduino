#pragma once

struct OperatorPixels {
  struct {
    uint8_t r, g, b;
  } pix0;
  struct {
    uint8_t r, g, b;
  } pix1;
  struct {
    uint8_t r, g, b;
  } paint;
  uint8_t brightness;
};


// fade between pix0 and inverted pix0
// depending on pix1 brightness
void invert_op(struct OperatorPixels &p) {
  uint8_t bright = p.brightness;
  if (bright == 0)
    return;
  uint8_t ir = ~p.pix0.r;
  uint8_t ig = ~p.pix0.g;
  uint8_t ib = ~p.pix0.b;
  if (bright == 255) {
    p.pix0.r = ir;
    p.pix0.g = ig;
    p.pix0.b = ib;
    return;
  }
  p.pix0.r = bblend8(p.pix0.r, ir, bright);
  p.pix0.g = bblend8(p.pix0.g, ig, bright);
  p.pix0.b = bblend8(p.pix0.b, ib, bright);
}


// fade between pix0 and "paint" colour
// depending on pix1 brightness
void alphamask_op(struct OperatorPixels &p) {
  uint8_t bright = p.brightness;
  if (bright == 0)
    return;
  else if (bright == 255) {
    p.pix0.r = p.paint.r;
    p.pix0.g = p.paint.g;
    p.pix0.b = p.paint.b;
    return;
  }
  p.pix0.r = bblend8(p.pix0.r, p.paint.r, bright);
  p.pix0.g = bblend8(p.pix0.g, p.paint.g, bright);
  p.pix0.b = bblend8(p.pix0.b, p.paint.b, bright);
}
