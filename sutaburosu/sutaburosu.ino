#include <Adafruit_ILI9341.h>
#include "motif.h"
#include "rgb_plasma.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9);
Plasma plasma;
PROGMEM RLE565 * const motifs[] = {&smmotif, &bigmotif};
RLE565 *this_motif = motifs[0];

// To support different hardware or different plot effects
// create your own version of this function which takes the
// same parameters.
// It will be called once for each row of decompressed image data.
void draw_ILI9341(int16_t x, int16_t y, uint16_t *buffer, uint16_t width) {
  tft.drawRGBBitmap(x, y, buffer, width, 1);
}

void setup(void) {
  Serial.begin(115200);
  tft.begin();

  // draw the big motif centred on the screen
  int16_t x = ILI9341_TFTWIDTH / 2 - bigmotif.width() / 2;
  int16_t y = ILI9341_TFTHEIGHT / 2 - bigmotif.height() / 2;
  bigmotif.plot(draw_ILI9341, x, y);
  delay(1000);

  // draw lots of small motifs
  for (int16_t y = smmotif.height() / -3; y < ILI9341_TFTHEIGHT; y += smmotif.height())
    for (int16_t x = smmotif.width() / -3; x < ILI9341_TFTWIDTH; x += smmotif.width())
      smmotif.plot(draw_ILI9341, x, y);
}


void loop() {
  static int16_t last_ymin = 0;
  static int16_t last_ymax = ILI9341_TFTHEIGHT;
  int16_t h = this_motif->height();
  int16_t w = this_motif->width();
  int16_t x, y;

  // delay(1000);
  plasma.reset();

  // either centre or randomise the position of the motif
  if (rand() & 0x1) {
    x = (rand() % (ILI9341_TFTWIDTH - w / 3)) - w / 3;
    y = (rand() % (ILI9341_TFTHEIGHT - h / 3 )) - h / 3;
  } else {
    x = ILI9341_TFTWIDTH / 2 - w / 2;
    y = ILI9341_TFTHEIGHT / 2 - h / 2;
  }

  // calc the smallest span of rows which covers both
  // the previous motif position and the current one.
  int16_t ymin = min(ymin, last_ymin);
  int16_t ymax = min(y + h, ILI9341_TFTHEIGHT);
  ymin = max(ymin, 0);
  ymax = max(ymax, last_ymax);
  
  // fill the "empty" area above the motif
  if (ymin < y) {
    // overwrite the extent of the previous motif
    plasma.ystepn(ymin);
    yfill_plasma(ymin, y, ILI9341_TFTWIDTH);
  } else {
    // just skip to start of this motif
    plasma.ystepn(y);
  }

  // mix the motif with the plasma
  this_motif->plot(composite_ILI9341, x, y);

  // fill the "empty" area below the motif
  if (y + h < ymax) {
    // is there a gap before the area to be filled?
    if (last_ymin > y + h) {
      // skip the gap, then fill
      delay(2000);  // 😮 Look! There are two of them!
      plasma.ystepn(last_ymin - y - h);
      yfill_plasma(last_ymin, last_ymax, ILI9341_TFTWIDTH);
    } else {
      // no gap
      yfill_plasma(y + h, last_ymax, ILI9341_TFTWIDTH);
    }
  }

  // remember which bits need to be cleared next time
  last_ymin = max(y, 0);
  last_ymax = min(y + h, ILI9341_TFTHEIGHT);

  if (rand() & 1) {
    // switch to a random motif from the list
    uint8_t motif_num = rand() % (sizeof(motifs) / sizeof(motifs[0]));
    this_motif = pgm_read_ptr_near(&motifs[motif_num]);

  } else if (rand() & 1) {
    last_ymin = 0, last_ymax = ILI9341_TFTHEIGHT;
    plasma.randomise();

    if (rand() & 1)
      plasma.p.paint.r = plasma.p.paint.g = plasma.p.paint.b = 0;
    else
      plasma.p.paint.r = plasma.p.paint.g = plasma.p.paint.b = 255;

    if (rand() & 1)
      plasma.compositeoperator = alphamask_op;
    else
      plasma.compositeoperator = invert_op;
  }
}

void yfill_plasma(int16_t y0, int16_t y1, uint16_t width) {
  uint16_t buffer[width];
  for (auto y = y0; y < y1; y++) {
    memset(buffer, 0, width * 2);
    composite_ILI9341(0, y, buffer, width);
  }
}

// here's another plot function for the RLE decompressor
// this one composites the image with RGB plasma
void composite_ILI9341(int16_t x, int16_t y, uint16_t *buffer, uint16_t width) {
  if (y < 0) return;
  plasma.ystep();
  if (x > 0) {
    // fill plasma to the left of the RLE image
    int16_t left = x < ILI9341_TFTWIDTH ? x : ILI9341_TFTWIDTH;
    uint16_t tmp[left];
    memset(tmp, 0, left * 2);
    plasma.composite_row(left, tmp);
    tft.drawRGBBitmap(x - left, y, tmp, left, 1);
  } else if (x < 0) {
    buffer -= x;
    width += x;
    x = 0;
  }

  // mix plasma with image
  plasma.composite_row(width, buffer);
  tft.drawRGBBitmap(x, y, buffer, width, 1);

  int16_t right = ILI9341_TFTWIDTH - x - width;
  if (right > 0) {
    // fill plasma to the right of the image
    right = right < ILI9341_TFTWIDTH ? right : ILI9341_TFTWIDTH;
    uint16_t tmp[right];
    memset(tmp, 0, right * 2);
    plasma.composite_row(right, tmp);
    tft.drawRGBBitmap(ILI9341_TFTWIDTH - right, y, tmp, right, 1);
  }
}

uint16_t freemem() {
  extern int __heap_start; //start of the heap
  extern int *__brkval; //highest point of heap
  static uint16_t freemin = -1;
  uint16_t free;
  free = (int) &free - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  if (free < freemin) {
    freemin = free;
    Serial.print(free);
    Serial.println(" free");
  }
  return free;
}
