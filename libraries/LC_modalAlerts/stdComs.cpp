#include <stdComs.h>
#include <lilOS.h>			// for the ourOSPtr.


// **************************************************************
// ***********************		listener		***********************
// **************************************************************


listener::listener(void) {  }

listener::~listener(void) {  }
	
void listener::handleCom(stdComs comID) {  }



// **************************************************************
// ***********************		stdCom		***********************
// **************************************************************


stdComBtn::stdComBtn(int xLoc,int yLoc,char* path,stdComs iconType,listener* inListener,int pixles)
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
				anBmpObjPtr = new bmpObj(x,y,32,32,ourOSPtr->stdIconPath(warn32));
			}
		break;
		case choiceLbl		:
			if (inSize==icon32) {
				anBmpObjPtr = new bmpObj(x,y,32,32,ourOSPtr->stdIconPath(choice32));
			}
		break;
		case noteLbl		:
			if (inSize==icon32) {
				anBmpObjPtr = new bmpObj(x,y,32,32,ourOSPtr->stdIconPath(note32));
			}
		break;
		case folderLbl		:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,ourOSPtr->stdIconPath(folder16));
			}
		break;
		case folderRetLbl	:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,ourOSPtr->stdIconPath(fldrRet16));
			}
		break;
		case docLbl			:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,ourOSPtr->stdIconPath(doc16));
			}
		break;
		case SDCardLbl		:
			if (inSize==icon16) {
				anBmpObjPtr = new bmpObj(x,y,16,16,ourOSPtr->stdIconPath(SDCard16));
			}
		break;
	}
	if (anBmpObjPtr) {
		if (inSize==icon32) {
			anBmpObjPtr->setMask(&(ourOSPtr->icon32Mask));
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
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(cut32),iconType,inListener,32);
			}		
		break;
		case copyCmd	:
			if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(copy32),iconType,inListener,32);
			}		
		break;
		case closeCmd	:
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(x22),iconType,inListener,22);
			}		
		break;
		case pasteCmd :
			if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(paste32),iconType,inListener,32);
			}		
		break;
		case cancelCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(x22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(x32),iconType,inListener,32);
			}	
		break;
		case okCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(check22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(check32),iconType,inListener,32);
			}	
		break;
		case newFileCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(fNew22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(fNew32),iconType,inListener,32);
			}	
		break;
		case newFolderCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(FdrNew22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(FdrNew32),iconType,inListener,32);
			}	
		break;
		case newItemCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(cross22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(cross32),iconType,inListener,32);
			}	
		break;
		case openFileCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(fOpen22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(fOpen32),iconType,inListener,32);
			}	
		break;
		case saveFileCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(fSave22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(fSave32),iconType,inListener,32);
			}	
		break;
		case deleteItemCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(trashC22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(trashC32),iconType,inListener,32);
			}	
		break;
		case searchCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(search22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(search32),iconType,inListener,32);
			}	
		break;
		case sortCmd	:
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(sort22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(sort32),iconType,inListener,32);
			}	
		break;
		case editCmd :
			if (inSize==icon22) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(edit22),iconType,inListener,22);
			}	else if (inSize==icon32) {
				anIconPtr = new stdComBtn(x,y,ourOSPtr->stdIconPath(edit32),iconType,inListener,32);
			}	
		break;
	}
	if (anIconPtr) {
		if  (inSize==icon22) {
			anIconPtr->setMask(&(ourOSPtr->icon22Mask));
		} else if (inSize==icon32) {
			anIconPtr->setMask(&(ourOSPtr->icon32Mask));
		}
	}
	return anIconPtr;
}