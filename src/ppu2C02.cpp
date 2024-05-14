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

    if (readonly) {
        switch (address) {
        case 0x0000: // control
            data = control.reg;
            break;
        case 0x0001: // mask
            data = mask.reg;
            break;
        case 0x0002: // status
            // only interested in top 3 bits
            data = status.reg;
            break;
        case 0x0003: // OAM addr
            break;
        case 0x0004: // OAM data
            break;
        case 0x0005: // scroll
            break;
        case 0x0006: // PPU addr
            break;
        case 0x0007: // PPU data
            break;
        }
    }
    else {
        switch (address) {
        case 0x0000: // control
            // not readable
            break;
        case 0x0001: // mask
            // not readable
            break;
        case 0x0002: // status
            // only interested in top 3 bits
            data = (status.reg & 0xE0) | (pdata_buffer & 0x1F);
            status.verticalBlank = 0; // reset vertical blank
            addr_latch = 0;
            break;
        case 0x0003: // OAM addr
            break;
        case 0x0004: // OAM data
            break;
        case 0x0005: // scroll
            // not readable
            break;
        case 0x0006: // PPU addr
            // not readable
            break;
        case 0x0007: // PPU data
            data = pdata_buffer;
            pdata_buffer = pRead(vram.reg);

            if (vram.reg >= 0x3F00) {
                data = pdata_buffer;
            }
            vram.reg += (control.increment ? 32 : 1);
            break;
        }
    }

    return data;
}

void ppu2C02::cWrite(uint16_t address, uint8_t data) {
    switch (address) {
    case 0x0000: // control
        control.reg = data;
        tram.nmtableX = control.nmtableX;
        tram.nmtableY = control.nmtableY;
        break;
    case 0x0001: // mask
        mask.reg = data;
        break;
    case 0x0002: // status
        break;
    case 0x0003: // OAM addr
        break;
    case 0x0004: // OAM data
        break;
    case 0x0005: // scroll
        // written in 2 halves
        if (addr_latch == 0) {
            // data written into scroll register sets in screen space the pixel offset
            fineX = data & 0x07; // between 0 and 7 (bottom 3 bits of data)
            //extract coarse x location from the same bit of data
            tram.coarseX = data >> 3;
            addr_latch = 1;
        }
        else {
            tram.fineY = data & 0x07;
            tram.coarseY = data >> 3;
            addr_latch = 0;
        }
        break;
    case 0x0006: // PPU addr
        if (addr_latch == 0) {
            // store higher 8-bits of the new ppu address
            tram.reg = (uint16_t)((data & 0x3F) << 8) | (tram.reg & 0x00FF);
            addr_latch = 1; // so that next addr stores the high bytes
        }
        else {
            // store lower 8-bits of the new ppu address
            tram.reg = (tram.reg & 0xFF00) | data;
            // one a full 16-bit of address info has been written, vram is updated
            vram = tram;
            addr_latch = 0;
        }
        break;
    case 0x0007: // PPU data
        pWrite(vram.reg, data);
        // 1 =  incrementing along the x-axis
        // 32 = incrementing along the y -axis (going down 1 row on the y -axis)
        vram.reg += (control.increment ? 32 : 1);
        break;
    }
}

