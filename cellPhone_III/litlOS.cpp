

#define SYS_TEDIT_BCOLOR_C      LC_WHITE
#define SYS_TEDIT_TCOLOR_C      LC_BLACK
#define SYS_TEDIT_HCOLOR_C      LC_LT_BLUE

#define SYS_PANEL_COLOR_C       LC_LIGHT_GREY
#define SYS_TEXT_BTN_COLOR_C    LC_WHITE
#define SYS_CONTROL_BTN_COLOR_C LC_GREY
#define SYS_SHADOW_COLOR_C      LC_DARK_GREY

litlOS::litlOS(void) {

  
}


litlOS:~litlOS(void) {

  
}

colorObj litlOS::getColor(byte colorIndex) {

  colorObj  result;

  if (colorIndex<NUM_SYS_COLORS) {
    result.setColor(&systemPalette[colorIndex];
  }
  return result;
}
colorObj litlOS::systemPalette[NUM_SYS_COLORS];
