#include "bitmap.h"
#include "resizeBuff.h"
#include "colorObj.h"


// Give back an initialzed but empty bitmap.
bitmap::bitmap(void) {

  mWidth    = 0;
  mHeight   = 0;
  mBitmap   = NULL;
  mHaveMap  = false;
}


// Give back a fully functonal bitmap. If it can get the RAM that is.
bitmap::bitmap(int width,int height) {
  
  mBitmap   = NULL;
  mHaveMap  = false;
  setSize(width,height);   
}


// Create ourselves as a clone of another bitmap. Just like all bitmaps, if it doesn't
// work? We still have all the check methods working. So you can tell if it didn't make it.
bitmap::bitmap(bitmap* aBitmap) {

	byte* theirMap;
	byte* ourMap;
	int	numBytes;
	
	mWidth    = 0;																		// Start off with the standard defaults.
	mHeight   = 0;
	mBitmap   = NULL;
	mHaveMap  = false;
	if (aBitmap) {																		// Sanity, did they even pass in a bitmap?
		if (aBitmap->getHasMap()) {												// Cool, lets see if it has a bitmap?
			if (setSize(aBitmap->getWidth(),aBitmap->getHeight())) {		// Ok, lets see if we can allocate one for us?
				ourMap = (byte*)mBitmap;											// Everything seems fine, point at our map as if its just as array og bytes.
				theirMap = (byte*)aBitmap->getBitmapAddr();					// Point at their map as if its just an array of bytes. Because, really they are.
				numBytes  = mWidth * mHeight * sizeof(RGBpack);				// Calculate the number of bytes in our map buffer.
				for(int i=0;i<numBytes;i++) {										// For every byte..
					ourMap[i] = theirMap[i];										// Copy theirs into ours.
				}
			}
		}
	}
}

		
bitmap::~bitmap(void) { resizeBuff(0,(byte**)&mBitmap); }


// Copy down the bitmap size then allocate the buffer, if possible.
// This can also be used to reset to different sizes, But it causes
// all previous data to be lost when called.
bool bitmap::setSize(int width,int height) {

  int numBytes;
  
  mWidth    = width;
  mHeight   = height;
  numBytes  = mWidth * mHeight * sizeof(RGBpack);
  mHaveMap  = resizeBuff(numBytes,(byte**)&mBitmap);
  return mHaveMap;   
}


// Just dump everything. Probably need the memory elsewhere.
void bitmap::clearMap(void) {

	mWidth    = 0;
	mHeight   = 0;
	resizeBuff(0,(byte**)&mBitmap);
	mHaveMap  = false;
}


// Caller wants to know if we have a map allocated.
bool bitmap::getHasMap(void) { return mHaveMap; }


// These two are for reading back our size..
int bitmap::getWidth(void) { return mWidth; }
int bitmap::getHeight(void) { return mHeight; }


// Set this location to a color. Actually just a pass
// through to the color pack version.
void bitmap::setColor(int x,int y,colorObj* aColor) {

  RGBpack aCPack;

  if (aColor) {
    aCPack = aColor->packColor();
  }
  setColor(x,y,&aCPack);
}


// Set this location to a color. This is the method that actually
// writes to RAM. Hence, everything is sanity checked first.
void  bitmap::setColor(int x,int y,RGBpack* aColor) {

  int index;

  if (x>=0 && x<mWidth && y>=0 && y<mHeight && mHaveMap && aColor) {
    index = y * mWidth + x;
    mBitmap[index] = *aColor;
  }
}


// Grab a color object out of RAM. We need this fast so, no sanity checking.
// Give bad input? Get undefined output.
colorObj bitmap::getColor(int x,int y) {

  colorObj  aColor;
  RGBpack   aCPack;

  aCPack = getColorPack(x,y);
  aColor.setColor(&aCPack);
  return aColor;
}


// Grab a color pack out of RAM. Again, we need this fast so, no sanity checking.
// Give bad input? Get bad output.
RGBpack  bitmap::getColorPack(int x,int y) {

  RGBpack aCPack;
  int     index;
  
  index = y * mWidth + x;
  aCPack = mBitmap[index];
  return aCPack;
}


// This one is kinda' scary. We hand off the address to your bitmap buffer. I mean, I
// don't even like valet parking.
RGBpack* bitmap::getBitmapAddr(void) { return mBitmap; }

