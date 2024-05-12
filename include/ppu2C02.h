#pragma once

#include <cstdint>
#include <memory>

class ppu2C02
{
public:
	ppu2C02();
	~ppu2C02();

private:
	uint8_t nameTable[2][1024]; //vram
	uint8_t palette[32];
	uint8_t pattern[2][4096];

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

};
