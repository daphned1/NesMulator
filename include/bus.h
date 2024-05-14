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

    uint8_t controller[2]; // stores the instantaneous state of both controllers 

public:
    // system interface
    void insertCart(const std::shared_ptr<cartridge>& cartridge);
    void reset();
    void clock();

private:
    uint32_t systemClcCounter = 0;

    uint8_t ctrller_state[2]; //snapshot of input

    // dma
    uint8_t dmaPage = 0x00;
    uint8_t dmaAddr = 0x00;
    uint8_t dmaData = 0x00; // represent data in transit

    bool dmaTransfer = false;
    bool dmaDummy = true;
};