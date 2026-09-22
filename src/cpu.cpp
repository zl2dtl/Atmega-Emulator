#include "avr/cpu.hpp"
#include <iostream>

namespace avr{

constexpr uint8_t SREG_Z = 1 << 1;
constexpr uint8_t SREG_N = 1 << 2;
constexpr uint8_t SREG_V = 1 << 3;
constexpr uint8_t SREG_S = 1 << 4; 
 
    CPU::CPU(){
        reset();
    }

void CPU::reset(){

    registers_.fill(0);

    pc_ = 0;
    sp_ = 0x08FF;
    sreg_ = 0;
}

void CPU::step(){
    uint16_t opcode =
        flash_[pc_ * 2] | 
        (static_cast<uint16_t>(flash_[pc_ * 2 + 1]) << 8);

    //NOP: no operation
    if(opcode == 0x0000){
        pc_++;
        return;
    }

    // LDI: Load Immediate
    if ((opcode & 0xF000) == 0xE000){
        uint8_t d = (opcode >> 4) & 0x0F;

        uint8_t K =
        ((opcode >> 4) & 0xF0) |
        (opcode & 0x0F);

        writeRegister(16 + d, K);

        pc_++;
        return;
    }

    // MOV: Copy register
if ((opcode & 0xFC00) == 0x2C00)
{
    uint8_t d =
        (opcode >> 4) & 0x1F;

    uint8_t r =
        (opcode & 0x0F) |
        ((opcode >> 5) & 0x10);

    writeRegister(d, readRegister(r));

    pc_++;
    return;
}
    
// INC: Increment Register
if ((opcode & 0xFE0F) == 0x9403)
{
    uint8_t d =
        (opcode >> 4) & 0x1F;

    uint8_t oldValue = readRegister(d);
    uint8_t result = oldValue + 1;

    writeRegister(d, result);

    // Z flag
    if (result == 0)
        sreg_ |= SREG_Z;
    else
        sreg_ &= ~SREG_Z;

    // N flag
    if (result & 0x80)
        sreg_ |= SREG_N;
    else
        sreg_ &= ~SREG_N;

    // V flag: positive to negative
    if (oldValue == 0x7F)
        sreg_ |= SREG_V;
    else
        sreg_ &= ~SREG_V;

    // S = N XOR V
    if (((sreg_ & SREG_N) != 0) ^
        ((sreg_ & SREG_V) != 0))
        sreg_ |= SREG_S;
    else
        sreg_ &= ~SREG_S;

    pc_++;
    return;
}

// DEC: Decrement Register
if ((opcode & 0xFE0F) == 0x940A)
{
    uint8_t d =
        (opcode >> 4) & 0x1F;

    uint8_t oldValue = readRegister(d);
    uint8_t result = oldValue - 1;

    writeRegister(d, result);

    // Z flag
    if (result == 0)
        sreg_ |= SREG_Z;
    else
        sreg_ &= ~SREG_Z;

    // N flag
    if (result & 0x80)
        sreg_ |= SREG_N;
    else
        sreg_ &= ~SREG_N;

    // V flag: negative to positive
    if (oldValue == 0x80)
        sreg_ |= SREG_V;
    else
        sreg_ &= ~SREG_V;

    // S = N XOR V
    if (((sreg_ & SREG_N) != 0) ^
        ((sreg_ & SREG_V) != 0))
        sreg_ |= SREG_S;
    else
        sreg_ &= ~SREG_S;

    pc_++;
    return;
}

// ADD: Add two registers
if ((opcode & 0xFC00) == 0x0C00)
{
    uint8_t d =
        (opcode >> 4) & 0x1F;

    uint8_t r =
        (opcode & 0x0F) |
        ((opcode >> 5) & 0x10);

    uint8_t Rd = readRegister(d);
    uint8_t Rr = readRegister(r);
    uint8_t result = Rd + Rr;

    writeRegister(d, result);

    // H: Half Carry
    if (((Rd & 0x0F) + (Rr & 0x0F)) & 0x10)
        sreg_ |= (1 << 5);
    else
        sreg_ &= ~(1 << 5);

    // V: Two's-complement overflow
    if ((~(Rd ^ Rr) & (Rd ^ result) & 0x80) != 0)
        sreg_ |= SREG_V;
    else
        sreg_ &= ~SREG_V;

    // N: Negative
    if (result & 0x80)
        sreg_ |= SREG_N;
    else
        sreg_ &= ~SREG_N;

    // S: N XOR V
    if (((sreg_ & SREG_N) != 0) ^
        ((sreg_ & SREG_V) != 0))
        sreg_ |= SREG_S;
    else
        sreg_ &= ~SREG_S;

    // Z: Zero
    if (result == 0)
        sreg_ |= SREG_Z;
    else
        sreg_ &= ~SREG_Z;

    // C: Carry
    if ((static_cast<uint16_t>(Rd) + Rr) & 0x100)
        sreg_ |= 1 << 0;
    else
        sreg_ &= ~(1 << 0);

    pc_++;
    return;
}

// SUB: Subtract one register from another
if ((opcode & 0xFC00) == 0x1800)
{
    uint8_t d =
        (opcode >> 4) & 0x1F;

    uint8_t r =
        (opcode & 0x0F) |
        ((opcode >> 5) & 0x10);

    uint8_t Rd = readRegister(d);
    uint8_t Rr = readRegister(r);

    uint8_t result = Rd - Rr;

    writeRegister(d, result);

    // H: Half Carry / borrow
    if ((Rd & 0x0F) < (Rr & 0x0F))
        sreg_ |= (1 << 5);
    else
        sreg_ &= ~(1 << 5);

    // V: Two's-complement overflow
    if (((Rd ^ Rr) & (Rd ^ result) & 0x80) != 0)
        sreg_ |= SREG_V;
    else
        sreg_ &= ~SREG_V;

    // N: Negative
    if (result & 0x80)
        sreg_ |= SREG_N;
    else
        sreg_ &= ~SREG_N;

    // S: N XOR V
    if (((sreg_ & SREG_N) != 0) ^
        ((sreg_ & SREG_V) != 0))
        sreg_ |= SREG_S;
    else
        sreg_ &= ~SREG_S;

    // Z: Zero
    if (result == 0)
        sreg_ |= SREG_Z;
    else
        sreg_ &= ~SREG_Z;

    // C: Carry / borrow
    if (Rd < Rr)
        sreg_ |= (1 << 0);
    else
        sreg_ &= ~(1 << 0);

    pc_++;
    return;
}

// ADC: Add two registers with Carry
if ((opcode & 0xFC00) == 0x1C00)
{
    uint8_t d =
        (opcode >> 4) & 0x1F;

    uint8_t r =
        (opcode & 0x0F) |
        ((opcode >> 5) & 0x10);

    uint8_t Rd = readRegister(d);
    uint8_t Rr = readRegister(r);

    uint8_t carry =
        (sreg_ & (1 << 0)) ? 1 : 0;

    uint8_t result = Rd + Rr + carry;

    writeRegister(d, result);

    // H: Half Carry
    if (((Rd & 0x0F) + (Rr & 0x0F) + carry) & 0x10)
        sreg_ |= (1 << 5);
    else
        sreg_ &= ~(1 << 5);

    // V: Two's-complement overflow
    if ((~(Rd ^ Rr) & (Rd ^ result) & 0x80) != 0)
        sreg_ |= SREG_V;
    else
        sreg_ &= ~SREG_V;

    // N: Negative
    if (result & 0x80)
        sreg_ |= SREG_N;
    else
        sreg_ &= ~SREG_N;

    // S: N XOR V
    if (((sreg_ & SREG_N) != 0) ^
        ((sreg_ & SREG_V) != 0))
        sreg_ |= SREG_S;
    else
        sreg_ &= ~SREG_S;

    // Z: Zero
    if (result == 0)
        sreg_ |= SREG_Z;
    else
        sreg_ &= ~SREG_Z;

    // C: Carry
    if ((static_cast<uint16_t>(Rd) + Rr + carry) & 0x100)
        sreg_ |= (1 << 0);
    else
        sreg_ &= ~(1 << 0);

    pc_++;
    return;
}

// SBC: Subtract register and Carry
if ((opcode & 0xFC00) == 0x0800)
{
    uint8_t d =
        (opcode >> 4) & 0x1F;

    uint8_t r =
        (opcode & 0x0F) |
        ((opcode >> 5) & 0x10);

    uint8_t Rd = readRegister(d);
    uint8_t Rr = readRegister(r);

    uint8_t carry =
        (sreg_ & (1 << 0)) ? 1 : 0;

    uint8_t result = Rd - Rr - carry;

    std::cout << "SBC: Rd=" << static_cast<int>(Rd)
          << " Rr=" << static_cast<int>(Rr)
          << " C=" << static_cast<int>(carry)
          << " result=" << static_cast<int>(result)
          << '\n';

    writeRegister(d, result);

    // H: Half Carry / borrow
    if ((Rd & 0x0F) < ((Rr & 0x0F) + carry))
        sreg_ |= (1 << 5);
    else
        sreg_ &= ~(1 << 5);

    // V: Two's-complement overflow
    if (((Rd ^ Rr) & (Rd ^ result) & 0x80) != 0)
        sreg_ |= SREG_V;
    else
        sreg_ &= ~SREG_V;

    // N: Negative
    if (result & 0x80)
        sreg_ |= SREG_N;
    else
        sreg_ &= ~SREG_N;

    // S: N XOR V
    if (((sreg_ & SREG_N) != 0) ^
        ((sreg_ & SREG_V) != 0))
        sreg_ |= SREG_S;
    else
        sreg_ &= ~SREG_S;

    // Z: Zero
    // SBC has cumulative Z
    if (result == 0 && (sreg_ & SREG_Z))
        sreg_ |= SREG_Z;
    else
        sreg_ &= ~SREG_Z;
    
    // C: Carry / borrow
    if (static_cast<uint16_t>(Rr) + carry > Rd)
        sreg_ |= (1 << 0);
    else
        sreg_ &= ~(1 << 0);

    pc_++;
    return;
}

    }

uint8_t CPU::readRegister(uint8_t index) const{
        return registers_.at(index);
    }

void CPU::writeRegister(uint8_t index, uint8_t value){
        registers_.at(index) = value;
    }

uint16_t CPU::programCounter() const{
        return pc_;
}

uint16_t CPU::stackPointer() const{
        return sp_;
}

uint8_t CPU::statusRegister() const{
        return sreg_;
}

void CPU::writeFlash(uint16_t address, uint8_t value)
{
    flash_.at(address) = value;
}

}

