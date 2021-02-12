#ifndef baseImage_h
#define baseImage_h

#include <colorObj.h>


class baseImage {

	public:
					baseImage(char* inPath=NULL);
	virtual		~baseImage(void);
	
	virtual		bool		newImage(int inWidth,int inHeight);
	virtual		bool		openImage(char* inPath=NULL);
	virtual		bool		saveImage(char* inPath=NULL);
	
	virtual		colorObj	getPixel(int x,int y);
	virtual		RGBpack	getPixel(int x,int y);
	virtual		void		getRow(int row,RGBpack* RGBPackArray,int xMin=0,xMax=0);
	
	virtual		void		setPixel(int x,int y,colorObj* aColor);
	virtual		void		setPixel(int x,int y,RGBpack* anRGBPack);
	virtual		void		setRow(int row,RGBpack* RGBPackArray,int xMin=0,xMax=0);
	
	protected:
	
	virtual		bool		readImage(void) = 0;
	virtual		bool		openWorkingFile(void);
					
					char*		imagePath;
					char*		tempPath;
					bool		haveInfo;
					int		width;
					int		height;
};


#endif