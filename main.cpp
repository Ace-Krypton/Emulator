#include <iostream>

struct CPU {
public:
    /* Registers */
    __uint8_t  AC;      //Accumulator
    __uint8_t  SP;      //Stack Pointer
    __uint8_t  X;       //X Register
    __uint8_t  Y;       //Y Register
    __uint8_t  SR;      //Status Register [NV-BDIZC]
    __uint16_t PC;      //Program Counter

    /* Status Flags */
    __uint8_t N : 1;    //Negative
    __uint8_t V : 1;    //Overflow
    __uint8_t G : 1;    //Ignored
    __uint8_t B : 1;    //Break
    __uint8_t D : 1;    //Decimal
    __uint8_t I : 1;    //Interrupt (IRQ disable)
    __uint8_t Z : 1;    //Zero
    __uint8_t C : 1;    //Carry
};

auto main() -> int {

    return 0x0;
}
