#ifndef ltlOS_h
#define ltlOS_h

#define SYS_TEDIT_BCOLOR      1
#define SYS_TEDIT_TCOLOR      2
#define SYS_TEDIT_HCOLOR      3

#define SYS_PANEL_COLOR       4
#define SYS_TEXT_BTN_COLOR    5
#define SYS_CONTROL_BTN_COLOR 6
#define SYS_SHADOW_COLOR      7

#define NUM_SYS_COLORS        7   // Well, look up there and count 'em.


#define SYS_TEDIT_BCOLOR_C      LC_WHITE
#define SYS_TEDIT_TCOLOR_C      LC_BLACK
#define SYS_TEDIT_HCOLOR_C      LC_LT_BLUE

#define SYS_PANEL_COLOR_C       LC_LIGHT_GREY
#define SYS_TEXT_BTN_COLOR_C    LC_WHITE
#define SYS_CONTROL_BTN_COLOR_C LC_GREY
#define SYS_SHADOW_COLOR_C      LC_DARK_GREY




class litlOS {

  public:
  
          litlOS(void);
  virtual ~litlOS(void);

          colorObj  getColor(byte colorIndex);
          colorObj  systemPalette[NUM_SYS_COLORS];
}

extern  litlOS ourOS;   // As it says, this is our OS. For better or worse..


#endif
