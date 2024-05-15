

#include "ppu2C02.h"

ppu2C02::ppu2C02()
{
	paletteScrn[0x00] = olc::Pixel(84, 84, 84);
	paletteScrn[0x01] = olc::Pixel(0, 30, 116);
	paletteScrn[0x02] = olc::Pixel(8, 16, 144);
	paletteScrn[0x03] = olc::Pixel(48, 0, 136);
	paletteScrn[0x04] = olc::Pixel(68, 0, 100);
	paletteScrn[0x05] = olc::Pixel(92, 0, 48);
	paletteScrn[0x06] = olc::Pixel(84, 4, 0);
	paletteScrn[0x07] = olc::Pixel(60, 24, 0);
	paletteScrn[0x08] = olc::Pixel(32, 42, 0);
	paletteScrn[0x09] = olc::Pixel(8, 58, 0);
	paletteScrn[0x0A] = olc::Pixel(0, 64, 0);
	paletteScrn[0x0B] = olc::Pixel(0, 60, 0);
	paletteScrn[0x0C] = olc::Pixel(0, 50, 60);
	paletteScrn[0x0D] = olc::Pixel(0, 0, 0);
	paletteScrn[0x0E] = olc::Pixel(0, 0, 0);
	paletteScrn[0x0F] = olc::Pixel(0, 0, 0);

	paletteScrn[0x10] = olc::Pixel(152, 150, 152);
	paletteScrn[0x11] = olc::Pixel(8, 76, 196);
	paletteScrn[0x12] = olc::Pixel(48, 50, 236);
	paletteScrn[0x13] = olc::Pixel(92, 30, 228);
	paletteScrn[0x14] = olc::Pixel(136, 20, 176);
	paletteScrn[0x15] = olc::Pixel(160, 20, 100);
	paletteScrn[0x16] = olc::Pixel(152, 34, 32);
	paletteScrn[0x17] = olc::Pixel(120, 60, 0);
	paletteScrn[0x18] = olc::Pixel(84, 90, 0);
	paletteScrn[0x19] = olc::Pixel(40, 114, 0);
	paletteScrn[0x1A] = olc::Pixel(8, 124, 0);
	paletteScrn[0x1B] = olc::Pixel(0, 118, 40);
	paletteScrn[0x1C] = olc::Pixel(0, 102, 120);
	paletteScrn[0x1D] = olc::Pixel(0, 0, 0);
	paletteScrn[0x1E] = olc::Pixel(0, 0, 0);
	paletteScrn[0x1F] = olc::Pixel(0, 0, 0);

	paletteScrn[0x20] = olc::Pixel(236, 238, 236);
	paletteScrn[0x21] = olc::Pixel(76, 154, 236);
	paletteScrn[0x22] = olc::Pixel(120, 124, 236);
	paletteScrn[0x23] = olc::Pixel(176, 98, 236);
	paletteScrn[0x24] = olc::Pixel(228, 84, 236);
	paletteScrn[0x25] = olc::Pixel(236, 88, 180);
	paletteScrn[0x26] = olc::Pixel(236, 106, 100);
	paletteScrn[0x27] = olc::Pixel(212, 136, 32);
	paletteScrn[0x28] = olc::Pixel(160, 170, 0);
	paletteScrn[0x29] = olc::Pixel(116, 196, 0);
	paletteScrn[0x2A] = olc::Pixel(76, 208, 32);
	paletteScrn[0x2B] = olc::Pixel(56, 204, 108);
	paletteScrn[0x2C] = olc::Pixel(56, 180, 204);
	paletteScrn[0x2D] = olc::Pixel(60, 60, 60);
	paletteScrn[0x2E] = olc::Pixel(0, 0, 0);
	paletteScrn[0x2F] = olc::Pixel(0, 0, 0);

	paletteScrn[0x30] = olc::Pixel(236, 238, 236);
	paletteScrn[0x31] = olc::Pixel(168, 204, 236);
	paletteScrn[0x32] = olc::Pixel(188, 188, 236);
	paletteScrn[0x33] = olc::Pixel(212, 178, 236);
	paletteScrn[0x34] = olc::Pixel(236, 174, 236);
	paletteScrn[0x35] = olc::Pixel(236, 174, 212);
	paletteScrn[0x36] = olc::Pixel(236, 180, 176);
	paletteScrn[0x37] = olc::Pixel(228, 196, 144);
	paletteScrn[0x38] = olc::Pixel(204, 210, 120);
	paletteScrn[0x39] = olc::Pixel(180, 222, 120);
	paletteScrn[0x3A] = olc::Pixel(168, 226, 144);
	paletteScrn[0x3B] = olc::Pixel(152, 226, 180);
	paletteScrn[0x3C] = olc::Pixel(160, 214, 228);
	paletteScrn[0x3D] = olc::Pixel(160, 162, 160);
	paletteScrn[0x3E] = olc::Pixel(0, 0, 0);
	paletteScrn[0x3F] = olc::Pixel(0, 0, 0);
	sprScrn = new olc::Sprite(256, 240);
	sprNmTbl[0] = new olc::Sprite(256, 240);
	sprNmTbl[1] = new olc::Sprite(256, 240);
	sprPatternTable[0] = new olc::Sprite(128, 128);
	sprPatternTable[1] = new olc::Sprite(128, 128);
}


