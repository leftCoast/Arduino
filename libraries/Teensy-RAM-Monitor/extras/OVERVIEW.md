# RamMonitor Overview

RamMonitor has been designed to be a single instance object. This means that a single RamMonitor object is create and initialized at a global level. Any code that needs access the RamMonitor object should use an extern declaration.

## Configuration
Two private static constants can be adjusted to suit your needs.

uint16_t STACKALLOCATION
This defines the size of the emulated stack allocations. This value is in bytes and must be dividable by 4.

uint16_t LOWMEM
The low memory warning is triggered when unallocated memory is less than this value. Again this value is in bytes.

## Raw Memory Information
These methods do not require initialization; they provide raw, unadjusted information.

int32_t unallocated() const
Using the current stack size, calculates unallocated memory. This is the memory between the top of the stack (tos) and the top of heap. This function can return a negitive result; if this happens the stack is currenty overlapping the heap!

uint32_t stack_used() const
Returns the current stack size. Other stack details are provided by extended methods.

uint32_t heap_total() const
uint32_t heap_used() const
uint32_t heap_free() const
These three methods return the current heap status. It may be of note that the non-standard `&lt;malloc.h&gt;` is used to retrieve these values.

int32_t free() const
Returns the amount of free memory. This is calculated with the current stack size and free heap.

uint32_t total() const
Returns the total amount of ram.

## Extended Memory Information
The extended methods require that the RamMonitor is initialized and the run() method called frequently. Information returned takes in to account historical stack usage and "reserves" memory for future similar usage.

uint32_t stack_total()
This returns the total stack size, using historical stack usage.

int32_t stack_free()
Calculates the possible stack usage before more memory is "allocated" to the stack total. This function can return a negitive result, if the stack and heap current overlap!

int32_t adj_unallocd()
Returns the amount of unallocated ram, reserving memory for the stack. A negitive result will be returned if the stack and heap have possibly crashed.

int32_t adj_free()
Total free memory; calculated with free heap and unallocated memory. This method takes into account stack allocation.

## Memory Warnings
These extended methods return warnings about the memory status. They require initialization and freqent run() method calls.

bool warning_lowmem() const
This method return true if the unallocated memory goes below the level set by the LOWMEM constant. This function takes in to account stack allocation.

bool warning_crash() const
The crash warning method returns true when a heap and stack crash has occured.

## Core Methods
void initialize()
Enables stack monitoring and the extended methods.

void run()
Monitors the stack and the adjusted unallocated memory. This method updates the memory warnings.