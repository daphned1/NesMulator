#pragma once
#include "CPU6502.h"
#include "ppu2C02.h"
#include "cartridge.h"
#include <cstdint>
#include <array>


class Bus {
public:
    Bus();
    ~Bus();

    // read write
    uint8_t cRead(uint16_t address, bool readOnly = false);
    void cWrite(uint16_t address, uint8_t val);

    //device on bus
    CPU6502 cpu;

    ppu2C02 ppu;

    uint8_t cRam[2048];

    // the cartridge
    std::shared_ptr<cartridge> cart;

public:
    // system interface
    void insertCart(const std::shared_ptr<cartridge>& cartridge);
    void reset();
    void clock();

private:
    uint32_t systemClcCounter = 0;
};