ppu2C02::~ppu2C02()
{
	delete sprScrn;
	delete sprNmTbl[0];
	delete sprNmTbl[1];
	delete sprPatternTable[0];
	delete sprPatternTable[1];
}


olc::Sprite& ppu2C02::GetScreen()
{
	return *sprScrn;
}



olc::Sprite& ppu2C02::GetPatternTable(uint8_t i, uint8_t palette)
{
	// Characters on NES
	// ~~~~~~~~~~~~~~~~~
	// The NES stores characters using 2-bit pixels. These are not stored sequentially
	// but in singular bit planes. For example:
	//
	// 2-Bit Pixels       LSB Bit Plane     MSB Bit Plane
	// 0 0 0 0 0 0 0 0	  0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0
	// 0 1 1 0 0 1 1 0	  0 1 1 0 0 1 1 0   0 0 0 0 0 0 0 0
	// 0 1 2 0 0 2 1 0	  0 1 1 0 0 1 1 0   0 0 1 0 0 1 0 0
	// 0 0 0 0 0 0 0 0 =  0 0 0 0 0 0 0 0 + 0 0 0 0 0 0 0 0
	// 0 1 1 0 0 1 1 0	  0 1 1 0 0 1 1 0   0 0 0 0 0 0 0 0
	// 0 0 1 1 1 1 0 0	  0 0 1 1 1 1 0 0   0 0 0 0 0 0 0 0
	// 0 0 0 2 2 0 0 0	  0 0 0 1 1 0 0 0   0 0 0 1 1 0 0 0
	// 0 0 0 0 0 0 0 0	  0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0
	//
	// The planes are stored as 8 bytes of LSB, followed by 8 bytes of MSB

	// Loop through all 16x16 tiles
	for (uint16_t nTileY = 0; nTileY < 16; nTileY++)
	{
		for (uint16_t nTileX = 0; nTileX < 16; nTileX++)
		{
			// 256 bc a single tile consist of 16 bytes (16 x 16
			uint16_t nOffset = nTileY * 256 + nTileX * 16;

			// each tile has 8 rows of 8 pixels
			for (uint16_t row = 0; row < 8; row++)
			{
				uint8_t tile_lsb = pRead(i * 0x1000 + nOffset + row + 0x0000);
				uint8_t tile_msb = pRead(i * 0x1000 + nOffset + row + 0x0008);

				// combine bytes by adding
				for (uint16_t col = 0; col < 8; col++)
				{
					uint8_t pixel = (tile_lsb & 0x01) << 1 | (tile_msb & 0x01); // adding lsb of each byte to get a value between 0-3

					tile_lsb >>= 1; tile_msb >>= 1; 

					sprPatternTable[i]->SetPixel
					(
						nTileX * 8 + (7 - col),
						nTileY * 8 + row,
						GetColorFromPaletteRam(palette, pixel)
					);
				}
			}
		}
	}

	return *sprPatternTable[i];
}


