#include "ppu2C02.h"
#include "cartridge.h"

uint8_t ppu2C02::cRead(uint16_t address, bool readonly) {
    uint8_t data = 0x00;

    switch (address) {
    // control
    case 0x0000:
        break;
    // mask
    case 0x0001:
        break;
    // status
    case 0x0002:
        break;
    // OAM addr
    case 0x0003:
        break;
    // OAM data
    case 0x0004:
        break;
    // scroll
    case 0x0005:
        break;
    // PPU addr
    case 0x0006:
        break;
    // PPU data
    case 0x0007:
        break;
    }

    return data;
}

void ppu2C02::cWrite(uint16_t address, uint8_t data) {
    switch (address) {
        // control
    case 0x0000:
        break;
        // mask
    case 0x0001:
        break;
        // status
    case 0x0002:
        break;
        // OAM addr
    case 0x0003:
        break;
        // OAM data
    case 0x0004:
        break;
        // scroll
    case 0x0005:
        break;
        // PPU addr
    case 0x0006:
        break;
        // PPU data
    case 0x0007:
        break;
    }
}

uint8_t ppu2C02::pRead(uint16_t address, bool readonly) {
    uint8_t data = 0x00;
    address &= 0x3FFF;

    return data;
}

void ppu2C02::pWrite(uint16_t address, uint8_t data) {
    address &= 0x3FFF;
}

void ppu2C02::connectCart(const std::shared_ptr<cartridge>& cartr) {
    this->cart = cartr;
}

void ppu2C02::clock()
{
}
