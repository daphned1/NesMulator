#pragma once
#include <cstdint>

class mapper {
public:
	mapper(uint8_t prg_rom_banks, uint8_t chr_rom_banks);
	~mapper();

public:
	//mappers are abstract based class
	// transforms CPU addr -> PRG ROM offset
	virtual bool CPUMapRead(uint16_t address, uint32_t& mappedAddr) = 0;
	virtual bool CPUMapWrite(uint16_t address, uint32_t& mappedAddr, uint8_t data = 0) = 0;
	// transform PPU addr -> CHR ROM offset
	virtual bool PPUMapRead(uint16_t address, uint32_t& mappedAddr) = 0;
	virtual bool PPUMapWrite(uint16_t address, uint32_t& mappedAddr) = 0;

	virtual void reset() = 0;

protected:
	// stored locally b/c many mappers need this info
	uint8_t prgbanks = 0;
	uint8_t chrbanks = 0;

};

