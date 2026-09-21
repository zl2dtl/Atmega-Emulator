#pragma once 

#include <array>
#include <cstdint>

namespace avr {

    class CPU{
        public:
            CPU();

            void reset();

            uint8_t readRegister(uint8_t index) const;
            void writeRegister(uint8_t index, uint8_t value);

            uint16_t programCounter() const;
            uint16_t stackPointer() const;
            uint8_t statusRegister() const;

        private:
            std::array<uint8_t, 32> registers_{};
            std::array<uint8_t, 32768> flash_{};

            uint16_t pc_ = 0;
            uint16_t sp_ = 0;
            uint8_t sreg_ = 0;

    };

}
