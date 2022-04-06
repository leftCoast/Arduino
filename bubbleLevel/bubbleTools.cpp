#include "bubbleTools.h"
#include <triDVector.h>
#include <resizeBuff.h>



colorObj       backColor;
bubbleRender*  renderMan;                          // triDRender is the engine that reads the model to do the 2D drawing.
triDRotation   new_angle;                          // This sets the oreantation of the model's drwaing on the screen.
triDRotation   my_angle;                           // Saves the last location.
bitmap         bubbleMap(TARGET_DIA,TARGET_DIA);   // A bitmap for offscreen drawing.

double radToDeg(double x) { return x*(180.0/PI); }  // Humans like degrees. These two do the traslantions.
double degToRad(double x) { return x*(PI/180.0); }


triDTriangle* createCircleThing(int slices,double coneAngle,double radius) {
   
   double         slice;
   double         twoPi;
   triDVector     vectI;
   triDVector     vectA;
   triDTriangle*  facets;
   triDRotation   rotation;
   
   facets = NULL;
   if (resizeBuff(sizeof(triDTriangle)*slices,(byte**)&facets)) {
      twoPi = 2*M_PI;
      slice = twoPi/(float)slices;
      Serial.println(slice,5);
      vectI.setVector(0,0,radius);
      rotation = setRotation(0,coneAngle,0);
      vectI.rotateVect(&rotation);
      for (int i=0;i<slices;i++) {
         
         facets[i].corners[0].x = 0;                     // 
         facets[i].corners[0].y = 0;
         facets[i].corners[0].z = radius;
                   
         vectA.setVector(&vectI);                  // Initial vector.
         rotation = setRotation(0,0,i*slice);      // How far we'll need to shift the vector..
         vectA.rotateVect(&rotation);              // Add this much to the rotation.
         facets[i].corners[2].x = vectA.getX();
         facets[i].corners[2].y = vectA.getY();
         facets[i].corners[2].z = vectA.getZ();
         
         rotation = setRotation(0,0,slice);        // We've aleready added i*slice. We need one more slice.
         vectA.rotateVect(&rotation);
         facets[i].corners[1].x = vectA.getX();
         facets[i].corners[1].y = vectA.getY();
         facets[i].corners[1].z = vectA.getZ();
      }
   }
   return facets;
}


void setupModel(void) {
   
   triDPoint      location;                                                   // This sets the location of the model's drawing on the screen.
   triDTriangle*  theModel = createCircleThing(SLICES,degToRad(15),RADIUS-2);   // This hand crafts the list of triangles.
   bubble*        arrayModel = new bubble(theModel,SLICES);                   // Use the hand crafted list to make an array model.
      
   renderMan = new bubbleRender(BOUND_X,BOUND_Y,BOUND_DIA,BOUND_DIA);  // Create the render engine with its screen location. (x,y,w,h)
   renderMan->begin(arrayModel);                // Slot the model into the render enegine.
   renderMan->setObjScale(1);                   // Set the drawing scale.
   new_angle.xRad = 0;                          // The angle around x axis.
   new_angle.yRad = 0;                          // The angle around y axis.
   new_angle.zRad = 0;                          // The angle around z axis.
   renderMan->setObjAngle(&new_angle);          // Set the orentation.
   my_angle = new_angle;
   location.x = 99;                             // x position offset.
   location.y = 99;                             // y position offset.
   location.z = 0;                              // z position offset.
   renderMan->setObjLoc(&location);             // Set the location offsets in..
   viewList.addObj(renderMan);                  // Add this render object into the screen list. (causing it to draw itself)
}


bubble::bubble(triDTriangle* inList,long numItems)
   : arrayList(inList,numItems) {
   
   centerPt.x = 0;
   centerPt.y = 0;
   centerPt.z = 0;
   haveCenter  = true;
}

bubble::~bubble(void) {  }

point pt[SLICES*3];

void clearPtList(void) {

   for (int i=0;i<SLICES*3;i++) {
      pt[i].x = -1;
      pt[i].y = -1;
   }
}


void addTriangle(viewFacet* aFacet) {

   int   i;
   bool done;

   for (byte j=1;j<3;j++) {
      i = 0;
      done = false;
      while(!done) {
         if (aFacet->corner[j].x==pt[i].x && aFacet->corner[j].y==pt[i].y) {
            done = true;
         } else if (pt[i].x == -1) {
            pt[i].x = aFacet->corner[j].x;
            pt[i].y = aFacet->corner[j].y;
            done = true;
         } else {
            i++;
         }
      }
   }
}


