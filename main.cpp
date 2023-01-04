#include <iostream>

auto main() -> int {
    constexpr __uint16_t op_nop   = 0x00;
    constexpr __uint16_t op_halt  = 0x01;
    constexpr __uint16_t op_alert = 0x02;

    using _tvm_instruction = __uint16_t;
    using _ptr_tvm_instruction = _tvm_instruction*;
    using _tvm_instruction_handler = void (*)();
    using _t_instruction_set [[maybe_unused]] = _tvm_instruction_handler[0x3];

    _tvm_instruction data[0x3] {op_nop, op_halt, op_alert};
    _ptr_tvm_instruction _program_counter = data;

    bool is_running = true;

    std::cout << "Starting virtual program" << std::endl;
    while (is_running) {
        switch (*_program_counter) {
            case op_nop : {
                std::cout << "nop" << std::endl;
                break;
            }

            case op_alert : {
                std::cout << "alert" << std::endl;
                break;
            }

            case op_halt : {
                std::cout << "halt" << std::endl;
                is_running = false;
                break;
            }

            default : std::cout << "Something went wrong" << std::endl;
        }
        ++_program_counter;
    }
    std::cout << "Ending virtual program" << std::endl;

    return 0x0;
}
