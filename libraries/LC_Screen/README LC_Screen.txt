LC_Screen

LC_Screen. This was started actually, just for fun. A long time ago working on the first vacuum gauge. The thought was to have a toolkit for drawing and managing a graphic user interface for Arduinos using the Adafruit 2.8” TFT. The resistive screens were soso, the memory footprint of the necessary bits was too large so things kinda’ parked for awhile.

The original calculator project came along bringing the new capacitive touch screens. The screens were much better, the memory foot print was smaller but still, confining. Things moved forward some more.. And stopped.

The Teensy 3.2 µp showed up and that jumped everything forward a BUNCH! This also restarted the calculator project and finished it. Then reopened up the old Vacuum gauge project and finished that as well. This also caused the push to get the screen library to the level it is now. A generic screen object that can be created from different hardware libraries, base drawing objects that can be clicked on, bitmap handling, text handling. The user interface runs in the background. There are some bits missing but all the basic framework is now in place and running.

www.leftcoast.biz 

Left Coast is where this came from. Use it all you want. I'd be nice if Left Coast got some credit for it.


Lets start with..

***
displayObj 
***

The global object screen is a displayObj. It is created automatically. It’s initialized by selecting the hardware object you want and optionally the rotation.

boolean initScreen(byte hardware, int inRotation=0) <— Call to init the screen.

The user typically won’t interact with the screen itself, typically the different subclassed drawObjs will interact with the screen.

***
drawObj
***

This is the base class of something that can be drawn on the screen and, if desired, respond to user clicks. This is what should be inherited and customized to make your displayable and clickable screen objects.
