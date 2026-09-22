# Atmega-Emulator
# ATmega328P Emulator

A small, from-scratch, **rudimentary ATmega328P emulator written in C++**.

## Why am I making this?

I've spent a stupid amount of time working with AVRs and embedded electronics, so AVR is pretty much second nature to me. Most of the programming I normally do is also pretty low-level. I like Verilog, digital logic, ASICs, microcontrollers, reverse engineering, and generally figuring out what is happening underneath the software rather than building another app or web program.

So this project is a bit different for me.

I'm making it for the Hack Club YSWS (presumably made apparent by the fact that the only people reading this are those reviewing my request for some cool merch), but honestly that's only part of the reason. I've been interested in emulation and CPU architecture for a while, and at some point I had the thought that an AVR emulator should actually be something I could build myself.

I also saw a lot of the idea in a dream, and it seemed like it was probably possible in the dream, so I figured I'd better actually try it. Terry Davis would probably call this divine intuition.

The basic idea is pretty simple: take AVR machine code, decode it, execute it, and keep track of what the CPU is doing.

## How I'm doing it

I'm not using an existing AVR emulator or copying an implementation of the instruction set.

I'm working through the ATmega328P datasheet and the AVR Instruction Set Manual on Microchip website, looking at the opcode encoding, registers, flags and behaviour of each instruction, and then implementing that behaviour myself.

For each instruction, the general process is:

1. Find the instruction in the datasheet/manual.
2. Work out its binary opcode format.
3. Decode the relevant register/immediate fields.
4. Implement what the instruction actually does.
5. Implement the status flags it affects.
6. Write a test specifically for it.
7. Run the entire test suite and make sure I didn't break something else.

So far it's basically been one instruction at a time being written, but as I write more I have found that especially with the tests they become more similar and repetitive and it's a case of using the same code but changing the registers written to or the opcode used.

## What's implemented?

Currently the CPU supports:

* `NOP`
* `LDI`
* `MOV`
* `INC`
* `DEC`
* `ADD`
* `SUB`
* `ADC`
* `SBC`
* `AND`
* `OR`

The emulator currently has the beginnings of:

* 32 general-purpose 8-bit registers
* Program counter
* Stack pointer
* Status register (`SREG`)
* Flash memory
* Instruction fetching
* Opcode decoding
* Arithmetic and logical operations
* AVR status flag handling

(essentially in some rudimentary form every bit of an ATmega 328p)

The flags currently being implemented where appropriate are:

* `Z` - Zero
* `N` - Negative
* `V` - Two's complement overflow
* `S` - Sign
* `H` - Half Carry
* `C` - Carry

This is still very much a CPU core rather than a complete ATmega328P emulator. There are no proper GPIO peripherals, timers, ADC, USART, interrupts, EEPROM, etc. yet.

That's a future problem.

## Tests

I'm trying to test instructions individually rather than just throwing a bunch of opcodes at the emulator and hoping the output looks right.

Current tests include:

```cpp
testLdi();

testMov();

testInc();

testDec();
testDecFlags();
testDecNegative();

testAdd();
testAddFlags();
testAddOverflow();

testSub();
testSubOverflow();

testAdc();
testAdcOverflow();

testSbc();
testSbcZeroFlag();
testSbcZeroFlagCumulative();

testAnd();
```

Some tests just check that an instruction produces the expected register value, while others specifically check the AVR status flags and edge cases such as overflow and the cumulative zero behaviour of `SBC`.

The intention is for adding an instruction to be pretty straightforward: implement it, write a test for it, and add that test to the suite.

## Where this is going

The ATmega328P has 131 instructions, so I've got a fair bit of opcode writing left to do.

The eventual goal is to get far enough that I can take actual AVR code compiled with `avr-gcc`, load the resulting machine code into the emulator, and have it execute.

After that, I'd like to start adding the actual microcontroller side of things: SRAM, I/O registers, GPIO, timers, interrupts, USART, ADC and the other peripherals that make an ATmega328P an actual microcontroller rather than just an AVR CPU.

For now though, I'm quite happy just staring at instruction tables and writing increasingly large amounts of C++ to make eight-bit arithmetic happen.

**One opcode at a time.**

## References

Most of the actual instruction behaviour, opcode encoding and CPU architecture in this project comes from the official Microchip documentation:

* **Microchip ATmega328P Datasheet**
  https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

* **Microchip AVR Instruction Set Manual**
  https://ww1.microchip.com/downloads/en/devicedoc/AVR-Instruction-Set-Manual-DS40002198A.pdf

For understanding the AVR status register and how the individual flags behave, I also used:

* **SREG / AVR Status Register explanation**
  https://www.youtube.com/watch?v=LquFL2dlvDE&t=29s

The Microchip instruction manual is the main reference for implementing individual instructions. It gives the opcode format, instruction syntax, affected registers and status flags, and the behaviour of each instruction.

The ATmega328P datasheet is the main reference for the actual architecture of the chip, including the register file, program memory, SRAM, stack, I/O and peripherals.

The SREG video was particularly useful while working through the flag behaviour, since getting things like `Z`, `N`, `V`, `S`, `H` and `C` right is arguably half the fun of implementing AVR arithmetic.

This is a learning project and is not intended to ever be a production quality emulator, it's for a Hack Club project to win some prizes hopefully. It's mostly an excuse for me to read an AVR datasheet very closely and see how far I can take the idea.
