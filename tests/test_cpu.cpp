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

void testInc()
{
    avr::CPU cpu;

    // LDI R16, 0x41
cpu.writeFlash(0, 0x01);
cpu.writeFlash(1, 0xE4);

cpu.step();

std::cout << "R16 after LDI: 0x"
          << std::hex
          << static_cast<int>(cpu.readRegister(16))
          << '\n';

std::cout << "PC after LDI: "
          << std::dec
          << cpu.programCounter()
          << '\n';

    // INC R16
    cpu.writeFlash(2, 0x03);
    cpu.writeFlash(3, 0x95);

std::cout << "PC before INC: "
          << cpu.programCounter()
          << '\n';
    cpu.step();

    if (cpu.readRegister(16) != 0x42)
    {
        std::cerr << "INC test failed: R16 is not 0x42\n";
        std::exit(1);
    }

    if (cpu.programCounter() != 2)
    {
        std::cerr << "INC test failed: PC is not 2\n";
        std::exit(1);
    }

    std::cout << "INC test passed\n";
}

int main() {

    testLdi();
    testMov();
    testInc();
    return 0;
}   