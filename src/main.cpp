#include <iostream>

#include "avr/cpu.hpp"


int main() {

    avr::CPU cpu;

    std::cout << "ATmega 328P Emulator\n";
    std::cout << "--------------------\n";\

    std::cout << "Initial PC: "
              << cpu.programCounter()
              << '\n';

    cpu.writeFlash(0, 0x00);
    cpu.writeFlash(1, 0x00);

    cpu.step();


    std::cout << "PC after NOP: "
              << cpu.programCounter()
              << '\n';

    return 0;

}
