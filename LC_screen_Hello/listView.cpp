#include "listView.h"


listView::listView(word inX,word inY,word width,word inRowHeight,word inRows)
  : drawObj(inX,inY,width,inRowHeight*inRows,true) {

    rowHeight = inRowHeight;
    numRows = inRows;
    currentRow = 0;            // Units are rows.
    totalRows = 0;
    colWidth = 0;              
    numCols = 0;
    currentCol = 0;
    totalCols = 0;
  }


void listView::addObj(drawObj* newObj) {

  viewMgr::addObj(newObj);
  totalRows++;
}


drawObj* listView::findObj(word index) {

  drawObj*  trace;
  word      i;

  trace = theList;
  i = 0;
  while(i<index && trace!=NULL) {
    if (trace) {
      trace = (drawObj*)trace->next;
      i++;
    }
  }
  return trace;
}


void listView::drawSelf(void) {

  drawObj*  trace;
  word      i;
  word      endRow;
  word      x;
  word      y;
  word      row;
  
  trace = findObj(currentRow);
  i = currentRow;
  endRow = currentRow+numRows;
  x = currentCol*colWidth;
  row = 0;
  while(i<endRow && trace!=NULL) {  // Set the positions then draw 'em.
    if (trace) {
      y = row * rowHeight;
      row++;
      trace->setLocation(x,y);
      trace->draw();
      trace = (drawObj*)trace->next;
      i++;
    }
  }
}


void listView::idle(void) {  }

  
void listView::scrollList(scrollCommand command) {

  switch (command) {
    case  scrollUp : 
      if (currentRow>0) {
        currentRow--;
        needRefresh = true;
      }
      break;
    case  pageUp  : 
      currentRow = max(0,currentRow-numRows);
      needRefresh = true;
      break;
    case  toTop :
      if (currentRow) {   // IE, not zero.
        currentRow = 0;
        needRefresh = true;
      }
      break;
    case  scrollDn : 
      if (totalRows>numRows) {
        if (currentRow+numRows<totalRows) {
          currentRow++;
          needRefresh = true;
        }
      }
      break;
    case  pageDn :
      if (totalRows>numRows) {
        currentRow = min(totalRows-numRows,currentRow+numRows);
        needRefresh = true;
      }
      break;
    case  toBottom :
      if (totalRows>numRows) {
        currentRow = max(totalRows-numRows,0);
        needRefresh = true;
      }
      break;
    case  scrollRight : 
      if (totalCols>numCols) {
        if (currentCol+numCols<totalCols) {
          currentCol++;
          needRefresh = true;
        }
      }
      break;
    case  pageRight : 
      if (totalCols>numCols) {
        currentCol = min(totalRows-numCols,currentCol+numCols);
        needRefresh = true;
      }
      break;
    case  toRight :
        if (totalCols>numCols) {
        currentCol = max(totalCols-numCols,0);
        needRefresh = true;
      }
      break;
    case  scrollLeft :
      if (currentCol>0) {
        currentCol--;
        needRefresh = true;
      }
      break;
    case  pageLeft :
      currentCol = max(0,currentCol-numCols);
      break;
    case  toLeft : 
     if (currentCol>0) { 
        currentCol = 0;
        needRefresh = true;
      }
      break;
  }        
}



// And here's the guy that will tell the listView where to scroll.
scrollControl::scrollControl(word inX,word inY,word width, word height, listView* inList, scrollCommand inCommand) 
  : drawObj(inX,inY,width,height,true) {

  ourList = inList;
  command = inCommand;
}

  
void scrollControl::drawSelf(void) { drawObj::drawSelf(); }

void scrollControl::doAction(void) { if (ourList) ourList->scrollList(command); }

     
