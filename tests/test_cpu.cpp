#include <iostream>
#include <cstdlib>
#include <cassert>

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
void testAdd()
{
    avr::CPU cpu;

    // LDI R16, 0x20
    cpu.writeFlash(0, 0x00);
    cpu.writeFlash(1, 0xE2);

    cpu.step();

    // LDI R17, 0x22
    cpu.writeFlash(2, 0x12);
    cpu.writeFlash(3, 0xE2);

    cpu.step();

    // ADD R16, R17
    // Opcode: 0x0F01
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x0F);

    cpu.step();

    if (cpu.readRegister(16) != 0x42)
    {
        std::cerr << "ADD test failed: R16 is not 0x42\n";
        std::exit(1);
    }

    if (cpu.readRegister(17) != 0x22)
    {
        std::cerr << "ADD test failed: R17 was modified\n";
        std::exit(1);
    }

    if (cpu.programCounter() != 3)
    {
        std::cerr << "ADD test failed: PC is not 3\n";
        std::exit(1);
    }

    std::cout << "ADD test passed\n";
}


void testAddFlags()
{
    avr::CPU cpu;

    // LDI R16, 0xFF
    cpu.writeFlash(0, 0x0F);
    cpu.writeFlash(1, 0xEF);

    cpu.step();

    // LDI R17, 0x01
    cpu.writeFlash(2, 0x11);
    cpu.writeFlash(3, 0xE0);

    cpu.step();

    // ADD R16, R17
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x0F);

    cpu.step();

    uint8_t sreg = cpu.statusRegister();


    if (cpu.readRegister(16) != 0x00)
    {
        std::cerr << "ADD flag test failed: R16 is not 0x00\n";
        std::exit(1);
    }

    if (!(sreg & (1 << 5))) // H
    {
        std::cerr << "ADD flag test failed: H is not set\n";
        std::exit(1);
    }

    if (!(sreg & (1 << 1))) // Z
    {
        std::cerr << "ADD flag test failed: Z is not set\n";
        std::exit(1);
    }

    if (!(sreg & (1 << 0))) // C
    {
        std::cerr << "ADD flag test failed: C is not set\n";
        std::exit(1);
    }

    if (sreg & (1 << 3)) // V
    {
        std::cerr << "ADD flag test failed: V should be clear\n";
        std::exit(1);
    }

    if (sreg & (1 << 2)) // N
    {
        std::cerr << "ADD flag test failed: N should be clear\n";
        std::exit(1);
    }

    if (sreg & (1 << 4)) // S
    {
        std::cerr << "ADD flag test failed: S should be clear\n";
        std::exit(1);
    }

    std::cout << "ADD flag test passed\n";
}

void testAddOverflow()
{
    avr::CPU cpu;

    // LDI R16, 0x7F
    cpu.writeFlash(0, 0x0F);
    cpu.writeFlash(1, 0xE7);

    cpu.step();

    // LDI R17, 0x01
    cpu.writeFlash(2, 0x11);
    cpu.writeFlash(3, 0xE0);

    cpu.step();

    // ADD R16, R17
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x0F);

    cpu.step();

    if (cpu.readRegister(16) != 0x80)
    {
        std::cerr << "ADD overflow test failed: R16 is not 0x80\n";
        std::exit(1);
    }

    uint8_t sreg = cpu.statusRegister();

    // H should be set
    if (!(sreg & (1 << 5)))
    {
        std::cerr << "ADD overflow test failed: H is not set\n";
        std::exit(1);
    }

    // V should be set
    if (!(sreg & (1 << 3)))
    {
        std::cerr << "ADD overflow test failed: V is not set\n";
        std::exit(1);
    }

    // N should be set
    if (!(sreg & (1 << 2)))
    {
        std::cerr << "ADD overflow test failed: N is not set\n";
        std::exit(1);
    }

    // S = N XOR V, therefore S should be clear
    if (sreg & (1 << 4))
    {
        std::cerr << "ADD overflow test failed: S should be clear\n";
        std::exit(1);
    }

    // Z should be clear
    if (sreg & (1 << 1))
    {
        std::cerr << "ADD overflow test failed: Z should be clear\n";
        std::exit(1);
    }

    // C should be clear
    if (sreg & (1 << 0))
    {
        std::cerr << "ADD overflow test failed: C should be clear\n";
        std::exit(1);
    }

    std::cout << "ADD overflow test passed\n";
}

