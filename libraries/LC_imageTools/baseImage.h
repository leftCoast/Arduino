#ifndef baseImage_h
#define baseImage_h

#include <docFile.h>
#include <colorObj.h>

// We have to start somewhere. This'll be better than zero.
#define DEF_WIDTH		240
#define DEF_HEIGHT	320

	
class baseImage :	public docFile {

	public:
					baseImage(void);
	virtual		~baseImage(void);
	
	virtual		bool		newImage(int inWidth,int inHeight);
	
	virtual		colorObj	getPixel(int x,int y);
	virtual		void		getRow(int row,RGBpack* RGBArray,int numPix,int xStart,bool fromTemp=false);
	virtual		void		getRow(int row,RGBpack* RGBArray);
	
	virtual		void		setPixel(int x,int y,colorObj* aColor);
	virtual		void		setRow(int row,RGBpack* RGBPackArray,int numPix,int xStart);
	
	protected:
	
	virtual		RGBpack	getPixel(int x,int y,File* imageFile);
	virtual		void		setPixel(int x,int y,RGBpack* anRGBPack,File* imageFile);
					bool		checkXYLImits(int* x, int* y);
					bool		checkRowXminXmax(int* row,int* xMin,int* xMax);
					
					int		width;
					int		height;
};


#endif
