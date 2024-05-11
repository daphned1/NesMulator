#pragma once
#include "CPU6502.h"
#include <cstdint>
#include <array>


class Bus {
    public:
        Bus();
        ~Bus();

        // read write
        uint8_t read(uint16_t address, bool readOnly = false);
        void write(uint16_t address, uint8_t val);

        //device on bus
        CPU6502 cpu;

        std::array<uint8_t, 64 * 1024> ram;
};