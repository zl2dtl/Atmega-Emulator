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

}

