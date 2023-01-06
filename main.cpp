#include <iostream>
#include <cstring>

struct Memory {
private:
    static constexpr __uint32_t MEMORY_SIZE = 0x10000;
    __uint8_t memory[MEMORY_SIZE] { };

public:
    /**
     * \brief Assigns memory to 0 in other words just resets it
     */
    auto mem_reset() -> void {
        std::memset(memory, 0x0, sizeof(memory));
    }
};

class CPU {
public:
    /* Registers */
    __uint8_t  AC;      //Accumulator
    __uint8_t  SP;      //Stack Pointer
    __uint8_t  X;       //X Register
    __uint8_t  Y;       //Y Register
    __uint8_t  SR;      //Status Register [NV-BDIZC]
    __uint16_t PC;      //Program Counter

    /* Status Flags */
    __uint8_t N : 0x1;    //Negative
    __uint8_t V : 0x1;    //Overflow
    __uint8_t G : 0x1;    //Ignored
    __uint8_t B : 0x1;    //Break
    __uint8_t D : 0x1;    //Decimal
    __uint8_t I : 0x1;    //Interrupt (IRQ disable)
    __uint8_t Z : 0x1;    //Zero
    __uint8_t C : 0x1;    //Carry

    /**
     * \brief Resets 6502 processor with assigning program counter to certain address,
     *      assigning decimal flag to 0, and assigning memory to 0
     * @param mem is a instance of Memory object which will be used for resetting memory
     */
    auto reset(Memory mem) -> void {
        PC = 0xFFFC;
        __uint16_t reset_vector = (__uint16_t) read_memory(PC) |
                ((__uint16_t) read_memory(PC + 1) << 8);
        PC = reset_vector;
        D = 0x0;
        SP = 0xFF;
        mem.mem_reset();
    }

    static auto read_memory(uint16_t address) -> __uint8_t {
        // TODO: Add code to read a byte from memory
        return 0x0;
    }
};

auto main() -> int {
    Memory memory;
    CPU cpu { };
    cpu.reset(memory);
    return 0x0;
}
