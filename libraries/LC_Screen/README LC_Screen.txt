A GUI toolkit for Arduino. Well, actually better suited to something more like a Teensy. You need more RAM than your standard Arduino to run this.

There are a few core bits that come together to create this toolkit. displayObj is the base class that you draw to. The global, screen, is a displayObj. It is created by choosing an init parameter that matches your hardware.

Then there is the base drawing object drawObj. This is a rectangle that can respond to clicks from the user. It takes care of redrawing itself when there are changes. Location, size, possible callback function, its all here in drawObj. This is what is used as a base class for everyone to create user controls & images for the screen.

All of the interaction between the use interface objects is managed by the global object viewList. Which is a instance of the viewManager class. When you create objects for the screen, you add them by address to the view list to manage. the viewManager class derives from idler meaning it runs in the background not cluttering up the main program.

There are some bits missing but all the basic framework is in place and running. The plan is that the programmers will use drawObj and create their own screen controls.

www.leftcoast.biz 

Left Coast is where this came from. Use it all you want. I'd be nice if Left Coast got some credit for it.


Lets start with..

***
displayObj 
***

The global object screen is a displayObj. It is created automatically. It’s initialized by selecting the hardware object you want and optionally the rotation and then, possibly the SD card. Most have them.

boolean initScreen(byte hardware,byte inRotation=0,byte,SD_CS=0); <— Call to init the screen.

The user typically won’t interact with the screen itself, typically the different subclassed drawObjs will interact with the screen.

***
drawObj
***

This is the base class of something that can be drawn on the screen and, if desired, respond to user clicks. This is what should be inherited and customized to make your displayable and clickable screen objects.


***
lineObj
***
Just like it says, different ways of setting up a Line for drawing to the screen.


***
label
***
A text label. Size, color, background color, justification. Also the ability to input numbers and have them formatted and displayed as text to the screen are features of label.


***
bmpPipe
***
Actually not derived from drawObj. But used by those that are. Once created, a bmpPipe is used for getting rectangular parts of bitmaps onto the screen. The pipe is opened with a bitmap file name. An SD card reader must already be set up and running. The source rectangle could be set in the constructor or later, Then an x,y top left point is specified on the screen and the pipe will copy the its source rectangle of bitmap bits to the screen rectangle of the same size located at x,y.

***
adafruit_1947_Obj, adafruit_1431_Obj…
***
These are the different hardware interface objects derived from displayObj that become your graphic environment.

***
other objects?
***
There will most likely be other drawObject derived things in this folder. The plan was to make it easy for people to derive custom objects. So I’m sure some will end up being useful enough to end up in here.







