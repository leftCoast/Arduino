#ifndef bmpImage_h
#define bmpImage_h

#include <baseImage.h>
#include <colorObj.h>


bool createNewBMPFile(const char* newPath,int inWidth,int inHeight);


class bmpImage :	public baseImage {

	public:
				bmpImage(const char* filePath);
	virtual	~bmpImage(void);
	
				void		setPWH(const char* imgPath,int w,int h);
	virtual	bool		setNewBMPFile(const char* BMPPath,int w,int h);
	virtual	bool		createNewDocFile(void);
	virtual	RGBpack	getRawPixel(int x,int y);
	virtual	void		setRawPixel(int x,int y,RGBpack* anRGBPack);	
	virtual	bool		checkDoc(File inFile);
				uint32_t	fileIndex(int x,int y);
					
	protected:
				
				char*		newImgPath;
				int		newW;
				int		newH;
				bool		rightSideUp;
				uint32_t	imageOffset;
				int16_t	pixBytes;
				int32_t	bytesPerRow;
};

#endif