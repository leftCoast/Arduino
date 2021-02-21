#ifndef bmpImage_h
#define bmpImage_h

//LftC
#include <baseImage.h>


class bmpImage :	public baseImage {

	public:
				bmpImage(void);
	virtual	~bmpImage(void);
	
	virtual	bool		saveImage(char* inPath=NULL);
	virtual	RGBpack	getPixel(int x,int y,File inFile=NULL);
	
	virtual	void		setPixel(int x,int y,colorObj* aColor);
	virtual	void		setPixel(int x,int y,RGBpack* anRGBPack);
	virtual	void		setRow(int row,RGBpack* RGBPackArray,int xMin=0,xMax=0,File inFile=NULL);

	protected:
	
	virtual	bool		readImage(void);
	
				uint32_t	fileSize;
				uint32_t	creatorBits;
				uint32_t	DIBHeaderSize;	// No idea what this bit is.
				bool		rightSideUp;
				int     	imageOffset;
				int		imageDepth;
				int		pixBytes;
				int		bytesPerRow;
};

#endif