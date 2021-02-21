#ifndef baseImage_h
#define baseImage_h

#include <colorObj.h>

#define TEMP_PATH_CHARS	40				// Max length(-1) of temp file full path.
#define TEMP_FOLDER		"/temp/";	// Folder to save temp files in.
#define FILE_SEARCH_MS	500			// Amount of milliseconds allowed to find a temp file name.

class baseImage {

	public:
					baseImage(char* inPath=NULL);
	virtual		~baseImage(void);
	
	virtual		bool		newImage(int inWidth,int inHeight);
	virtual		bool		openImage(char* inPath);
	virtual		bool		saveImage(char* inPath=NULL);
	
	virtual		colorObj	getPixel(int x,int y);
	virtual		RGBpack	getPixel(int x,int y);
	virtual		void		getRow(int row,RGBpack* RGBArray,int numPix=0,int xStart=0);
	
	virtual		void		setPixel(int x,int y,colorObj* aColor);
	virtual		void		setPixel(int x,int y,RGBpack* anRGBPack);
	virtual		void		setRow(int row,RGBpack* RGBPackArray,int numPix=0,int xStart=0);
	
	protected:
	
	virtual		bool		readImage(void) = 0;
	virtual		bool		copyToWorkingFile(void);
	virtual		bool		newWorkingFile(int width,int height);
	virtual		RGBpack	readPixel(int x,int y) = 0;
	virtual		void		writePixel(int x,int y,RGBpack* anRGBPack);
					
					bool		openImage(int mode);
					void		closeIMage(void);
					bool		openTemp(int mode);
					void		closeTemp(void);
					
					char*		imagePath;
					char*		tempPath;
					File		imageFile;
					File		tempFile;
					bool		imageFileOpen;
					bool		tempFileOpen;
					bool		haveInfo;
					int		width;
					int		height;
};


#endif