olc::Pixel& ppu2C02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
	// take palette id and multiply by 4
	return paletteScrn[pRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

olc::Sprite& ppu2C02::GetNameTable(uint8_t i)
{
	return *sprNmTbl[i];
}


uint8_t ppu2C02::cRead(uint16_t addr, bool rdonly)
{
	uint8_t data = 0x00;

	if (rdonly)
	{
		switch (addr)
		{
		case 0x0000: // control
			data = control.reg;
			break;
		case 0x0001: // mask
			data = mask.reg;
			break;
		case 0x0002: // status
			data = status.reg;
			break;
		case 0x0003: // OAM Addr
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Addr
			break;
		case 0x0007: // PPU Data
			break;
		}
	}
	else
	{
		switch (addr)
		{
		case 0x0000: // control
			// not readable
			break;

		case 0x0001: // mask
			// not readable
			break;

		case 0x0002: // status
			// only interested in top 3 bits
			data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);

			status.vertical_blank = 0; // reset vertical blank

			// reset Loopy's Address latch flag
			address_latch = 0;
			break;

		case 0x0003: // OAM Address - Not Readable
			break;

		case 0x0004: // OAM Data
			data = pOAM[oam_addr];
			break;

		case 0x0005: // Scroll 
			// Not Readable
			break;

		case 0x0006: // PPU Address 
			// Not Readable
			break;

		case 0x0007: // PPU Data
			data = ppu_data_buffer;
			ppu_data_buffer = pRead(vram_addr.reg);
			if (vram_addr.reg >= 0x3F00) {
				data = ppu_data_buffer;
			}
			vram_addr.reg += (control.increment_mode ? 32 : 1);
			break;
		}
	}

	return data;
}

void ppu2C02::cWrite(uint16_t addr, uint8_t data)
{
	switch (addr)
	{
	case 0x0000: // control
		control.reg = data;
		tram_addr.nametable_x = control.nametable_x;
		tram_addr.nametable_y = control.nametable_y;
		break;
	case 0x0001: // mask
		mask.reg = data;
		break;
	case 0x0002: // status
		break;
	case 0x0003: // OAM Address
		oam_addr = data;
		break;
	case 0x0004: // OAM Data
		pOAM[oam_addr] = data;
		break;
	case 0x0005: // Scroll
		// written in 2 halves
		if (address_latch == 0) {
			// data written into scroll register sets in screen space the pixel offset
			fine_x = data & 0x07; // between 0 and 7 (bottom 3 bits of data)
            //extract coarse x location from the same bit of data
			tram_addr.coarse_x = data >> 3;
			address_latch = 1;
		}
		else {
			tram_addr.fine_y = data & 0x07;
			tram_addr.coarse_y = data >> 3;
			address_latch = 0;
		}
		break;
	case 0x0006: // PPU Address
		if (address_latch == 0)
		{
			// store higher 8-bits of the new ppu address
			tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
			address_latch = 1; // so that next addr stores the high bytes
		}
		else
		{
			// store lower 8-bits of the new ppu address
			tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
			// one a full 16-bit of address info has been written, vram is updated
			vram_addr = tram_addr;
			address_latch = 0;
		}
		break;
	case 0x0007: // PPU Data
		pWrite(vram_addr.reg, data);
		// 1 =  incrementing along the x-axis
		// 32 = incrementing along the y -axis (going down 1 row on the y -axis)
		vram_addr.reg += (control.increment_mode ? 32 : 1);
		break;
	}
}

