# How It Works
RamMonitors workings are quite simple. The examples here are simplified; an understanding of the [Teensy Memory](TEENSY-MEMORY.md) model will help too. Okay, lets explore...

The current stack size is calculated by creating a local variable, the address of this variable is the top of stack address; the stack extending from the top of memory, down to this point. A linker created "variable" marks the bottom of the stack (and therefore the top of ram too.) The stack size is simply calculated by pointer subtraction...

```c
extern char _estack;

unsigned int stack_size() {
    char tos;

    return &_estack - &tos;
}
```

> ** The `_estack` "variable" should never by read from or written to, it does not exist; it's location is the first byte OUTSIDE of the ram. **

Likewise, finding the end of the heap is simplified by access to a variable, `__brkval`, this time created by the heap memory manage. Space between the heap and stack can again be calculated with pointer subtraction. Note this time we are dealing with a real variable, a pointer. The address that `__brkval` points to (as with `_estack`) is the first byte outside the heap; this is the same principle as a C++ end() iterator, being one past the last.

```c
extern char* __brkval;

unsigned int unallocated_ram() {
    char tos;

    return &tos - __brkval;
}
```

> ** `__brkval` is a private variable of the heap memory manger; do NOT change it's value. **

Other heap information (used, free & total) are provided by the non-standard header `&lt;malloc.h&gt;`. Althought non-standard, it seem to still be quite widely supported (except on Windows.)

Extended information provided by RamMonitor requires initialization and run() method calls. These methods put markers in the unallocated memory at 1kb (default value) intervals and then monitors them. As the stack grows it corrupts these markers, allowing RamMonitor to see what stack other parts of the progam are using. Simple hey? The `initialize()` method creates the markers...

```c
const char MARKER  = 'M';

char* firstmarker;

void mark_memory() {
    char* memloc = &_estack;

    while(memloc >= &memloc)
        memloc -= 1024;

    firstmarker = memloc;

    while(memloc >= __brkval) {
        *memloc = MARKER;
        memloc -= 1024;
    }
}
```

We start by initializing memloc to the base of the stack; this ensures our markers are alligned with the stack. The first while skips memloc over the current stack (remember the address of our local variable is the top of stack address.) We then record the location of our first marker, we'll use this later. The final while then puts the markers evey 1k until it reaches the top of the heap. Internally, RamMonitor uses a quick bit of pointer math in place of the first while and it uses a 32-bit marker.

The run() method monitors the markers with a simple loop. It starts at firstmarker and steps back if it's been corrupted, until it reaches the top of the heap.

```c
void check_markers() {
    while((*firstmarker != MARKER) && (firstmarker >= __brkval))
        firstmarker -= 1024;
}
```
 
With firstmarker, simple pointer subtraction provides stack total and free values; and an adjusted unallocated memory state, that is more usefull and is used to provide RamMonitors memory warnings.

```c
unsigned int stack_total() {
    return &_estack - firstmarker;
}

int stack_free() {
    char tos;

    return &tos - firstmarker;
}

int adjusted_unallocated() {
    return firstmarker - __brkval;
}
```

Notice that both `stack_free()` and `adjusted_unallocated()` return signed values. A negitive result is return once the heap and stack have crash.