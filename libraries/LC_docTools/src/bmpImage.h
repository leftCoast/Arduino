#ifndef bmpImage_h
#define bmpImage_h

#include <baseImage.h>
#include <colorObj.h>


bool createNewBMPFile(char* newPath,int inWidth,int inHeight);


class bmpImage :	public baseImage {

	public:
				bmpImage(char* filePath);
	virtual	~bmpImage(void);
	
				void		setPWH(char* imgPath,int w,int h);
	virtual	bool		setNewBMPFile(char* BMPPath,int w,int h);
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
				int32_t	imageOffset;
				int16_t	pixBytes;
				int32_t	bytesPerRow;
};

#endif