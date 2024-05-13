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

};
