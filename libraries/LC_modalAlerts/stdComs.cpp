#include <stdComs.h>
#include <lilOS.h>			// for the OSPtr.


// **************************************************************
// ***********************		listener		***********************
// **************************************************************


listener::listener(void) {  }

listener::~listener(void) {  }
	
void listener::handleCom(stdComs comID) {  }



// **************************************************************
// ***********************		stdCom		***********************
// **************************************************************


stdComBtn::stdComBtn(int xLoc,int yLoc,const char* path,stdComs iconType,listener* inListener,int pixles)
	:iconButton(xLoc,yLoc,path,pixles) { 
	
	ourListener = inListener;
	ourComID		= iconType;
}


stdComBtn::~stdComBtn(void) {  }

	
void stdComBtn::doAction(void) { ourListener->handleCom(ourComID); }


void stdComBtn::active(bool trueFalse) {
				
	if (trueFalse) {
		setEventSet(touchLift);
	} else {
		setEventSet(noEvents);
	}
}


// Need a standard label icon? Here you go!
bmpObj* newStdLbl(int x,int y,iconSize inSize,stdLabels iconType) {

	bmpObj*	anBmpObjPtr;

	anBmpObjPtr = NULL;
	switch (iconType) {
		case warnLbl		:
			if (inSize==icon32) {
				anBmpObjPtr = new bmpObj(x,y,32,32,OSPtr->stdIconPath(warn32));
			}
		break;
		case choiceLbl		:
			if (inSize==icon32) {
				anBmpObjPtr = new bmpObj(x,y,32,32,OSPtr->stdIconPath(choice32));
			}
		break;
		case noteLbl		:
			if (inSize==icon32) {
				anBmpObjPtr = new bmpObj(x,y,32,32,OSPtr->stdIconPath(note32));
			}
		break;
		case folderLbl		:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,OSPtr->stdIconPath(folder16));
			}
		break;
		case folderRetLbl	:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,OSPtr->stdIconPath(fldrRet16));
			}
		break;
		case docLbl			:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,OSPtr->stdIconPath(doc16));
			}
		break;
		case SDCardLbl		:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,OSPtr->stdIconPath(SDCard16));
			}
		break;
	}
	if (anBmpObjPtr) {
		if (inSize==icon32) {
			anBmpObjPtr->setMask(&(OSPtr->icon32Mask));
		}
	}
	return anBmpObjPtr;
}


// Need a standard button? Let us create one for you!
stdComBtn* newStdBtn(int x,int y,iconSize inSize,stdComs iconType,listener* inListener) {

	stdComBtn*	anIconPtr;

	anIconPtr = NULL;
	switch (iconType) {
		case cutCmd		:
			if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(cut32),iconType,inListener,32);
			}		
		break;
		case copyCmd	:
			if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(copy32),iconType,inListener,32);
			}		
		break;
		case closeCmd	:
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(x22),iconType,inListener,22);
			}		
		break;
		case pasteCmd :
			if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(paste32),iconType,inListener,32);
			}		
		break;
		case cancelCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(x22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(x32),iconType,inListener,32);
			}	
		break;
		case okCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(check22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(check32),iconType,inListener,32);
			}	
		break;
		case newFileCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(fNew22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(fNew32),iconType,inListener,32);
			}	
		break;
		case newFolderCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(FdrNew22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(FdrNew32),iconType,inListener,32);
			}	
		break;
		case newItemCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(cross22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(cross32),iconType,inListener,32);
			}	
		break;
		case openFileCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(fOpen22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(fOpen32),iconType,inListener,32);
			}	
		break;
		case saveFileCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(fSave22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(fSave32),iconType,inListener,32);
			}	
		break;
		case deleteItemCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(trashC22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(trashC32),iconType,inListener,32);
			}	
		break;
		case searchCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(search22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(search32),iconType,inListener,32);
			}	
		break;
		case sortCmd	:
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(sort22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(sort32),iconType,inListener,32);
			}	
		break;
		case editCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(edit22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,OSPtr->stdIconPath(edit32),iconType,inListener,32);
			}	
		break;
	}
	if (anIconPtr) {
		if  (inSize==icon22) {
			anIconPtr->setMask(&(OSPtr->icon22Mask));
		} else if (inSize==icon32) {
			anIconPtr->setMask(&(OSPtr->icon32Mask));
		}
	}
	return anIconPtr;
}
