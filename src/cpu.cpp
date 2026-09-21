#include "avr/cpu.hpp"

namespace avr{
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

