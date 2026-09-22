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

void testMov()
{
    avr::CPU cpu;

    // LDI R16, 0x42
    cpu.writeFlash(0, 0x02);
    cpu.writeFlash(1, 0xE4);

    cpu.step();

    // MOV R17, R16
    cpu.writeFlash(2, 0x10);
    cpu.writeFlash(3, 0x2F);

    cpu.step();

    if (cpu.readRegister(17) != 0x42)
    {
        std::cerr << "MOV test failed: R17 is not 0x42\n";
        std::exit(1);
    }

    if (cpu.programCounter() != 2)
    {
        std::cerr << "MOV test failed: PC is not 2\n";
        std::exit(1);
    }

    std::cout << "MOV test passed\n";
}

int main() {

    testLdi();
    testMov();

    return 0;
}