#pragma once

#include <cstdint>
#include <memory>

#include "olcPixelGameEngine.h"
#include "cartridge.h"

class ppu2C02
{
public:
	ppu2C02();
	~ppu2C02();

private:
	uint8_t nameTable[2][1024]; //vram
	uint8_t palette[32];
	uint8_t pattern[2][4096];

public:
	// communications w/ main bus
	uint8_t cRead(uint16_t address, bool readonly = false);
	void cWrite(uint16_t address, uint8_t data);

	// comm w/ ppu bus
	uint8_t pRead(uint16_t address, bool readonly = false);
	void pWrite(uint16_t address, uint8_t data);

public:
	// interface
	void connectCart(const std::shared_ptr<cartridge>& cartr);
	void clock();
	void reset();

	bool nmi = false;

private:
	// the cartridge
	std::shared_ptr<cartridge> cart;

private:
	olc::Pixel paletteScreen[0x40];
	//olc::Sprite spriteScreen = olc::Sprite(256, 240); // full screen output
	//olc::Sprite spriteNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) }; // represent graphical depiction of name table
	//olc::Sprite spritePatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) }; // graphical depiction of pattern

	olc::Sprite* spriteScreen;
	olc::Sprite* spriteNameTable[2];
	olc::Sprite* spritePatternTable[2];

public:
	// debuggin
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t x);
	olc::Sprite& GetPatternTable(uint8_t x, uint8_t palette);
	bool frameComplete = false; 

	olc::Pixel& getColorFromPalette(uint8_t palette, uint8_t pixel);

private:
	uint16_t scanline = 0; // row on screen
	uint16_t cycle = 0; // column on screen

	// struct that consists of bit fields to represents important registers
	union {
		struct {
			uint8_t unused : 5;
			uint8_t spriteOverflow : 1;
			uint8_t spriteZeroHit : 1;
			uint8_t verticalBlank : 1;
		};
		uint8_t reg;
	} status;

	// mask register
	// series of switches to determine which is turned on/off
	union {
		struct {
			uint8_t grayscale : 1;
			uint8_t renderBackgroundLeft : 1;
			uint8_t renderSpritesLeft : 1;
			uint8_t renderBackground : 1;
			uint8_t renderSprites : 1;
			uint8_t enhanceRed : 1;
			uint8_t enhanceGreen : 1;
			uint8_t enhanceBlue : 1;
		};
		uint8_t reg;
	} mask;

	union ppuControl {
		struct {
			uint8_t nmtableX : 1;
			uint8_t nmtableY : 1;
			uint8_t increment : 1;
			uint8_t spritePattern : 1;
			uint8_t backgroundPattern : 1;
			uint8_t spriteSize : 1;
			uint8_t slaveMode : 1; // unused
			uint8_t enableNMI : 1;
		};
		uint8_t reg;
	} control;

	uint8_t addr_latch = 0x00; 
	uint8_t pdata_buffer = 0x00; // read data from ppu is delayed by 1 cycle -> need to buffer the byte
	//uint16_t pAddress = 0x0000; // store compiled address

	union loopyReg {
		// loopy implementation from nesWiki
		struct {
			uint16_t coarseX : 5;
			uint16_t coarseY : 5;
			uint16_t nmtableX : 1;
			uint16_t nmtableY : 1;
			uint16_t fineY : 3;
			uint16_t unused : 1;
		};
		uint16_t reg = 0x0000;
	};

	loopyReg vram;
	loopyReg tram;

	uint8_t fineX = 0x00;

	uint8_t bg_next_tile_id = 0x00;
	uint8_t bg_next_tile_attr = 0x00;
	uint8_t bg_next_tile_lsb = 0x00;
	uint8_t bg_next_tile_msb = 0x00;

	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attr_lo = 0x0000;
	uint16_t bg_shifter_attr_hi = 0x0000;

private:
	struct ObjectAttributeEntry {
		uint8_t y; // y position of sprite
		uint8_t id; // tile id from pattern memory
		uint8_t attr; // flag defines how sprite should be rendered
		uint8_t x; // x position of sprite
	} OAM[64];

	uint8_t oam_address = 0x00;

	ObjectAttributeEntry sprite_scanline[8];
	uint8_t sprCount;
	uint8_t sprite_shifter_pattern_lo[8];
	uint8_t sprite_shifter_pattern_hi[8];

	bool spriteZeroHitP = false;
	bool spriteZeroRender = false;

public:
	uint8_t* OAM_ptr = (uint8_t*)OAM;

};
