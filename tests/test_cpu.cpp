#include <iostream>
#include <cstdlib>

#include "avr/cpu.hpp"

void testLdi(){
    
    avr::CPU cpu;

    // LDI R20, 0x42
    cpu.writeFlash(0, 0x42);
    cpu.writeFlash(1, 0xE4);

    cpu.step();

    if (cpu.readRegister(20) != 0x42)
    {
        std::cerr << "LDI test failed: R20 is not 0x42\n";
        std::exit(1);
    }

    if (cpu.programCounter() != 1)
    {
        std::cerr << "LDI test failed: PC is not 1\n";
        std::exit(1);
    }

    std::cout << "LDI test passed\n";
}

int main() {
        
    testLdi();

    return 0;
}