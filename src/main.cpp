#include <iostream>

#include "avr/cpu.hpp"


int main() {

    avr::CPU cpu;

    std::cout << "ATmega 328P Emulator\n";
    std::cout << "--------------------\n";\

    // LDI R20, 0x42
cpu.writeFlash(0, 0x42);
cpu.writeFlash(1, 0xE4);

cpu.step();

std::cout << "PC after LDI: "
          << cpu.programCounter()
          << '\n';

std::cout << "R20 after LDI: 0x"
          << std::hex
          << static_cast<int>(cpu.readRegister(20))
          << '\n';

return 0;

}
