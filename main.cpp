#include <iostream>
#include <cstdarg>
#include <cstring>
#include <arpa/inet.h>

struct Memory {
private:
    static constexpr __uint32_t MEMORY_SIZE = 0x10000;
    __uint8_t memory[MEMORY_SIZE] { };

public:
    /**
     * \brief Assigns memory to 0 in other words just resets it
     */
    auto mem_reset() -> void {
        std::memset(memory, 0x0, MEMORY_SIZE);
    }

    /**
     * \brief Overloads the '[]' operator for memory
     * @param index Program counter's value
     * @return memory[index] if the condition is met
     */
    auto operator[](std::size_t index) -> __uint8_t& {
        return (index >= MEMORY_SIZE) ? throw std::out_of_range("index out of bounds") : memory[index];
    }

    /**
     * \brief Function takes a 16-bit value and writes it to a specified memory address
     * @param value 16-bit value to be written to memory
     * @param address Memory address where the value should be written
     * @param cycles Reference to a 32-bit integer that represents the number of cycles taken by the operation
     */
    auto write_two(__uint16_t value, __uint32_t address, __uint32_t& cycles) -> void {
        std::memcpy(memory + address, &value, sizeof(value));
        cycles -= 0x2;
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
     * \brief Resets the processor with assigning program counter to certain address,
     *      assigning decimal flag to 0, and assigning memory to 0
     * @param mem Instance of Memory object which will be used for resetting memory
     */
    auto reset(Memory& mem) -> void {
        PC = 0xFFFC;
        D  = 0x0;
        SP = 0xFF;
        mem.mem_reset();
    }

    /**
     * \brief Fatal error indicator
     * @param format Type
     * @param ... (Variadic function)
     */
    [[noreturn]] static auto fatal_error(char const * format, ...) -> void {
        std::fflush(stdout);
        std::va_list var_args;
        va_start(var_args, format);
        std::vfprintf(stderr, format, var_args);
        std::exit(0x1);
    }

    /**
     * \brief Fetches an instruction from memory for 8 bits (1 byte)
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
     * \brief Fetches an instruction from memory for 16 bits (2 bytes)
     * @param cycles Number of Cycles
     * @param memory Instance of Memory object for using memory array
     * @return Program counter's value in memory array and make sure that it will be in little endian
     */
    auto fetch_two(__uint32_t& cycles, Memory& memory) -> __uint16_t {
        __uint16_t instruction = memory[PC];
        PC++;
        cycles--;

        instruction |= (memory[PC] << 0x8);
        PC++;
        cycles -= 0x2;

        return ntohs(instruction);
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

                case 0xB5 : {   //Zero-page-X LDA
                    __uint8_t zero_page = fetch(cycles, memory);
                    zero_page += X;
                    cycles--;
                    AC = read_memory(cycles, zero_page ,memory);
                    zero_out();
                    break;
                }

                case 0xAD : {   //Absolute LDA
                    __uint16_t byte = fetch_two(cycles, memory);
                    AC = byte;
                    zero_out();
                    break;
                }

                case 0x20 : {   //JSR Jump To Subroutine
                    __uint16_t sub_address = fetch_two(cycles, memory);
                    memory.write_two(PC - 0x1, SP, cycles);
                    PC = sub_address;
                    SP++;
                    cycles--;
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
     *      also sets the N (Negative) flag, if the most significant bit (bit 7) of the accumulator is set
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
    static auto read_memory(__uint32_t& cycles, __uint8_t address, Memory& memory) -> __uint8_t {
        __uint8_t instruction = memory[address];
        cycles--;
        return instruction;
    }
};

#if something
auto main() -> int {
    /* --------------------------------- START TEST --------------------------------- */
    Memory memory;
    CPU cpu { };
    cpu.reset(memory);

    memory[0xFFFC] = 0x20;
    memory[0xFFFD] = 0x22;
    memory[0xFFFE] = 0x22;
    memory[0x2222] = 0xA9;
    memory[0x2223] = 0x34;

    cpu.execute(0x9, memory);
    /* ---------------------------------- END TEST ---------------------------------- */

    return 0x0;
}
#endif

#include <gtest/gtest.h>

TEST(MultiplicationTest, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}
