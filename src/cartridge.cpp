#include "cartridge.h"

cartridge::cartridge(const std::string& filename) {
	struct formatHeader {
		char name[4];
		uint8_t PRGRom_chunks;
		uint8_t CHRRom_chunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t PRGRam_size;
		uint8_t tvSystem;
		uint8_t tvSystem2;
		char unused[5];
	} header;

	std::ifstream ifs;
	ifs.open(filename, std::ifstream::binary);
	if (ifs.is_open()) {
		// read file header
		ifs.read((char*)&header, sizeof(formatHeader));

		// next 512 bytes are for training info (junk)
		if (header.mapper1 & 0x04) {
			ifs.seekg(512, std::ios_base::cur);
		}

		// extract which mapper rom is using
		mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// discover file format
		uint8_t fileType = 1;

		if (fileType == 0) {

		}

		if (fileType == 1) {
			// read how many banks of memory in rom
			PRGBanks = header.PRGRom_chunks;
			PRGMemory.resize(PRGBanks * 16384); // resize vector to that size 
			ifs.read((char*)PRGMemory.data(), PRGMemory.size()); // read data from file directly into vector 

			// do same for char memory
			CHRBanks = header.CHRRom_chunks;
			CHRMemory.resize(CHRBanks * 8192);
			ifs.read((char*)CHRMemory.data(), CHRMemory.size());
		}

		if (fileType == 2) {

		}

		// load appropriate mapper based on id
		switch (mapperID) {
		case 0:
			mapper_ptr = std::make_shared<mapper_000>(PRGBanks, CHRBanks);
			break;
		}

		imageValid = true;
		ifs.close();
	}
}

cartridge::~cartridge()
{
}

bool cartridge::ImageValid()
{
	return imageValid;
}

// use boolean to tell calling system whether cartridge is handling read/write
bool cartridge::cRead(uint16_t address, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	// only transformed if the corresponding mapper routine says the info needs to come from cartridge
	if (mapper_ptr->CPUMapRead(address, mapped_addr)) {
		data = PRGMemory[mapped_addr]; // access data
		return true;
	}
	else {
		return false;
	}
}

bool cartridge::cWrite(uint16_t address, uint8_t data)
{
	uint32_t mapped_addr = 0;
	// only transformed if the corresponding mapper routine says the info needs to come from cartridge
	if (mapper_ptr->CPUMapWrite(address, mapped_addr)) {
		PRGMemory[mapped_addr] = data; // access data
		return true;
	}
	else {
		return false;
	}
}

bool cartridge::pRead(uint16_t address, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	// only transformed if the corresponding mapper routine says the info needs to come from cartridge
	if (mapper_ptr->PPUMapRead(address, mapped_addr)) {
		data = CHRMemory[mapped_addr]; // access data
		return true;
	}
	else {
		return false;
	}
}

bool cartridge::pWrite(uint16_t address, uint8_t data)
{
	uint32_t mapped_addr = 0;
	// only transformed if the corresponding mapper routine says the info needs to come from cartridge
	if (mapper_ptr->PPUMapRead(address, mapped_addr)) {
		CHRMemory[mapped_addr] = data; // access data
		return true;
	}
	else {
		return false;
	}
}
