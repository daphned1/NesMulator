#pragma once

#include <cstdint>

class cartridge
{
public:
	cartridge();
	~cartridge();

public:
	// communications w/ main bus
	bool cRead(uint16_t address, uint8_t &data);
	bool cWrite(uint16_t address, uint8_t data);

	// comm w/ ppu bus
	bool pRead(uint16_t address, uint8_t &data);
	bool pWrite(uint16_t address, uint8_t data);
};

