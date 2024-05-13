#include "ppu2C02.h"

ppu2C02::ppu2C02()
{
    paletteScreen[0x00] = olc::Pixel(84, 84, 84);
    paletteScreen[0x01] = olc::Pixel(0, 30, 116);
    paletteScreen[0x02] = olc::Pixel(8, 16, 144);
    paletteScreen[0x03] = olc::Pixel(48, 0, 136);
    paletteScreen[0x04] = olc::Pixel(68, 0, 100);
    paletteScreen[0x05] = olc::Pixel(92, 0, 48);
    paletteScreen[0x06] = olc::Pixel(84, 4, 0);
    paletteScreen[0x07] = olc::Pixel(60, 24, 0);
    paletteScreen[0x08] = olc::Pixel(32, 42, 0);
    paletteScreen[0x09] = olc::Pixel(8, 58, 0);
    paletteScreen[0x0A] = olc::Pixel(0, 64, 0);
    paletteScreen[0x0B] = olc::Pixel(0, 60, 0);
    paletteScreen[0x0C] = olc::Pixel(0, 50, 60);
    paletteScreen[0x0D] = olc::Pixel(0, 0, 0);
    paletteScreen[0x0E] = olc::Pixel(0, 0, 0);
    paletteScreen[0x0F] = olc::Pixel(0, 0, 0);

    paletteScreen[0x10] = olc::Pixel(152, 150, 152);
    paletteScreen[0x11] = olc::Pixel(8, 76, 196);
    paletteScreen[0x12] = olc::Pixel(48, 50, 236);
    paletteScreen[0x13] = olc::Pixel(92, 30, 228);
    paletteScreen[0x14] = olc::Pixel(136, 20, 176);
    paletteScreen[0x15] = olc::Pixel(160, 20, 100);
    paletteScreen[0x16] = olc::Pixel(152, 34, 32);
    paletteScreen[0x17] = olc::Pixel(120, 60, 0);
    paletteScreen[0x18] = olc::Pixel(84, 90, 0);
    paletteScreen[0x19] = olc::Pixel(40, 114, 0);
    paletteScreen[0x1A] = olc::Pixel(8, 124, 0);
    paletteScreen[0x1B] = olc::Pixel(0, 118, 40);
    paletteScreen[0x1C] = olc::Pixel(0, 102, 120);
    paletteScreen[0x1D] = olc::Pixel(0, 0, 0);
    paletteScreen[0x1E] = olc::Pixel(0, 0, 0);
    paletteScreen[0x1F] = olc::Pixel(0, 0, 0);

    paletteScreen[0x20] = olc::Pixel(236, 238, 236);
    paletteScreen[0x21] = olc::Pixel(76, 154, 236);
    paletteScreen[0x22] = olc::Pixel(120, 124, 236);
    paletteScreen[0x23] = olc::Pixel(176, 98, 236);
    paletteScreen[0x24] = olc::Pixel(228, 84, 236);
    paletteScreen[0x25] = olc::Pixel(236, 88, 180);
    paletteScreen[0x26] = olc::Pixel(236, 106, 100);
    paletteScreen[0x27] = olc::Pixel(212, 136, 32);
    paletteScreen[0x28] = olc::Pixel(160, 170, 0);
    paletteScreen[0x29] = olc::Pixel(116, 196, 0);
    paletteScreen[0x2A] = olc::Pixel(76, 208, 32);
    paletteScreen[0x2B] = olc::Pixel(56, 204, 108);
    paletteScreen[0x2C] = olc::Pixel(56, 180, 204);
    paletteScreen[0x2D] = olc::Pixel(60, 60, 60);
    paletteScreen[0x2E] = olc::Pixel(0, 0, 0);
    paletteScreen[0x2F] = olc::Pixel(0, 0, 0);

    paletteScreen[0x30] = olc::Pixel(236, 238, 236);
    paletteScreen[0x31] = olc::Pixel(168, 204, 236);
    paletteScreen[0x32] = olc::Pixel(188, 188, 236);
    paletteScreen[0x33] = olc::Pixel(212, 178, 236);
    paletteScreen[0x34] = olc::Pixel(236, 174, 236);
    paletteScreen[0x35] = olc::Pixel(236, 174, 212);
    paletteScreen[0x36] = olc::Pixel(236, 180, 176);
    paletteScreen[0x37] = olc::Pixel(228, 196, 144);
    paletteScreen[0x38] = olc::Pixel(204, 210, 120);
    paletteScreen[0x39] = olc::Pixel(180, 222, 120);
    paletteScreen[0x3A] = olc::Pixel(168, 226, 144);
    paletteScreen[0x3B] = olc::Pixel(152, 226, 180);
    paletteScreen[0x3C] = olc::Pixel(160, 214, 228);
    paletteScreen[0x3D] = olc::Pixel(160, 162, 160);
    paletteScreen[0x3E] = olc::Pixel(0, 0, 0);
    paletteScreen[0x3F] = olc::Pixel(0, 0, 0);

    spriteScreen = new olc::Sprite(256, 240);
    spriteNameTable[0] = new olc::Sprite(256, 240);
    spriteNameTable[1] = new olc::Sprite(256, 240);
    spritePatternTable[0] = new olc::Sprite(128, 128);
    spritePatternTable[1] = new olc::Sprite(128, 128);
}

ppu2C02::~ppu2C02()
{
    delete spriteScreen;
    delete spriteNameTable[0];
    delete spriteNameTable[1];
    delete spritePatternTable[0];
    delete spritePatternTable[1];
}

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

    if (cart->pRead(address, data)) {

    }

    return data;
}

void ppu2C02::pWrite(uint16_t address, uint8_t data) {
    address &= 0x3FFF;

    if (cart->pWrite(address, data)) {

    }
}

void ppu2C02::connectCart(const std::shared_ptr<cartridge>& cartr) {
    this->cart = cartr;
}

void ppu2C02::clock()
{
    // fake noise 
    spriteScreen->SetPixel(cycle - 1, scanline, paletteScreen[(rand() % 2) ? 0x3F : 0x30]);

    // never stops
    cycle++;
    if (cycle >= 341) {
        cycle = 0; // reset cycle
        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frameComplete = true;
        }
    }
}

olc::Sprite& ppu2C02::GetScreen()
{
    return *spriteScreen;
}

olc::Sprite& ppu2C02::GetNameTable(uint8_t x)
{
    return *spriteNameTable[x];
}

olc::Sprite& ppu2C02::GetPatternTable(uint8_t x)
{
    return *spritePatternTable[x];
}
