#include "mapper.h"

mapper::mapper(uint8_t prg_rom_banks, uint8_t chr_rom_banks) {
	prgbanks = prg_rom_banks;
	chrbanks = chr_rom_banks;

	reset();
}

mapper::~mapper()
{
}

void mapper::reset() {

}