void erasePoints(void) {
   
   int   i;
   bool done;

   i = 0;
   done = false;
   while(!done) {
      if (pt[i].x == -1) {
         done = true;
      } else {
         screen->drawPixel(pt[i].x,pt[i].y,&backColor);
         i++;
      }
   }
}

bubbleRender::bubbleRender(int inX,int inY,int inWidth,int inHeight)
   : triDRender(inX,inY,inWidth,inHeight) { clearPtList(); }

bubbleRender::~bubbleRender(void) {  }





// This is where it all gets put together. Can we actually draw this thing?
void bubbleRender::drawSelf(void) {

   viewFacet   aFacet;
   colorObj    aColor;
   bool        done;

   if (!init) {
      screen->drawRect(this,&red);
      return;
   }
   if (ourModel->openList()) {
      theGrid->setNeedRefresh();
      if (setupChange) {
         if (createList()) {
            setupChange = false;
         }
      }
      resetList();
      erasePoints();
      clearPtList();
      done = false;
      do {
         aFacet = getNextViewFacet();
         if (aFacet.normalVect.isNullVector()) {
            done = true;
         } else {
            offset2DFacet(&aFacet,x,y);
            addTriangle(&aFacet);
            //screen->drawPixel(aFacet.corner[0].x,aFacet.corner[0].y,&cyan);
            screen->drawPixel(aFacet.corner[1].x,aFacet.corner[1].y,&cyan);
            screen->drawPixel(aFacet.corner[2].x,aFacet.corner[2].y,&cyan);
         }
      } while(!done);
      ourModel->closeList();
   }
}


/*
void bubbleRender::drawSelf(void) {
   
   rect        eraseRect;

   theGrid->setNeedRefresh();
   eraseRect = getLastRect();
   eraseRect.insetRect(-1);
   screen->fillRect(&eraseRect,&backColor);
   triDRender::drawSelf();  
}
*/

/*
void bubbleRender::drawSelf(void) {
   
   viewFacet   aFacet;
   rect        eraseRect;
   rect        renderRect;
   offscreen   oSDisp;
   bool        first;
   bool        done;

     
   if (!init) {
      screen->drawRect(this,&red);
      return;
   }
   
   eraseRect = getLastRect();
   eraseRect.insetRect(-1);
   
   if (ourModel->openList()) {
      if (setupChange) {
         if (createList()) {
            setupChange = false;
         }
      }
      resetList();
      first = true;
      done = false;
      do {
         aFacet = getNextViewFacet();
         if (aFacet.normalVect.isNullVector()) {
            done = true;
         } else {
            offset2DFacet(&aFacet,x,y);
            add2DPointToRect(&aFacet,first);
            first = false;
         }
      } while(!done);
      ourModel->closeList();
      renderRect = getLastRect();
      renderRect.insetRect(-1);
      oSDisp.beginDraw(&bubbleMap,renderRect.minX(),renderRect.minY());
      oSDisp.fillRect(&renderRect,&backColor);
      triDRender::drawSelf();
      oSDisp.endDraw();
      screen->fillRect(&eraseRect,&backColor);
      screen->blit(renderRect.minX(),renderRect.minY(),&bubbleMap);
      theGrid->setNeedRefresh();
   }
}
*/

//****************************************************************************************
// grid:
// 
// The cross hair thing.
//****************************************************************************************


grid::grid(int centerX,int centerY)
   :drawObj(BOUND_X,BOUND_Y,BOUND_DIA,BOUND_DIA) {

   center.x = centerX;
   center.y = centerY;
   //gridColor.setColor(LC_CYAN);
   gridColor.setColor(&red);
   gridColor.blend(&black,50);
}


grid::~grid(void) { }


void grid::setColor(colorObj* inColor) { gridColor.setColor(inColor); }


void grid::drawSelf(void) {

   screen->drawCircle(BOUND_X,BOUND_Y,BOUND_DIA,&gridColor);
   screen->drawCircle(TARGET_X,TARGET_Y,TARGET_DIA,&gridColor);
   screen->drawHLine(BOUND_X,BOUND_CY,(BOUND_DIA-TARGET_DIA)/2,&gridColor);
   screen->drawHLine(BOUND_CX+(TARGET_DIA/2),BOUND_CY,(BOUND_DIA-TARGET_DIA)/2,&gridColor);
   screen->drawVLine(BOUND_CX,BOUND_Y,(BOUND_DIA-TARGET_DIA)/2,&gridColor);
   screen->drawVLine(BOUND_CX,BOUND_CY+(TARGET_DIA/2),(BOUND_DIA-TARGET_DIA)/2,&gridColor);
}

grid* theGrid;
