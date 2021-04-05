#ifndef bmpImage_h
#define bmpImage_h

#include <baseImage.h>
#include <colorObj.h>

class bmpImage :	public baseImage {

	public:
				bmpImage(void);
	virtual	~bmpImage(void);

	virtual		bool				getRow(int row,RGBpack* RGBArray,int numPix,File* inFile);
								
	protected:
	
	virtual		RGBpack			getPixel(int x,int y,File* imageFile);
	virtual		void				setPixel(int x,int y,RGBpack* anRGBPack,File* imageFile);	
	virtual		bool				checkDoc(void);
					uint32_t			calcFileSize(void);
					void				calcDefaults(void);
	virtual		bool				initNewTempFile(void);
					unsigned long	fileIndex(int x,int y);
				
				bool				rightSideUp;
				int     			imageOffset;
				int				imageDepth;
				int				pixBytes;
				int				bytesPerRow;
				unsigned long	fileSize;
};

#endif