void testSub()
{
    avr::CPU cpu;

    // LDI R16, 0x42
    cpu.writeFlash(0, 0x02);
    cpu.writeFlash(1, 0xE4);

    cpu.step();

    // LDI R17, 0x20
    cpu.writeFlash(2, 0x10);
    cpu.writeFlash(3, 0xE2);

    cpu.step();

    // SUB R16, R17
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x1B);

    cpu.step();

    if (cpu.readRegister(16) != 0x22)
    {
        std::cerr << "SUB test failed: R16 is not 0x22\n";
        std::exit(1);
    }

    if (cpu.readRegister(17) != 0x20)
    {
        std::cerr << "SUB test failed: R17 was modified\n";
        std::exit(1);
    }

    if (cpu.programCounter() != 3)
    {
        std::cerr << "SUB test failed: PC is not 3\n";
        std::exit(1);
    }

    std::cout << "SUB test passed\n";
}

void testSubOverflow()
{
    avr::CPU cpu;

    // LDI R16, 0x80
    cpu.writeFlash(0, 0x00);
    cpu.writeFlash(1, 0xE8);

    // LDI R17, 0x01
    cpu.writeFlash(2, 0x11);
    cpu.writeFlash(3, 0xE0);

    // SUB R16, R17
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x1B);

    cpu.step();
    cpu.step();
    cpu.step();

    assert(cpu.readRegister(16) == 0x7F);

    uint8_t sreg = cpu.statusRegister();

    // H = 1
    assert((sreg & (1 << 5)) != 0);

    // V = 1
    assert((sreg & (1 << 3)) != 0);

    // N = 0
    assert((sreg & (1 << 2)) == 0);

    // S = 1
    assert((sreg & (1 << 4)) != 0);

    // Z = 0
    assert((sreg & (1 << 1)) == 0);

    // C = 0
    assert((sreg & (1 << 0)) == 0);

    assert(cpu.programCounter() == 3);
}

void testAdc()
{
    avr::CPU cpu;

    // LDI R16, 0xFF
    cpu.writeFlash(0, 0xFF);
    cpu.writeFlash(1, 0xEF);

    // LDI R17, 0x01
    cpu.writeFlash(2, 0x11);
    cpu.writeFlash(3, 0xE0);

    // ADD R16, R17
    // 0xFF + 0x01 = 0x00, Carry = 1
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x0F);

    // ADC R16, R17
    // 0x00 + 0x01 + Carry = 0x02
    cpu.writeFlash(6, 0x01);
    cpu.writeFlash(7, 0x1F);

    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();

    assert(cpu.readRegister(16) == 0x02);
    assert(cpu.readRegister(17) == 0x01);
    assert(cpu.programCounter() == 4);
}

void testAdcOverflow()
{
    avr::CPU cpu;

    // LDI R16, 0x7F
    cpu.writeFlash(0, 0x0F);
    cpu.writeFlash(1, 0xE7);

    // LDI R17, 0x00
    cpu.writeFlash(2, 0x10);
    cpu.writeFlash(3, 0xE0);

    // Set Carry using:
    // LDI R18, 0xFF
    cpu.writeFlash(4, 0x2F);
    cpu.writeFlash(5, 0xEF);

    // LDI R19, 0x01
    cpu.writeFlash(6, 0x31);
    cpu.writeFlash(7, 0xE0);

    // ADD R18, R19
    // 0xFF + 0x01 = 0x00, C = 1
    cpu.writeFlash(8, 0x23);
    cpu.writeFlash(9, 0x0F);

    // ADC R16, R17
    // 0x7F + 0x00 + 1 = 0x80
    cpu.writeFlash(10, 0x01);
    cpu.writeFlash(11, 0x1F);

    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();

    assert(cpu.readRegister(16) == 0x80);

    uint8_t sreg = cpu.statusRegister();

    // H = 1
    assert((sreg & (1 << 5)) != 0);

    // V = 1
    assert((sreg & (1 << 3)) != 0);

    // N = 1
    assert((sreg & (1 << 2)) != 0);

    // S = 0
    assert((sreg & (1 << 4)) == 0);

    // Z = 0
    assert((sreg & (1 << 1)) == 0);

    // C = 0
    assert((sreg & (1 << 0)) == 0);
}

