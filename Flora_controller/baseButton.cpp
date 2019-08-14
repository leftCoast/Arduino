#include "baseButton.h"
#include "floraOS.h"

#define TEXT_SIZE     2
#define CHAR_WIDTH    6
#define BUTTON_HEIGHT 18
#define RADIUS        3



baseButton::baseButton(char* inLabel,int x, int y,int width, int height)
  : drawObj(x, y, width, height, touchLift) {

  // default button colors.
  activeBColor.setColor(LC_BLUE);
  activeBColor.blend(&white, 40);
  activeTColor.setColor(LC_WHITE);
  clickedBColor.setColor(LC_WHITE);
  clickedTColor.setColor(LC_BLACK);

  int numChars = strlen(inLabel) + 1;
  label = (char*) malloc(numChars);
  strcpy(label, inLabel);
}

baseButton::~baseButton(void) {

    if (label) {
        free(label);
        label = NULL;
    }
}

// The original didn't do what I expected. So I just hacked a local copy for the buttons here.
//nullEvent, touchEvent, liftEvent, dragBegin, dragOn, clickEvent
bool baseButton::acceptEvent(event* inEvent,point* locaPt) {

  switch (mEventSet) {
    case noEvents   : return false;             // noEvents, pass on..
    case touchLift    :                         // Classic button events, clicked lets you draw clicked.
      if (inEvent->mType==touchEvent) {         // If its a touch..
        if (inRect(locaPt)) {                   // - and if its on us..
          clicked   = true;                     // Might want to show we're clicked on.
          ourOS.beep(clicked);
          doAction(inEvent,locaPt);             // Do our stuff.
          theTouched  = this;                   // Tell the world WE are accepting this event set.
          needRefresh = true;                   // touchLift doesn't get a lift event. So it needs the setRefresh here.
          return true;                          // Tell the world the event has been accepted.
        }
      } else if (theTouched==this && inEvent->mType==liftEvent) {   // We only want lifts if we'd accepted the touch.
        clicked   = false;                      // And we're no longer clicked.
        ourOS.beep(clicked);
        doAction(inEvent,locaPt);               // Do our other stuff.
        needRefresh = true;                     // And here.. (see above)
        return true;                            // Again, tell the world the event has been accepted.
      }
      break;
    case fullClick  :               // Things like edit fields. A click changes their state.
      if (inEvent->mType==clickEvent) {   // If its a click event, that matches.
        if (inRect(locaPt)) {         // and if its on us..
          clicked   = false;          // No longer clicked by the time you see this.
          doAction();               // Do the action.
          return true;              // We don't set touched because this is a one shot event.
        }
      }
      break;
    case dragEvents :               // Things that move by touch.
      if (inEvent->mType==dragBegin) {      // If, the dragging finger has started..
        if (inRect(locaPt)) {         // and if its on us..
          doAction(inEvent,locaPt);     // Do our stuff.
          theTouched  = this;         // Tell the world WE are accepting this event set.
          return true;
        }
      } else if (inEvent->mType==dragOn) {    // still moving,
        doAction(inEvent,locaPt);         // Stil dragging? Keep drawing.
        return true;                  // Event has been accepted.
      } else if (inEvent->mType==liftEvent) { // Done dragging.
        doAction(inEvent,locaPt);         // Do our stuff.
        return true;                  // Again, tell the world the event has been accepted.
      }
      break;
    }
    return false;
  }



// OK, lets NOT call hookup() by default. If the derived needs to
// do idle things, let them call it.
void baseButton::idle(void) { }


void baseButton::drawSelf(void) {

  word dispWidth;
  word textWidth;
  
  if (clicked) {
    screen->fillRoundRect(x-2, y-2, width+4, height+4, RADIUS+2, &activeBColor);
    screen->fillRoundRect(x, y, width, height, RADIUS, &clickedBColor);
    screen->setTextColor(&clickedTColor, &clickedBColor);
  } else {
    screen->fillRoundRect(x-2, y-2, width+4, height+4, RADIUS+2, &white); 
    screen->fillRoundRect(x, y, width, height, RADIUS, &activeBColor);
    screen->setTextColor(&activeTColor, &activeBColor);
  }
  screen->setTextSize(TEXT_SIZE);
  screen->setTextWrap(false);
  dispWidth = width - (2 * RADIUS);
  textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(label)) - 1;
  if (dispWidth > textWidth) {
    screen->setCursor(x + RADIUS + ((dispWidth - textWidth) / 2), y + 2);
    screen->drawText(label);
  }
}


// Default action. You should call this in your derived method.
//void  baseButton::doAction(void) { ourOS.beep(); }
