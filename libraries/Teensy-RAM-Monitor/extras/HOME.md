# Welcome to the Teensy 3.x RamMonitor wiki.

RamMonitor is a simple class that provides ram usage information in a single c++ header. With initialization and freqent calls to the run() method, RamMonitor watches stack usage and emulates stack allocation. It all so provides low memory and memory crash warnings.

> ** \*\* This code is provided without any warrenties or gaurentees of
any kind. It is provided for any use, private, commercial or other. \*\* **

I'm sharing this little nugget of code to the world, in the hope that it may be of use to someone. All of the technical details are available else where on the net; but I failed to find any where that it was all brought together. Use the code as you wish; all I ask, is that any changes that may be of use to others, are shared here for the greater good... a credit wouldn't go amiss either!! ;)

*Editor's note:* Because of the dubious nature of the original license, this project has been re-licensed under the MIT license. Check the repository [LICENSE file](../LICENSE) for the full text.

RamMonitor was written for and tested on a Teensy 3.1; with what I know, Teensy 3.0 and 3.2 work the same way... at a guess Teensy LC should be the same too. It may even work for other Teensy's with little modification.

* [Overview](OVERVIEW.md)
* [Teensy 3.x Memory Model](TEENSY-MEMORY.md)
* [How it works](HOW-IT-WORKS.md)
* [API Reference](REFERENCE.md)