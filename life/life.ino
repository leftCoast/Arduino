#include <adafruit_1947_Obj.h>
#include <screen.h>
#include <timeObj.h>

#define GRID_X  64 //240
#define GRID_Y  127 //320

byte  grid[GRID_X / 8][GRID_Y];
byte  tempGrid[GRID_X / 8][GRID_Y];
long  bytes = GRID_X / 8 * GRID_Y;
timeObj  frameTimer(1000);
int    paint;

void  setGrid(byte grid[][GRID_Y], int x, int y, bool value);

void setup() {

  Serial.begin(57600);
  Serial.println("Hello?");
  if (!initScreen(ADAFRUIT_1947, ADA_1947_SHIELD_CS, PORTRAIT)) {
    Serial.println(F("Screen failed, halting program."));
    while (true);                 //  kill the program.
  }
  screen->fillScreen(&black);
  clearGrid();
  randomSeed(analogRead(A0));
  randomFill(10000);               // Put something out there to watch.
   paint = 0;
  setGrid(grid, 13, 12, true); // Flyer
  setGrid(grid, 14, 13, true);
  setGrid(grid, 15, 13, true);
  setGrid(grid, 13, 14, true);
  setGrid(grid, 14, 14, true);

  setGrid(grid, 34, 44, true); // Med spaceship
  setGrid(grid, 35, 44, true);
  setGrid(grid, 36, 44, true);
  setGrid(grid, 37, 44, true);
  setGrid(grid, 38, 44, true);
  setGrid(grid, 33, 45, true);
  setGrid(grid, 38, 45, true);
  setGrid(grid, 38, 46, true);
  setGrid(grid, 33, 47, true);
  setGrid(grid, 37, 47, true);
  setGrid(grid, 35, 48, true);

  setGrid(grid, 63, 70, true); // Blinker. Toad?
  setGrid(grid, 64, 70, true);
  setGrid(grid, 65, 70, true);
  setGrid(grid, 64, 71, true);
  setGrid(grid, 65, 71, true);
  setGrid(grid, 66, 71, true);

  paintGrid(true);
}


void randomFill(int numDots) {

  int x;
  int y;

  for (int i = 0; i < numDots; i++) {
    x = random(0, GRID_X);
    y = random(0, GRID_Y);
    setGrid(grid, x, y, true);
  }
}


void clearGrid(void) {

  byte* gPtr;

  gPtr = (byte*) &grid[0][0];
  for (int i = 0; i < GRID_X / 8 * GRID_Y; i++) {
    gPtr[i] = 0;
  }
}


void gridToTemp(void) {

  void* gPtr;
  void* tPtr;

  gPtr = (void*) &grid[0][0];
  tPtr = (void*) &tempGrid[0][0];
  memcpy(tPtr, gPtr, bytes);
}


void tempToGrid(void) {

  byte* gPtr;
  byte* tPtr;

  gPtr = (byte*) &grid[0][0];
  tPtr = (byte*) &tempGrid[0][0];
  memcpy(gPtr, tPtr, bytes);
}


void  setGrid(byte grid[][GRID_Y], int x, int y, bool value) {

  int   xIndex;
  byte  xBit;

  if (x < 0 || x >= GRID_X) return;
  if (y < 0 || y >= GRID_Y) return;
  xIndex = x >> 3;
  xBit = x - (xIndex << 3);
  if (value) {
    bitSet(grid[xIndex][y], xBit);
  } else {
    bitClear(grid[xIndex][y], xBit);
  }
}


bool getGrid(byte grid[][GRID_Y], int x, int y) {

  int   xIndex;
  byte  xBit;
  bool  result;

  result = false;
  if (x < 0 || x >= GRID_X) return result;
  if (y < 0 || y >= GRID_Y) return result;
  xIndex = x >> 3;
  xBit = x - (xIndex << 3);
  result = (bool) bitRead(grid[xIndex][y], xBit);
  return result;
}



// Any live cell with two or three live neighbours survives.
// Any dead cell with three live neighbours becomes a live cell.
// All other live cells die in the next generation.
// Similarly, all other dead cells stay dead.
bool updatePoint(int x, int y) {

  byte  numLiveCells;
  int   xMinus;
  int   xPlus;

  numLiveCells = 0;
  xMinus = x - 1;
  xPlus = x + 1;
  y--;
  numLiveCells = numLiveCells + (int)getGrid(grid, xMinus, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, x, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, xPlus, y);
  y++;
  numLiveCells = numLiveCells + (int)getGrid(grid, xMinus, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, xPlus, y);
  y++;
  numLiveCells = numLiveCells + (int)getGrid(grid, xMinus, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, x, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, xPlus, y);
  y--;
  if (getGrid(grid, x, y)) {
    if (numLiveCells == 2 || numLiveCells == 3) {
      return true;
    } else {
      return false;
    }
  } else {
    if (numLiveCells == 3) {
      return true;
    }
  }
  return false;
}


void stepTime(void) {

  bool  result;

  gridToTemp();
  for (int y = 0; y < GRID_Y; y++) {
    for (int x = 0; x < GRID_X; x++) {
      result = updatePoint(x, y);
      setGrid(tempGrid, x, y, result);
    }
  }
}


void paintGrid(bool fullGrid) {

  bool  tempGridVal;

  if (fullGrid) {
    for (int y = 0; y < GRID_Y; y++) {
      for (int x = 0; x < GRID_X; x++) {
        if (getGrid(grid, x, y)) {
          screen->drawPixel(x, y, &white);
        } else {
          screen->drawPixel(x, y, &black);
        }
      }
    }
  } else {
    for (int y = 0; y < GRID_Y; y++) {
      for (int x = 0; x < GRID_X; x++) {
        tempGridVal = getGrid(tempGrid, x, y);
        if (getGrid(grid, x, y) != tempGridVal) {
          if (tempGridVal) {
            screen->drawPixel(x, y, &white);
          } else {
            screen->drawPixel(x, y, &black);
          }
        }
      }
    }
    tempToGrid();
  }
}


void loop() {

   stepTime();
   paintGrid(false);
   paint++;
   if (frameTimer.ding()) {
      Serial.print(paint);
      Serial.println(" frames per sec.");
      paint = 0;
      frameTimer.start();
   }
}