uint8_t ppu2C02::pRead(uint16_t addr, bool rdonly)
{
	uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (cart->pRead(addr, data)) {

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		// read from pattern memory
		// 1st dimension: chooses whether its the left/right hand side of that array of data
		// chooses by examining the msb of the ppu address
		data = patterntbl[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF) { // name table memory
		addr &= 0x0FFF;

		if (cart->mirror == cartridge::MIRROR::VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF) {
				data = nametbl[0][addr & 0x03FF];
			}
			if (addr >= 0x0400 && addr <= 0x07FF) {
				data = nametbl[1][addr & 0x03FF];
			}
			if (addr >= 0x0800 && addr <= 0x0BFF) {
				data = nametbl[0][addr & 0x03FF];
			}
			if (addr >= 0x0C00 && addr <= 0x0FFF) {
				data = nametbl[1][addr & 0x03FF];
			}
		}
		else if (cart->mirror == cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF) {
				data = nametbl[0][addr & 0x03FF];
			}
			if (addr >= 0x0400 && addr <= 0x07FF) {
				data = nametbl[0][addr & 0x03FF];
			}
			if (addr >= 0x0800 && addr <= 0x0BFF) {
				data = nametbl[1][addr & 0x03FF];
			}
			if (addr >= 0x0C00 && addr <= 0x0FFF) {
				data = nametbl[1][addr & 0x03FF];
			}
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) {
			addr = 0x0000;
		}
		if (addr == 0x0014) {
			addr = 0x0004;
		}
		if (addr == 0x0018) {
			addr = 0x0008;
		}
		if (addr == 0x001C) {
			addr = 0x000C;
		}
		// read directly from memory location
		data = palettetbl[addr] & (mask.grayscale ? 0x30 : 0x3F);
	}

	return data;
}

void ppu2C02::pWrite(uint16_t addr, uint8_t data)
{
	addr &= 0x3FFF;

	if (cart->pWrite(addr, data)) {

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		patterntbl[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF) {
		addr &= 0x0FFF;
		if (cart->mirror == cartridge::MIRROR::VERTICAL) {
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF) {
				nametbl[0][addr & 0x03FF] = data;
			}
			if (addr >= 0x0400 && addr <= 0x07FF) {
				nametbl[1][addr & 0x03FF] = data;
			}
			if (addr >= 0x0800 && addr <= 0x0BFF) {
				nametbl[0][addr & 0x03FF] = data;
			}
			if (addr >= 0x0C00 && addr <= 0x0FFF) {
				nametbl[1][addr & 0x03FF] = data;
			}
		}
		else if (cart->mirror == cartridge::MIRROR::HORIZONTAL) {
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF) {
				nametbl[0][addr & 0x03FF] = data;
			}
			if (addr >= 0x0400 && addr <= 0x07FF) {
				nametbl[0][addr & 0x03FF] = data;
			}
			if (addr >= 0x0800 && addr <= 0x0BFF) {
				nametbl[1][addr & 0x03FF] = data;
			}
			if (addr >= 0x0C00 && addr <= 0x0FFF) {
				nametbl[1][addr & 0x03FF] = data;
			}
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF) {
		addr &= 0x001F;
		if (addr == 0x0010) {
			addr = 0x0000;
		}
		if (addr == 0x0014) {
			addr = 0x0004;
		}
		if (addr == 0x0018) {
			addr = 0x0008;
		}
		if (addr == 0x001C) {
			addr = 0x000C;
		}
		// write directly from memory location
		palettetbl[addr] = data;
	}
}

void ppu2C02::ConnectCartridge(const std::shared_ptr<cartridge>& cartridge)
{
	this->cart = cartridge;
}

void ppu2C02::reset()
{
	fine_x = 0x00;
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;
}

