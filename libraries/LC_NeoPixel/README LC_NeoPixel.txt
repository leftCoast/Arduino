LC_NeoPixel

This toolset is for working with NeoPixels. it extends the Adafruit Neopixel library to use our colorObj class. This makes doing programmed patterns and blends much easer. Also allows colors to pas between NeoPixels & color screens.

Also included in this package is chainPixels. This allows the user to daisy chain a set of different newPixel devices (rings, sticks, singles, strings) and be able to program each independently, disregarding indexing issues. Not only that, but all the devices are run in the background so as to not clutter up your main sketch. This is also a great tool for having multiple instances of the same neopixel class. 


www.leftcoast.biz 

Left Coast is where this came from. Use it all you want. I'd be nice if Left Coast got some credit for it.


Lets start with..

***
class neoPixel
***
This is your basic building block. The most common constructor is defaulted so all you need is the number of pixels and the pin number. This class extends the Adafruit library to use our coloObj for passing colors in and out.


***
chainPixels
***
Chain pixels along with its sibling class pixelGroup create a fully auto running group of neopixel devices. The user creates classes inheriting pixel group and uses this subclass to program the group’s behavior. Then the different groups are added to the chain pixel object in the order that they are physically wired. Their the chain pixel object creates the Adafruit class and manages the entire thing in the background.

This was originally developed for running lights on our quadcopters. Different light bards had different functions, some shared functions. For example some quads would have lights on the motor pods so you wanted the same thing in multiple places. Some needed information from the pilot so thy would watch the incoming R/C receiver channels. Without this way to break up the sections, the code would have been daunting.

chainPixels::chainPixels(byte inPin) <- only needs pin number. Number of LEDs is calculated dynamically

void chainPixels::addGroup(pixelGroup* inGroup) <- This is what you use to add your pixel group objects.

That’s pretty much all the calls you are going to need to use this. Everything else is automatic.


***
pixelGroup
***
This is the bit that goes with chain pixels. You inherit pixelGroup and write your own behaviour for the different light groups. There are some example groups included so you can get an idea what we mean. The pixelGroup is much like the neoPixel class except it has a draw method you inherit and rewrite like your own private loop(). If you need to, draw fast and get out, others are waiting. Then chain pixel will gather it all up and display everyone’s drawings on the lights.

pixelGroup::pixelGroup(word inNumPixels) <- You don’t know your pin or your index, just how many pixels you have.

pixelGroup::draw(). <— Like your own private loop() Do what you need to do, don’t block. Thats it.


***
liteLine
***
liteLine isn’t really used much. Its included because this is where it belongs. Its all about drawing a segment of lights independent of its position on a string of lights. Its something to play around with.

That's it.
