#ifndef baseImage_h
#define baseImage_h

#include <docFileObj.h>
#include <colorObj.h>

	
class baseImage :	public docFileObj {

	public:
					baseImage(const char* filePath);
	virtual		~baseImage(void);
	
					int		getWidth(void);
					int		getHeight(void);
	virtual		colorObj	getPixel(int x,int y);
	virtual		RGBpack	getRawPixel(int x,int y)=0;
	virtual		bool		setPixel(int x,int y,colorObj* aColor);
	virtual		void		setRawPixel(int x,int y,RGBpack* anRGBPack)=0;
	virtual		bool		getRow(int x,int y,int numPix,RGBpack* RGBArray);
	virtual		bool		setRow(int x,int y,int numPix,RGBpack* RGBArray);
					bool		checkXYLmits(int x, int y);
					
	protected:				
					int		width;
					int		height;
};

#endif
