#include "mapper_000.h"

mapper_000::mapper_000(uint8_t prgBanks, uint8_t chrBanks) : mapper(prgBanks, chrBanks)
{
}

mapper_000::~mapper_000()
{
}

void mapper_000::reset() {

}

bool mapper_000::CPUMapRead(uint16_t address, uint32_t &mappedAddr)
{
	//cartridge only interested in in the range 0x8000 - 0xFFFF
	if (address >= 0x8000 && address <= 0xFFFF) {
		// prgbanks = contains how many 16 kb chunks were loaded as program rom
		// > 1 = working with 32k rom
		// mask address to offset from 0x0000
		// = 1 = 16k rom
		// mirror 16k rom within address range 
		mappedAddr = address & (prgbanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}
	return false;
}

bool mapper_000::CPUMapWrite(uint16_t address, uint32_t &mappedAddr, uint8_t data)
{
	//cartridge only interested in in the range 0x8000 - 0xFFFF
	if (address >= 0x8000 && address <= 0xFFFF) {
		mappedAddr = address & (prgbanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}
	return false;
}

bool mapper_000::PPUMapRead(uint16_t address, uint32_t &mappedAddr)
{
	//cartridge only interested in in the range 0x0000 - 0x1FFF
	if (address >= 0x0000 && address <= 0x1FFF) {
		mappedAddr = address;
		return true;
	}
	return false;
}

bool mapper_000::PPUMapWrite(uint16_t address, uint32_t& mappedAddr)
{
	//cartridge only interested in in the range 0x0000 - 0x1FFF
	if (address >= 0x0000 && address <= 0x1FFF) {
		if (chrbanks == 0) {
			// treat as ram
			mappedAddr = address;
			return true;
		}
	}
	return false;
}