uint8_t ppu2C02::pRead(uint16_t address, bool readonly) {
    uint8_t data = 0x00;
    address &= 0x3FFF;

    if (cart->pRead(address, data)) {
        
    }
    else if (address >= 0x0000 && address <= 0x1FFF) { // pattern memory
        // read from pattern memory
        // 1st dimension: chooses whether its the left/right hand side of that array of data
        // chooses by examining the msb of the ppu address
        data = pattern[(address & 0x1000) >> 12][address & 0x0FFF];
    }
    else if (address >= 0x2000 && address <= 0x3EFF) { // name table memory
        address &= 0x0FFF;
        if (cart->mirror == cartridge::MIRROR::VERTICAL) {
            // vertical
            if (address >= 0x0000 && address <= 0x03FF) {
                data = nameTable[0][address & 0x03FF]; //offset
            }
            if (address >= 0x0400 && address <= 0x07FF) {
                data = nameTable[1][address & 0x03FF];
            }
            if (address >= 0x0800 && address <= 0x0BFF) {
                data = nameTable[0][address & 0x03FF];
            }
            if (address >= 0x0C00 && address <= 0x0FFF) {
                data = nameTable[1][address & 0x03FF];
            }
        }
        else if (cart->mirror == cartridge::MIRROR::HORIZONTAL) {
            // horizontal
            if (address >= 0x0000 && address <= 0x03FF) {
                data = nameTable[0][address & 0x03FF]; //offset
            }
            if (address >= 0x0400 && address <= 0x07FF) {
                data = nameTable[0][address & 0x03FF];
            }
            if (address >= 0x0800 && address <= 0x0BFF) {
                data = nameTable[1][address & 0x03FF];
            }
            if (address >= 0x0C00 && address <= 0x0FFF) {
                data = nameTable[1][address & 0x03FF];
            }
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF) { // palette memory
        // select appropriate index by masking the bottom 5 bits
        address &= 0x001F;

        // mirroring
        if (address == 0x0010) {
            address = 0x0000;
        }
        if (address == 0x0014) {
            address = 0x0004;
        }
        if (address == 0x0018) {
            address = 0x0008;
        }
        if (address == 0x001C) {
            address = 0x000C;
        }

        // read directly from memory location
        data = palette[address] & (mask.grayscale ? 0x30 : 0x3F);
    }

    return data;
}

void ppu2C02::pWrite(uint16_t address, uint8_t data) {
    address &= 0x3FFF;

    if (cart->pWrite(address, data)) {

    }
    else if (address >= 0x0000 && address <= 0x1FFF) { // pattern memory
        pattern[(address & 0x1000) >> 12][address & 0x0FFF] = data;
    }
    else if (address >= 0x2000 && address <= 0x3EFF) { // name table memory
        address &= 0x0FFF;
        if (cart->mirror == cartridge::MIRROR::VERTICAL) {
            // vertical
            if (address >= 0x0000 && address <= 0x03FF) {
                nameTable[0][address & 0x03FF] = data; //offset
            }
            if (address >= 0x0400 && address <= 0x07FF) {
                nameTable[1][address & 0x03FF] = data;
            }
            if (address >= 0x0800 && address <= 0x0BFF) {
                nameTable[0][address & 0x03FF] = data;
            }
            if (address >= 0x0C00 && address <= 0x0FFF) {
                nameTable[1][address & 0x03FF] = data;
            }
        }
        else if (cart->mirror == cartridge::MIRROR::HORIZONTAL) {
            // horizontal
            if (address >= 0x0000 && address <= 0x03FF) {
                nameTable[0][address & 0x03FF] = data; //offset
            }
            if (address >= 0x0400 && address <= 0x07FF) {
                nameTable[0][address & 0x03FF] = data;
            }
            if (address >= 0x0800 && address <= 0x0BFF) {
                nameTable[1][address & 0x03FF] = data;
            }
            if (address >= 0x0C00 && address <= 0x0FFF) {
                nameTable[1][address & 0x03FF] = data;
            }
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF) { // palette memory
        // select appropriate index by masking the bottom 5 bits
        address &= 0x001F;

        // mirroring
        if (address == 0x0010) {
            address = 0x0000;
        }
        if (address == 0x0014) {
            address = 0x0004;
        }
        if (address == 0x0018) {
            address = 0x0008;
        }
        if (address == 0x001C) {
            address = 0x000C;
        }

        // write directly from memory location
        palette[address] = data;
    }
}

void ppu2C02::connectCart(const std::shared_ptr<cartridge>& cartr) {
    this->cart = cartr;
}

void ppu2C02::clock()
{
    auto IncrementScroll_X = [&]() {
        // only if we render something
        if (mask.renderBackground || mask.renderSprites) {
            // if the end of nametable, wrap around
            if (vram.coarseX == 31) {
                // leave nametable so wrap around [reset]
                vram.coarseX = 0;
                // flip target nametable bit
                vram.nmtableX = ~vram.nmtableX;
            }
            else {
                // stay in current nametable if not at the end (increment)
                vram.coarseX++;
            }
        }
    };
    auto IncrementScroll_Y = [&]() {
        if (mask.renderBackground || mask.renderSprites) {
            // if possible inc the fine y offset
            if (vram.fineY < 7) {
                // inc y address b/c we're operating on a scanline (1 pixel high) basis
                vram.fineY++;
            }
            else {
                // reset fine y offset
                vram.fineY = 0;

                // check if we need to swap vertical nametable targets
                if (vram.coarseY == 29) {
                    // reset coarse y offset
                    vram.coarseY = 0;

                    // flip nametable bit
                    vram.nmtableY = ~vram.nmtableY;
                }
                else if (vram.coarseY == 31) {
                    // ptr is in attribute memory so just wrap around 
                    vram.coarseY = 0;
                }
                else {
                    vram.coarseY++;
                }
            }
        }
    };

    auto TransferAddr_X = [&]() {
        // only if rendering is enabled
        if (mask.renderBackground || mask.renderSprites) {
            vram.nmtableX = tram.nmtableX;
            vram.coarseX = tram.coarseX;
        }
    };
    auto TransferAddr_Y = [&]() {
        // only if rendering is enabled
        if (mask.renderBackground || mask.renderSprites) {
            vram.nmtableY = tram.nmtableY;
            vram.coarseY = tram.coarseY;
            vram.fineY = tram.fineY;
        }
    };

    auto loadBgShifters = [&]() {
        // prepares shifter for rendering
        // pixels = load 8-bit word into the bottom of the 16-bit shifter
        bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
        bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

        // palettes = take individual binary bit and inflate to full 8-bit
        bg_shifter_attr_lo = (bg_shifter_attr_lo & 0xFF00) | ((bg_next_tile_attr & 0b01) ? 0xFF : 0x00);
        bg_shifter_attr_hi = (bg_shifter_attr_hi & 0xFF00) | ((bg_next_tile_attr & 0b10) ? 0xFF : 0x00);
    };

    auto updateShifters = [&]() {
        if (mask.renderBackground) {
            // shift all to the right by 1 bit
            bg_shifter_pattern_lo <<= 1;
            bg_shifter_pattern_hi <<= 1;
            bg_shifter_attr_lo <<= 1;
            bg_shifter_attr_hi <<= 1;
        }
    };


    // leave vertical blank period
    if (scanline >= -1 && scanline < 240) { // apply to all visible scanline
        if (scanline == 0 && cycle == 0) {
            cycle = 1; // "odd frame" cycle skip
        }
        
        if (scanline == -1 && cycle == 1) {
            status.verticalBlank = 0;
        }
        
        // for a bunch of cycles on a particular scanline -> extract the tile id, attribute and bit map pattern
        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {
            updateShifters();
            // cycles for preloading the ppu w/ info to render the next 8 pixels
            switch ((cycle - 1) % 8) {
            case 0: // read tile id
                loadBgShifters();
                bg_next_tile_id = pRead(0x2000 | (vram.reg & 0x0FFF));
                break;
            case 2: // read attribute id
                bg_next_tile_attr = pRead(0x23C0 | (vram.nmtableY << 11)
                    | (vram.nmtableX << 10)
                    | ((vram.coarseY >> 2) << 3)
                    | (vram.coarseX >> 2));
                if (vram.coarseY & 0x02) {
                    bg_next_tile_attr >>= 4;
                }
                if (vram.coarseX & 0x02) {
                    bg_next_tile_attr >>= 2;
                }
                bg_next_tile_attr &= 0x03;
                break;
            case 4: // extract lsb plane
                bg_next_tile_lsb = pRead((control.backgroundPattern << 12) + ((uint16_t)bg_next_tile_id << 4) + (vram.fineY) + 0);
                break; 
            case 6: // extract msb plane
                bg_next_tile_msb = pRead((control.backgroundPattern << 12) + ((uint16_t)bg_next_tile_id << 4) + (vram.fineY) + 8);
                break;
            case 7:
                IncrementScroll_X();
                break;
            }
        }

        // end of a scanline -> inc in the y direction
        if (cycle == 256) {
            IncrementScroll_Y();
        }

        // reset x position
        if (cycle == 257) {
            loadBgShifters();
            TransferAddr_X();
        }

        // reads of tile id at the end of scanline
        if (cycle == 338 || cycle == 340) {
            bg_next_tile_id = pRead(0x2000 | (vram.reg & 0x0FFF));
        }

        if (scanline == -1 && cycle >= 280 && cycle < 305) {
            TransferAddr_Y();
        }

    }

    if (scanline == 240) {
        // nothing happens
    }

    if (scanline >= 241 && scanline < 261) {
        if (scanline == 241 && cycle == 1) {
            status.verticalBlank = 1;
            if (control.enableNMI) {
                nmi = true;
            }
        }
    }

    // draw bg
    uint8_t bgPixel = 0x00;
    uint8_t bgPalette = 0x00;

    if (mask.renderBackground) {
        // select which bit of the shift register depending on fine x value
        uint16_t bit_mux = 0x8000 >> fineX;

        // extract bit in shifter
        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

        //combine both pixel planes into a 2-bit word taht rep. pixels
        bgPixel = (p1_pixel << 1) | p0_pixel;

        // palette ------------------------------
        // extract bit in shifter
        uint8_t p0_pal = (bg_shifter_attr_lo & bit_mux) > 0;
        uint8_t p1_pal = (bg_shifter_attr_hi & bit_mux) > 0;

        //combine both pixel planes into a 2-bit word taht rep. pixels
        bgPalette = (p1_pal << 1) | p0_pal;
    }

    // fake noise 
    spriteScreen->SetPixel(cycle - 1, scanline, getColorFromPalette(bgPalette, bgPixel));

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

olc::Sprite& ppu2C02::GetPatternTable(uint8_t x, uint8_t palette)
{
    for (uint16_t tileY = 0; tileY < 16; tileY++) {
        for (uint16_t tileX = 0; tileX < 16; tileX++) {
            // 256 bc a single tile consist of 16 bytes (16 x 16
            uint16_t offset = tileY * 256 + tileX * 16;

            // each tile has 8 rows of 8 pixels
            for (uint16_t row = 0; row < 8; row++) {
                
                uint8_t lsbTile = pRead(x * 0x1000 + offset + row + 0x0000);
                uint8_t msbTile = pRead(x * 0x1000 + offset + row + 0x0008);

                for (uint16_t col = 0; col < 8; col++) {
                    // combine bytes by adding
                    uint8_t pixel = (lsbTile & 0x01) + (msbTile & 0x01); // adding lsb of each byte to get a value between 0-3
                    lsbTile >>= 1;
                    msbTile >>= 1;

                    spritePatternTable[x]->SetPixel(
                        tileX * 8 + (7 - col),
                        tileY * 8 + row,
                        getColorFromPalette(palette, pixel)
                    );
                }
            }
        }
    }
    return *spritePatternTable[x];
}

olc::Pixel& ppu2C02::getColorFromPalette(uint8_t palette, uint8_t pixel) {
    // take palette id and multiply by 4
    return paletteScreen[pRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

void ppu2C02::reset() {
    fineX = 0x00;
    addr_latch = 0x00;
    pdata_buffer = 0x00;
    scanline = 0;
    cycle = 0;
    bg_next_tile_id = 0;
    bg_next_tile_attr = 0x00;
    bg_next_tile_lsb = 0x00;
    bg_next_tile_msb = 0x00;
    bg_shifter_attr_lo = 0x0000;
    bg_shifter_attr_hi = 0x0000;
    bg_shifter_pattern_lo = 0x0000;
    bg_shifter_pattern_hi = 0x0000;
    status.reg = 0x00;
    mask.reg = 0x00;
    control.reg = 0x00;
    vram.reg = 0x0000;
    tram.reg = 0x0000;
}
