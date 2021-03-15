#ifndef bmpImage_h
#define bmpImage_h

#include <baseImage.h>


class bmpImage :	public baseImage {

	public:
				bmpImage(void);
	virtual	~bmpImage(void);

	protected:
	
	virtual		RGBpack	getPixel(int x,int y,File* imageFile);
	virtual		void		setPixel(int x,int y,RGBpack* anRGBPack,File* imageFile);	
	virtual		bool		checkDoc(void);
	virtual		bool		initNewtempFile(void);
								
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