#include "litlOS.h"


// The actual system control colors. We'll store them here to keep down
// clutter and confusion. They should be acssesed by index not color vales.
//
// Now, if you don't like these system colors and want to make a global change?
// These are what you'd change.

#define SYS_TEDIT_BCOLOR_C      LC_WHITE
#define SYS_TEDIT_TCOLOR_C      LC_BLACK
#define SYS_TEDIT_HCOLOR_C      LC_LIGHT_BLUE

#define SYS_PANEL_COLOR_C       LC_GREY
#define SYS_INPUT_BTN_COLOR_C   LC_LIGHT_GREY
#define SYS_CONTROL_BTN_COLOR_C LC_DARK_GREY
#define SYS_CONTROL_LBL_COLOR_C LC_BLACK
#define SYS_SHADOW_COLOR_C      LC_CHARCOAL

litlOS ourOS;


litlOS::litlOS(void) {

  systemPalette[SYS_TEDIT_BCOLOR].setColor(SYS_TEDIT_BCOLOR_C);
  systemPalette[SYS_TEDIT_TCOLOR].setColor(SYS_TEDIT_TCOLOR_C);
  systemPalette[SYS_TEDIT_HCOLOR].setColor(SYS_TEDIT_HCOLOR_C);
  
  systemPalette[SYS_PANEL_COLOR].setColor(SYS_PANEL_COLOR_C);
  systemPalette[SYS_INPUT_BTN_COLOR].setColor(SYS_INPUT_BTN_COLOR_C);
  systemPalette[SYS_CONTROL_BTN_COLOR].setColor(SYS_CONTROL_BTN_COLOR_C);
  systemPalette[SYS_CONTROL_LBL_COLOR].setColor(SYS_CONTROL_LBL_COLOR_C);
  systemPalette[SYS_SHADOW_COLOR].setColor(SYS_SHADOW_COLOR_C);
}


// Currently nothing to do.. But maybe later?
litlOS::~litlOS(void) {  }


// We're building a user control. Lets get the expected colors for it.
colorObj litlOS::getColor(byte colorIndex) {

  colorObj  result(LC_BLACK);                   // Can't go wrong with basic black, can yu? :)

  if (colorIndex<NUM_SYS_COLORS) {              // Sanity check the index.
    result.setColor(&(systemPalette[colorIndex])); // Grab the color indexed.
  }
  return result;                                // Pass back the color object.
}
