Virtual Machine Emulator

This program is a simple emulator for a virtual machine that executes a set of machine code instructions.
The program reads a list of machine code instructions from a constant array defined in the source code. 
It then executes these instructions one by one, calling the appropriate instruction handler function for each opcode.

The program supports the following opcodes:

    Nop: No operation. This instruction does nothing.
    Halt: Halt execution. This instruction stops the virtual machine.
    Alert: Alert. This instruction prints a message to the console.