void testSbc()
{
    avr::CPU cpu;

    // LDI R16, 0x42
    cpu.writeFlash(0, 0x02);
    cpu.writeFlash(1, 0xE4);

    // LDI R17, 0x20
    cpu.writeFlash(2, 0x10);
    cpu.writeFlash(3, 0xE2);

    // LDI R18, 0xFF
    cpu.writeFlash(4, 0x2F);
    cpu.writeFlash(5, 0xEF);

    // LDI R19, 0x01
    cpu.writeFlash(6, 0x31);
    cpu.writeFlash(7, 0xE0);

    // ADD R18, R19
    // 0xFF + 0x01 = 0x00, C = 1
    cpu.writeFlash(8, 0x23);
    cpu.writeFlash(9, 0x0F);

    // SBC R16, R17
    // 0x42 - 0x20 - 1 = 0x21
    cpu.writeFlash(10, 0x01);
    cpu.writeFlash(11, 0x0B);

    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();

    assert(cpu.readRegister(16) == 0x21);
    assert(cpu.readRegister(17) == 0x20);
    assert(cpu.programCounter() == 6);
}

void testSbcZeroFlag()
{
    avr::CPU cpu;

    // LDI R16, 0x42
    cpu.writeFlash(0, 0x02);
    cpu.writeFlash(1, 0xE4);

    // LDI R17, 0x42
    cpu.writeFlash(2, 0x12);
    cpu.writeFlash(3, 0xE4);

    // SUB R16, R17
    // 0x42 - 0x42 = 0x00
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x1B);

    // LDI R18, 0x20
    cpu.writeFlash(6, 0x20);
    cpu.writeFlash(7, 0xE2);

    // LDI R19, 0x20
    cpu.writeFlash(8, 0x30);
    cpu.writeFlash(9, 0xE2);

    // SBC R18, R19
    // 0x20 - 0x20 - 0 = 0x00
    cpu.writeFlash(10, 0x23);
    cpu.writeFlash(11, 0x0B);

    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();

    assert(cpu.readRegister(16) == 0x00);
    assert(cpu.readRegister(18) == 0x00);

    // Z should remain set
    assert((cpu.statusRegister() & (1 << 1)) != 0);
}

void testSbcZeroFlagCumulative()
{
    avr::CPU cpu;

    // LDI R16, 0x41
    cpu.writeFlash(0, 0x02);
    cpu.writeFlash(1, 0xE4);

    // LDI R17, 0x42
    cpu.writeFlash(2, 0x01);
    cpu.writeFlash(3, 0xE4);

    // SUB R16, R17
    // 0x41 - 0x42 = 0xFF
    // Z = 0
    cpu.writeFlash(4, 0x01);
    cpu.writeFlash(5, 0x1B);

    // LDI R18, 0x20
    cpu.writeFlash(6, 0x20);
    cpu.writeFlash(7, 0xE2);

    // LDI R19, 0x20
    cpu.writeFlash(8, 0x30);
    cpu.writeFlash(9, 0xE2);

    // SBC R18, R19
    // 0x20 - 0x20 - 0 = 0x00
    cpu.writeFlash(10, 0x23);
    cpu.writeFlash(11, 0x0B);

    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();

    assert(cpu.readRegister(18) == 0x00);

    // Z must remain clear because it was already clear
    assert((cpu.statusRegister() & (1 << 1)) == 0);
}

int main() {

    testLdi();
    testMov();
    testInc();
    testDec();
    testDecFlags();
    testDecNegative();
    testAdd();
    testAddFlags();
    testAddOverflow();
    testSub();
    testSubOverflow();
    testAdc();
    testAdcOverflow();
    testSbc();
    testSbcZeroFlag();
    testSbcZeroFlagCumulative();
    return 0;
}   