#ifndef bmpImage_h
#define bmpImage_h


#include <baseImage.h>


class bmpImage :	public baseImage {

	public:
						bmpImage(void);
	virtual			~bmpImage(void);
	
	virtual		bool		openImage(char* inPath=NULL);
	virtual		bool		saveImage(char* inPath=NULL);
	
	virtual		colorObj	getPixel(int x,int y);
	virtual		RGBpack	getPixel(int x,int y);
	virtual		void		getRow(int row,RGBpack* RGBPackArray,int xMin=0,xMax=0);
	
	virtual		void		setPixel(int x,int y,colorObj* aColor);
	virtual		void		setPixel(int x,int y,RGBpack* anRGBPack);
	virtual		void		setRow(int row,RGBpack* RGBPackArray,int xMin=0,xMax=0);

	protected:
	
	virtual		bool		readImage(void);
	virtual		bool		openWorkingFile(void);
	    			uint16_t	read16(File f);
    				uint32_t	read32(File f);
	
					bool		rightSideUp;
					int     	imageOffset;
					int		imageDepth;
					int		pixBytes;
					int		bytesPerRow;
};

#endif