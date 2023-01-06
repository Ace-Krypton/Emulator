#include <iostream>
#include <cstdarg>

struct Memory {
private:
    static constexpr __uint32_t MEMORY_SIZE = 0x10000;
    __uint8_t memory[MEMORY_SIZE] { };

public:
    /**
     * \brief Assigns memory to 0 in other words just resets it
     */
    auto mem_reset() -> void {
        std::fill(std::begin(memory), std::end(memory), 0x0);
    }

    /**
     * \brief Overloads the '[]' operator for memory
     * @param index Program counter's value
     * @return memory[index] if the condition is met
     */
    auto operator[](std::size_t index) -> __uint8_t& {
        return (index >= MEMORY_SIZE) ? throw std::out_of_range("index out of bounds") : memory[index];
    }
};

class CPU {
public:
    /* Registers */
    [[maybe_unused]] __uint8_t  AC;      //Accumulator
    [[maybe_unused]] __uint8_t  SP;      //Stack Pointer
    [[maybe_unused]] __uint8_t  X;       //X Register
    [[maybe_unused]] __uint8_t  Y;       //Y Register
    [[maybe_unused]] __uint8_t  SR;      //Status Register [NV-BDIZC]
    [[maybe_unused]] __uint16_t PC;      //Program Counter

    /* Status Flags */
    [[maybe_unused]] __uint8_t N : 0x1;    //Negative
    [[maybe_unused]] __uint8_t V : 0x1;    //Overflow
    [[maybe_unused]] __uint8_t G : 0x1;    //Ignored
    [[maybe_unused]] __uint8_t B : 0x1;    //Break
    [[maybe_unused]] __uint8_t D : 0x1;    //Decimal
    [[maybe_unused]] __uint8_t I : 0x1;    //Interrupt (IRQ disable)
    [[maybe_unused]] __uint8_t Z : 0x1;    //Zero
    [[maybe_unused]] __uint8_t C : 0x1;    //Carry

    /**
     * \brief Resets 6502 processor with assigning program counter to certain address,
     *      assigning decimal flag to 0, and assigning memory to 0
     * @param mem Instance of Memory object which will be used for resetting memory
     */
    auto reset(Memory& mem) -> void {
        PC = 0xFFFC;
        D = 0x0;
        SP = 0xFF;
        mem.mem_reset();
    }

    /**
     * \brief Fatal error indicator
     * @param format Type
     * @param ... (Variadic function)
     */
    [[noreturn]] static auto fatal_error(char const * format, ...) -> void {
        fflush(stdout);
        va_list var_args;
        va_start(var_args, format);
        vfprintf(stderr, format, var_args);
        exit(0x1);
    }

    /**
     * \brief Fetches an instruction from memory
     * @param cycles Number of Cycles
     * @param memory Instance of Memory object for using memory array
     * @return Program counter's value in memory array
     */
    auto fetch(__uint32_t& cycles, Memory& memory) -> __uint8_t {
        __uint8_t instruction = memory[PC];
        PC++;
        cycles--;
        return instruction;
    }

    /**
     * \brief Executes the instructions from memory
     * @param cycles Number of Cycles
     * @param memory Instance of Memory object for using memory array
     */
    auto execute(__uint32_t cycles, Memory& memory) -> void {
        while (cycles > 0x0) {
            __uint8_t instructions = fetch(cycles, memory);

            switch (instructions) {
                /* Load Accumulator with Memory (LDA) */
                case 0xA5 : {   //Zero-page LDA
                    __uint8_t zero_page = fetch(cycles, memory);
                    AC = read_memory(cycles, zero_page ,memory);
                    zero_out();
                    break;
                }

                case 0xA9 : {   //Immediate LDA
                    __uint8_t byte = fetch(cycles, memory);
                    AC = byte;
                    zero_out();
                    break;
                }

                default : fatal_error("Error occurred");
            }
        }
    }

    /**
     * \brief Sets false to Z (Zero) flag, if accumulator is equals to 0
     *      also sets the N(Negative) flag, if the most significant bit (bit 7) of the accumulator is set
     */
    auto zero_out() -> void {
        Z = (AC == 0x0);
        N = (AC & 0x80) != 0x0;
    }

    /**
     * \brief Reads memory from address
     * @param address Memory address
     * @return Instruction
     */
    static auto read_memory(__uint32_t& cycles, __uint8_t address ,Memory& memory) -> __uint8_t {
        __uint8_t instruction = memory[address];
        cycles--;
        return instruction;
    }
};

auto main() -> int {
    Memory memory;
    CPU cpu { };
    cpu.reset(memory);

    memory[0xFFFC] = 0xA5;
    memory[0xFFFD] = 0x42;
    memory[0x0042] = 0x84;

    cpu.execute(0x3, memory);
    return 0x0;
}
