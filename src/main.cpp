#include <iostream>

#include "avr/cpu.hpp"


int main() {

    avr::CPU cpu;

    std::cout << "ATmega 328P Emulator\n";
    std::cout << "--------------------\n";\

    std::cout << "PC: "
              << cpu.programCounter()
              << '\n';

        std::cout << "SP: "
              << cpu.stackPointer()
              << '\n';

        std::cout << "SREG: "
              << static_cast<int>(cpu.statusRegister())
              << '\n';

    return 0;

}
