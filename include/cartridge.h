#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#include "mapper_000.h"

class cartridge
{
public:
	cartridge(const std::string& filename);
	~cartridge();

public:
	bool ImageValid();

	enum MIRROR {
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONE_SCREEN_HI,
	} mirror = HORIZONTAL;

private:
	bool imageValid = false;

	std::vector<uint8_t> PRGMemory;
	std::vector<uint8_t> CHRMemory;

	uint8_t mapperID = 0; // which mapper are we using
	// how many banks
	uint8_t PRGBanks = 0;
	uint8_t CHRBanks = 0;

	std::shared_ptr<mapper> mapper_ptr;

public:
	// communications w/ main bus
	bool cRead(uint16_t address, uint8_t &data);
	bool cWrite(uint16_t address, uint8_t data);

	// comm w/ ppu bus
	bool pRead(uint16_t address, uint8_t &data);
	bool pWrite(uint16_t address, uint8_t data);
};