void ppu2C02::clock() {
	auto IncrementScrollX = [&]() {
			// only if we render something
			if (mask.render_background || mask.render_sprites) {
				// if the end of nametable, wrap around
				if (vram_addr.coarse_x == 31) {
					// leave nametable so wrap around [reset]
					vram_addr.coarse_x = 0;
					// flip target nametable bit
					vram_addr.nametable_x = ~vram_addr.nametable_x;
				}
				else {
					// stay in current nametable if not at the end (increment)
					vram_addr.coarse_x++;
				}
			}
		};

	auto IncrementScrollY = [&]() {
			
			if (mask.render_background || mask.render_sprites) {
				// if possible inc the fine y offset
				if (vram_addr.fine_y < 7) {
					// inc y address b/c we're operating on a scanline (1 pixel high) basis
					vram_addr.fine_y++;
				}
				else {

					// Reset fine y offset
					vram_addr.fine_y = 0;

					// Check if we need to swap vertical nametable targets
					if (vram_addr.coarse_y == 29) {
						// reset coarse y offset
						vram_addr.coarse_y = 0;
						// And flip the target nametable bit
						vram_addr.nametable_y = ~vram_addr.nametable_y;
					}
					else if (vram_addr.coarse_y == 31) {
						// ptr is in attribute memory so just wrap around 
						vram_addr.coarse_y = 0;
					}
					else {
						vram_addr.coarse_y++;
					}
				}
			}
		};

	auto TransferAddressX = [&]() {
			// Ony if rendering is enabled
			if (mask.render_background || mask.render_sprites)
			{
				vram_addr.nametable_x = tram_addr.nametable_x;
				vram_addr.coarse_x = tram_addr.coarse_x;
			}
		};

	auto TransferAddressY = [&]() {
			// Ony if rendering is enabled
			if (mask.render_background || mask.render_sprites)
			{
				vram_addr.fine_y = tram_addr.fine_y;
				vram_addr.nametable_y = tram_addr.nametable_y;
				vram_addr.coarse_y = tram_addr.coarse_y;
			}
		};

	auto LoadBackgroundShifters = [&]() {
			// prepares shifter for rendering
			// pixels = load 8-bit word into the bottom of the 16-bit shifter
			bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
			bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

			// palettes = take individual binary bit and inflate to full 8-bit
			bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
			bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
		};

	auto UpdateShifters = [&]() {
			if (mask.render_background) {
				// Shifting background tile pattern row
				bg_shifter_pattern_lo <<= 1;
				bg_shifter_pattern_hi <<= 1;

				// Shifting palette attributes by 1
				bg_shifter_attrib_lo <<= 1;
				bg_shifter_attrib_hi <<= 1;
			}

			if (mask.render_sprites && cycle >= 1 && cycle < 258) {
				for (int i = 0; i < sprite_count; i++) {
					if (spriteScanline[i].x > 0) {
						spriteScanline[i].x--;
					}
					else {
						sprite_shifter_pattern_lo[i] <<= 1;
						sprite_shifter_pattern_hi[i] <<= 1;
					}
				}
			}
		};

	if (scanline >= -1 && scanline < 240) {

		if (scanline == 0 && cycle == 0) {
			// "Odd Frame" cycle skip
			cycle = 1;
		}

		if (scanline == -1 && cycle == 1) {
			// Effectively start of new frame, so clear vertical blank flag
			status.vertical_blank = 0;

			// Clear sprite overflow flag
			status.sprite_overflow = 0;

			// Clear the sprite zero hit flag
			status.sprite_zero_hit = 0;

			// Clear Shifters
			for (int i = 0; i < 8; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
		}


		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			UpdateShifters();
			switch ((cycle - 1) % 8){
			case 0:
				// Load the current background tile pattern and attributes into the "shifter"
				LoadBackgroundShifters();
				bg_next_tile_id = pRead(0x2000 | (vram_addr.reg & 0x0FFF));
				break;
			case 2:				
				bg_next_tile_attrib = pRead(0x23C0 | (vram_addr.nametable_y << 11)
					| (vram_addr.nametable_x << 10)
					| ((vram_addr.coarse_y >> 2) << 3)
					| (vram_addr.coarse_x >> 2));				
				if (vram_addr.coarse_y & 0x02) {
					bg_next_tile_attrib >>= 4;
				}
				if (vram_addr.coarse_x & 0x02) {
					bg_next_tile_attrib >>= 2;
				}
				bg_next_tile_attrib &= 0x03;
				break;

			case 4:
				bg_next_tile_lsb = pRead((control.pattern_background << 12)
					+ ((uint16_t)bg_next_tile_id << 4)
					+ (vram_addr.fine_y) + 0);

				break;
			case 6:
				bg_next_tile_msb = pRead((control.pattern_background << 12)
					+ ((uint16_t)bg_next_tile_id << 4)
					+ (vram_addr.fine_y) + 8);
				break;
			case 7:
				IncrementScrollX();
				break;
			}
		}

		// End of a visible scanline, so increment downwards...
		if (cycle == 256)
		{
			IncrementScrollY();
		}

		//...and reset the x position
		if (cycle == 257)
		{
			LoadBackgroundShifters();
			TransferAddressX();
		}

		// Superfluous reads of tile id at end of scanline
		if (cycle == 338 || cycle == 340)
		{
			bg_next_tile_id = pRead(0x2000 | (vram_addr.reg & 0x0FFF));
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			// End of vertical blank period so reset the Y address ready for rendering
			TransferAddressY();
		}


		// Foreground Rendering -----------------------------
		// Sprite evaluation
		if (cycle == 257 && scanline >= 0)
		{
			std::memset(spriteScanline, 0xFF, 8 * sizeof(sObjectAttributeEntry));

			sprite_count = 0;

			// clear out any residual information in sprite pattern shifters
			for (uint8_t i = 0; i < 8; i++)
			{
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}

			uint8_t nOAMEntry = 0;

			bSpriteZeroHitPossible = false;

			while (nOAMEntry < 64 && sprite_count < 9)
			{
				int16_t diff = ((int16_t)scanline - (int16_t)OAM[nOAMEntry].y);


				if (diff >= 0 && diff < (control.sprite_size ? 16 : 8))
				{
					if (sprite_count < 8)
					{
						// Is this sprite sprite zero?
						if (nOAMEntry == 0)
						{
							// It is, so its possible it may trigger a 
							// sprite zero hit when drawn
							bSpriteZeroHitPossible = true;
						}

						memcpy(&spriteScanline[sprite_count], &OAM[nOAMEntry], sizeof(sObjectAttributeEntry));
						sprite_count++;
					}
				}

				nOAMEntry++;
			} // End of sprite evaluation for next scanline

			// Set sprite overflow flag
			status.sprite_overflow = (sprite_count > 8);
		}

		if (cycle == 340)
		{

			for (uint8_t i = 0; i < sprite_count; i++)
			{

				uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
				uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;

				if (!control.sprite_size)
				{
					// 8x8 Sprite Mode - The control register determines the pattern table
					if (!(spriteScanline[i].attribute & 0x80))
					{
						// Sprite is NOT flipped vertically, i.e. normal    
						sprite_pattern_addr_lo =
							(control.pattern_sprite << 12)  // Which Pattern Table? 0KB or 4KB offset
							| (spriteScanline[i].id << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
							| (scanline - spriteScanline[i].y); // Which Row in cell? (0->7)

					}
					else
					{
						// Sprite is flipped vertically, i.e. upside down
						sprite_pattern_addr_lo =
							(control.pattern_sprite << 12)  // Which Pattern Table? 0KB or 4KB offset
							| (spriteScanline[i].id << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
							| (7 - (scanline - spriteScanline[i].y)); // Which Row in cell? (7->0)
					}

				}
				else
				{
					// 8x16 Sprite Mode - The sprite attribute determines the pattern table
					if (!(spriteScanline[i].attribute & 0x80))
					{
						// Sprite is NOT flipped vertically, i.e. normal
						if (scanline - spriteScanline[i].y < 8)
						{
							// Reading Top half Tile
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)  // Which Pattern Table? 0KB or 4KB offset
								| ((spriteScanline[i].id & 0xFE) << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
								| ((scanline - spriteScanline[i].y) & 0x07); // Which Row in cell? (0->7)
						}
						else
						{
							// Reading Bottom Half Tile
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)  // Which Pattern Table? 0KB or 4KB offset
								| (((spriteScanline[i].id & 0xFE) + 1) << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
								| ((scanline - spriteScanline[i].y) & 0x07); // Which Row in cell? (0->7)
						}
					}
					else
					{
						// Sprite is flipped vertically, i.e. upside down
						if (scanline - spriteScanline[i].y < 8)
						{
							// Reading Top half Tile
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)    // Which Pattern Table? 0KB or 4KB offset
								| (((spriteScanline[i].id & 0xFE) + 1) << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
								| (7 - (scanline - spriteScanline[i].y) & 0x07); // Which Row in cell? (0->7)
						}
						else
						{
							// Reading Bottom Half Tile
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)    // Which Pattern Table? 0KB or 4KB offset
								| ((spriteScanline[i].id & 0xFE) << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
								| (7 - (scanline - spriteScanline[i].y) & 0x07); // Which Row in cell? (0->7)
						}
					}
				}

				// Hi bit plane equivalent is always offset by 8 bytes from lo bit plane
				sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;

				// Now we have the address of the sprite patterns, we can read them
				sprite_pattern_bits_lo = pRead(sprite_pattern_addr_lo);
				sprite_pattern_bits_hi = pRead(sprite_pattern_addr_hi);

				// If the sprite is flipped horizontally, we need to flip the 
				// pattern bytes. 
				if (spriteScanline[i].attribute & 0x40)
				{
					// lambda function "flips" a byte (0b11100000 -> 0b00000111)
					// from https://stackoverflow.com/a/2602885
					auto flipbyte = [](uint8_t b)
						{
							b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
							b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
							b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
							return b;
						};

					// Flip Patterns Horizontally
					sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
					sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
				}
				sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
				sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
			}
		}
	}

	if (scanline == 240)
	{
		// Post Render Scanline - Do Nothing!
	}

	if (scanline >= 241 && scanline < 261)
	{
		if (scanline == 241 && cycle == 1)
		{
			// Effectively end of frame, so set vertical blank flag
			status.vertical_blank = 1;
			if (control.enable_nmi)
				nmi = true;
		}
	}



	// Composition 

	// Background =============================================================
	uint8_t bg_pixel = 0x00;   // The 2-bit pixel to be rendered
	uint8_t bg_palette = 0x00; // The 3-bit index of the palette the pixel indexes

	if (mask.render_background)
	{
		uint16_t bit_mux = 0x8000 >> fine_x;

		// Select Plane pixels by extracting from the shifter 
		// at the required location. 
		uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
		uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

		// Combine to form pixel index
		bg_pixel = (p1_pixel << 1) | p0_pixel;

		// Get palette
		uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
		uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}

	// Foreground =============================================================
	uint8_t fg_pixel = 0x00;   // The 2-bit pixel to be rendered
	uint8_t fg_palette = 0x00; // The 3-bit index of the palette the pixel indexes
	uint8_t fg_priority = 0x00;// A bit of the sprite attribute indicates if its
	// more important than the background
	if (mask.render_sprites) {

		bSpriteZeroBeingRendered = false;

		for (uint8_t i = 0; i < sprite_count; i++)
		{
			// Scanline cycle has "collided" with sprite, shifters taking over
			if (spriteScanline[i].x == 0) {

				// Determine the pixel value
				uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

				fg_palette = (spriteScanline[i].attribute & 0x03) + 0x04;
				fg_priority = (spriteScanline[i].attribute & 0x20) == 0;

				if (fg_pixel != 0)
				{
					if (i == 0) // Is this sprite zero?
					{
						bSpriteZeroBeingRendered = true;
					}

					break;
				}
			}
		}
	}

	uint8_t pixel = 0x00;   // The FINAL Pixel...
	uint8_t palette = 0x00; // The FINAL Palette...

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		// The background pixel is transparent
		// The foreground pixel is transparent
		// No winner, draw "background" colour
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		// The background pixel is transparent
		// The foreground pixel is visible
		// Foreground wins!
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		// The background pixel is visible
		// The foreground pixel is transparent
		// Background wins!
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		// The background pixel is visible
		// The foreground pixel is visible
		// Hmmm...
		if (fg_priority)
		{
			// Foreground cheats its way to victory!
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			// Background is considered more important!
			pixel = bg_pixel;
			palette = bg_palette;
		}

		// Sprite Zero Hit detection
		if (bSpriteZeroHitPossible && bSpriteZeroBeingRendered)
		{
			// Sprite zero is a collision between foreground and background
			// so they must both be enabled
			if (mask.render_background & mask.render_sprites)
			{
				// The left edge of the screen has specific switches to control
				// its appearance. This is used to smooth inconsistencies when
				// scrolling (since sprites x coord must be >= 0)
				if (~(mask.render_background_left | mask.render_sprites_left))
				{
					if (cycle >= 9 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
				else
				{
					if (cycle >= 1 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
			}
		}
	}

	sprScrn->SetPixel(cycle - 1, scanline, GetColorFromPaletteRam(palette, pixel));

	// Advance renderer - it never stops, it's relentless
	cycle++;
	if (cycle >= 341)
	{
		cycle = 0;
		scanline++;
		if (scanline >= 261)
		{
			scanline = -1;
			frame_complete = true;
		}
	}
}
