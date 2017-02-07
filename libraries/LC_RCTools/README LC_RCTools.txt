LC_baseTools

This toolset is for communicating with radio control systems. Servos & receivers. R/C systems use a 1..2 ms pulse to communicate. 1.5 ms is center position. These pulses have to be repeated at 20ms or less intervals or the servos will turn off.


www.leftcoast.biz 

Left Coast is where this came from. Use it all you want. I'd be nice if Left Coast got some credit for it.


Lets start with..

***
class servo - SOME BLOCKING
***

A real simple servo class. It does block while the pin is held high but typically that's not an issue for most. It doesn't monkey around with any of the chips timers and things so it plays well with most everything else.

servo mServo(inPin);	// Tell it what pin to use for your RC servo.

the only other call is..


mServo.setServo(val) // Val is a float from -100 to 100

calling idle() in your main loop function lets it run in the background.


***
class reciverPin
***

This class allows you to read the timing pulses of RC receivers. You can read them and take action. Or pass them through to servos. Or, maybe.. read/modify and send them on. At Left Coast, we used the signals to trigger different light displays on our quadcopters. This class is based on the PinChangeInt library. I really don’t understand how that stuff works. But it does and I’m forever greatful for it.

reciverPin(int inPin);   	// What pin will this channel come into?
unsigned long pinResult();	// what’s the current pulsewidth in microseconds, coming in from this channel.

That's it.
