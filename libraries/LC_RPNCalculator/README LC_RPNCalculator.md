##LC_RPNCalculator

Ah, the LC_RPNCalculator. This was started as a Macintosh project because I wanted a RPN calculator and was tired of always looking for new ones. I wanted my own. Seeing I can’t figure out c# or the damn interface builder from apple the best I could do was pass in the text on top of the buttons. 

Funny but that was a blessing in disguise.

It forced me to build a fully self contained c++ library that relied on button presses coming in as c strings. Meaning it was extremely portable. So I brought it over to Arduino world and was able to compile it without any changes at all. Makes a really nice little calculator core.


**www.leftcoast.biz**  

Left Coast is where this came from. Use it all you want. I'd be nice if Left Coast got some credit for it.


Lets start with..


##class calculator

The only class in the library.

calculator::calculator(void) <— There you have one..

void calculator::buttonClick(const char* buttonID) <— Pass in your key text, look in source to see what’s available.


double calculator::getX(void) <— Your x reg, display this after each button press.

int calculator::getFixVal(void) <— Grab this before updating the display. So it comes out correctly.

void calculator::setDegrees(bool inDegrees)
bool calculator::toggleDegrees(void)
bool calculator::getDegrees(void)  <— These three are different ways to deal with the age old Degrees/Radians switch for doing trig functions. You can use them to set a mode button.

There’s more there but this’ll get you going. Look in the source to see how it works.

