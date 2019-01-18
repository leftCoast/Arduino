
#include <label.h>
#include "keystroke.h"
#include "editField.h"
#include "idlers.h"
#include "keyboard.h"
#include <drawDelete.h>
#include <enterArrow.h>

keyboard*       ourKeyboard;

void setup() {
  
  ourKeyboard = newkeyboard(NULL);
}

void loop() {
  idle();
}
