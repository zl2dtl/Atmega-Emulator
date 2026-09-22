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

void testDec()
{
    
    avr::CPU cpu;

    // LDI R16, 0x42
    cpu.writeFlash(0, 0x02);
    cpu.writeFlash(1, 0xE4);

    cpu.step();

    // DEC R16
    cpu.writeFlash(2, 0x0A);
    cpu.writeFlash(3, 0x95);

    cpu.step();

    if (cpu.readRegister(16) != 0x41)
    {
        std::cerr << "DEC test failed: R16 is not 0x41\n";
        std::exit(1);
    }

    if (cpu.programCounter() != 2)
    {
        std::cerr << "DEC test failed: PC is not 2\n";
        std::exit(1);
    }

    std::cout << "DEC test passed\n";
}

bool flagSet(uint8_t sreg, uint8_t flag)
{
    return (sreg & flag) != 0;
}
void testDecFlags()
{
    avr::CPU cpu;

    // LDI R16, 0x80
    cpu.writeFlash(0, 0x00);
    cpu.writeFlash(1, 0xE8);

    cpu.step();

    // DEC R16
    cpu.writeFlash(2, 0x0A);
    cpu.writeFlash(3, 0x95);

    cpu.step();

    if (cpu.readRegister(16) != 0x7F)
    {
        std::cerr << "DEC flags test failed: R16 is not 0x7F\n";
        std::exit(1);
    }

    uint8_t sreg = cpu.statusRegister();

    if (!flagSet(sreg, 1 << 3)) // V
    {
        std::cerr << "DEC flags test failed: V is not set\n";
        std::exit(1);
    }

    if (!flagSet(sreg, 1 << 4)) // S
    {
        std::cerr << "DEC flags test failed: S is not set\n";
        std::exit(1);
    }

    if (flagSet(sreg, 1 << 2)) // N
    {
        std::cerr << "DEC flags test failed: N should be clear\n";
        std::exit(1);
    }

    std::cout << "DEC flag test passed\n";
}

void testDecNegative()
{
    avr::CPU cpu;

    // LDI R16, 0x00
    cpu.writeFlash(0, 0x00);
    cpu.writeFlash(1, 0xE0);

    cpu.step();

    // DEC R16
    cpu.writeFlash(2, 0x0A);
    cpu.writeFlash(3, 0x95);

    cpu.step();

    if (cpu.readRegister(16) != 0xFF)
    {
        std::cerr << "DEC negative test failed: R16 is not 0xFF\n";
        std::exit(1);
    }

    uint8_t sreg = cpu.statusRegister();

    if (!flagSet(sreg, 1 << 2))
    {
        std::cerr << "DEC negative test failed: N is not set\n";
        std::exit(1);
    }

    if (!flagSet(sreg, 1 << 4))
    {
        std::cerr << "DEC negative test failed: S is not set\n";
        std::exit(1);
    }

    if (flagSet(sreg, 1 << 3))
    {
        std::cerr << "DEC negative test failed: V should be clear\n";
        std::exit(1);
    }

    std::cout << "DEC negative test passed\n";
}

int main() {

    testLdi();
    testMov();
    testInc();
    testDec();
    testDecFlags();
    testDecNegative();
    return 0;
}   