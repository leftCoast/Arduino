Teensy 3.x RAM Monitor
======================

Dynamic Memory Monitor for Teensy 3.x by Adrian Hunt

> Supports Teensy 3.0, 3.1, 3.2, 3.5, 3.6, and Teensy LC

This repository is a fork of [the original code](https://sourceforge.net/p/teensy-3-x-rammonitor), 
which is hosted on source forge. I have additionally recreated the 
[original Wiki information](extras/HOME.md), as well as relicensed the code 
under the MIT license, and added support for Teensy 3.5, 3.6, and Teensy LC.


Overview
--------

Simplifies Teensy 3.x memory monitoring; providing both "raw" memory 
information and with frequent calls to the run() function, adjusted information 
with simulated stack allocations. Memory is also monitored for low memory state 
and stack and heap crashes.


Installation
------------

Download a zip of the repository, then include it from the Arduino IDE.


Usage
-----

See [RamMonitorExample.ino](examples/simple/RamMonitorExample.ino) for an example sketch.


More information
----------------

Check out the extras folder for extra information about the technical details of the project including an overview of how it works by the original author.

* [Home](extras/HOME.md)
  * [Overview](extras/OVERVIEW.md)
  * [How it works](extras/HOW-IT-WORKS.md)
  * [Teensy 3.x Memory Model](extras/TEENSY-MEMORY.md)
* [API Reference](extras/REFERENCE.md)


License
-------

The original project contained only this license:

> \*\* This code is provided without any warrenties or gaurentees of
 any kind. It is provided for any use, private, commercial or other. \*\*

This repository is therefore re-licensed under the MIT license with the original copyright intact.
