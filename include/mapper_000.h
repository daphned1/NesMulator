#pragma once
#include "mapper.h"
class mapper_000 :  public mapper {
public:
	mapper_000(uint8_t prgBanks, uint8_t chrBanks);
	~mapper_000();

public:
	//mappers are abstract based class
	bool CPUMapRead(uint16_t address, uint32_t& mappedAddr) override;
	bool CPUMapWrite(uint16_t address, uint32_t& mappedAddr) override;
	bool PPUMapRead(uint16_t address, uint32_t& mappedAddr) override;
	bool PPUMapWrite(uint16_t address, uint32_t& mappedAddr) override;

};

